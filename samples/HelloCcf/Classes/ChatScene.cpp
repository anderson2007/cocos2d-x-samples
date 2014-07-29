//
//  ChatLayer.cpp
//  HelloCcf
//
//  Created by calf on 14-7-28.
//
//

#include "ChatScene.h"
#include "ConnectionInterface.h"

USING_NS_CC;

Scene* ChatLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ChatLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
        
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
    rootNode->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    
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
                Director::getInstance()->end();
                #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
                    exit(0);
                #endif
                break;
            default:
                break;
        }
        
    }
}
