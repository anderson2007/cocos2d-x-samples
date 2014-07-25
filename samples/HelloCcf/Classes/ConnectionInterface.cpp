//
//  ConnectionInterface.cpp
//  HelloCcf
//
//  Created by calf on 14-7-24.
//
//

#include "ConnectionInterface.h"
#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;

std::list<tagPEER> ConnectionInterface::_listPeer;
std::mutex ConnectionInterface::_mutex;
bool ConnectionInterface::_bConnect = false;

void ConnectionInterface::OnPeerUpdate(tagPEER peer)
{
    std::list<tagPEER>::iterator iter;
    iter = queryPeer(peer);
    
    _mutex.lock();
    if (iter == _listPeer.end()) {
        _listPeer.push_back(peer);
    }
    else{
        _listPeer.insert(iter, peer);
    }
    
    _mutex.unlock();

    cocos2d::EventCustom event("IntelCCFPeerUpdate");
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void ConnectionInterface::InvitePeer(tagPEER peer)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jclass peercls = JniHelper::getEnv()->FindClass("com/intel/csdk/wrapper/Peer");  //获得Peer类引用
    if(peercls == NULL)
    {
        CCLOG(" can't find class com/intel/csdk/wrapper/Peer");
        return;
    }
    
    jmethodID constrocMID = JniHelper::getEnv()->GetMethodID(peercls,"<init>","(Ljava/lang/String;Ljava/lang/String;ZZZ)V");  //获得得该类型的构造函数
    if(constrocMID == NULL)
    {
        CCLOG(" can't find construct function com/intel/csdk/wrapper/Peer");
        return;
    }
    
    jstring strName = JniHelper::getEnv()->NewStringUTF(peer._sessionName.c_str());
    jstring strId = JniHelper::getEnv()->NewStringUTF(peer._sessionId.c_str());
    
    jobject peer_ojb = JniHelper::getEnv()->NewObject(peercls,constrocMID, strName, strId, (jboolean)peer._availability, (jboolean)peer._availability_cloud, (jboolean)peer._availability_proximity);  //构造一个对象，调用该类的构造函数，并且传递参数
    
    if(peer_ojb == NULL)
    {
        CCLOG(" create objec failed com/intel/csdk/wrapper/Peer");
        return;
    }
    
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/helloccf/AppActivity", "invitePeer", "(Lcom/intel/csdk/wrapper/Peer;)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, peer_ojb);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void ConnectionInterface::ReciveInvite(std::string sessionId)
{
    CCLOG("ReciveInvite");
    cocos2d::EventCustom event("IntelCCFPeerInvite");
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void ConnectionInterface::SendMessage(std::string msg)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jstring strMsg = JniHelper::getEnv()->NewStringUTF(msg.c_str());
    
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/helloccf/AppActivity", "sendMessage", "(Ljava/lang/String;)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, strMsg);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void ConnectionInterface::ReceiveMessage(std::string message)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG("receive msg:%s", message.c_str());
#endif
}

void ConnectionInterface::Disconnect()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/helloccf/AppActivity", "remoteDisconnectJni", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void ConnectionInterface::sendInviteResponse(bool value)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/helloccf/AppActivity", "sendInviteResponse", "(Z)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, (jboolean)value);
        t.env->DeleteLocalRef(t.classID);
    }
    
    _bConnect = value;
#endif
}

std::list<tagPEER>::iterator ConnectionInterface::queryPeer(tagPEER &peer)
{
    _mutex.lock();
    
    std::list<tagPEER>::iterator iter;
    for(iter = _listPeer.begin(); iter != _listPeer.end(); iter++) {
        if( (*iter)._sessionId == peer._sessionId ) {
            break;
        }
    }
    _mutex.unlock();
    
    return iter;
}