//
//  ChatLayer.cpp
//  HelloCcf
//
//  Created by calf on 14-7-28.
//
//

#include "ChatScene.h"
#include "ConnectionInterface.h"
#include "HelloWorldScene.h"
USING_NS_CC;

Scene* ChatLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ChatLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    layer->setTag(10012);
        
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ChatLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //读取导出的json文件，并将编辑器中的画面添加到游戏界面
    rootNode =cocostudio::timeline::NodeReader::getInstance()->createNode("ChatProject/ChatProject.json");
    this->addChild(rootNode,0);
    
    Node* child = rootNode->getChildByTag(210);
    CCASSERT(child!=NULL, "child 210 should not be null!");
    
    Button *sendBtn = (Button*)child->getChildByTag(212);
    CCASSERT(sendBtn!=NULL, "child 212 should not be null!");
    sendBtn->addTouchEventListener(CC_CALLBACK_2(ChatLayer::touchEvent,this));
    
    Button *closeBtn = (Button*)child->getChildByTag(217);
    CCASSERT(closeBtn!=NULL, "child 218 should not be null!");
    closeBtn->addTouchEventListener(CC_CALLBACK_2(ChatLayer::touchEvent,this));
        
    listView = (ListView*)child->getChildByTag(214);
    CCASSERT(listView!=NULL, "child 214 should not be null!");
    
    textField = (TextField *)child->getChildByTag(211);
    CCASSERT(textField!=NULL, "child 211 should not be null!");
    textField->setTextHorizontalAlignment(TextHAlignment::LEFT);
    
    auto peerListener1 = EventListenerCustom::create("IntelCCFReceiveMessage", CC_CALLBACK_1(ChatLayer::listenToReceiveMessage, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(peerListener1, this);
    
    auto peerListener2 = EventListenerCustom::create("IntelCCFDisconnect", CC_CALLBACK_1(ChatLayer::listenToDisconnect, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(peerListener2, this);
    
    return true;
}

void ChatLayer::touchEvent(Ref*pSender, Widget::TouchEventType type)
{
    Button *btn = (Button *)pSender;
    if (type == Widget::TouchEventType::ENDED)
    {
        switch (btn->getTag()) {
            case 212:
            {
                ConnectionInterface::SendMessage(textField->getStringValue());
                
                Text* message = Text::create(textField->getStringValue(),"fonts/Marker Felt.ttf",14);
                message->ignoreContentAdaptWithSize(true);
                message->setTextHorizontalAlignment(TextHAlignment::LEFT);
                message->setTouchScaleChangeEnabled(false);
                message->setTouchEnabled(false);

                listView->pushBackCustomItem(message);
                textField->setText("");
            }
                break;
            case 217:
                ConnectionInterface::Disconnect();
                auto helloworld = HelloWorld::createScene();
                Director::getInstance()->replaceScene(helloworld);
                break;
        }
        
    }
}

void ChatLayer::listenToReceiveMessage(EventCustom *event)
{
    this->schedule(schedule_selector(ChatLayer::scheduleReceiveMessage));
}

void ChatLayer::scheduleReceiveMessage(float dt)
{
    std::list<std::string>::iterator iter;
    std::list<std::string> listMessage;
    ConnectionInterface::getMessageList(listMessage);
    
    while (listMessage.size()) {
        std::string msg = listMessage.front();
        
        Text* message = Text::create(msg,"fonts/Marker Felt.ttf",14);
        message->ignoreContentAdaptWithSize(true);
        message->setTextHorizontalAlignment(TextHAlignment::LEFT);
        message->setTouchScaleChangeEnabled(false);
        message->setTouchEnabled(false);
        listView->pushBackCustomItem(message);
        
        listMessage.pop_front();
    }
}

void ChatLayer::listenToDisconnect(EventCustom *event)
{
    this->schedule(schedule_selector(ChatLayer::scheduleDisconnect));
}

void ChatLayer::scheduleDisconnect(float dt)
{
    auto helloworld = HelloWorld::createScene();
    Director::getInstance()->replaceScene(helloworld);
}


