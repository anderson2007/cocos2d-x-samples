//
//  PeerMenuItem.cpp
//  HelloCcf
//
//  Created by calf on 14-7-23.
//
//

#include "PeerMenuItem.h"

///
/// MenuItemImage
///

PeerMenuItem * PeerMenuItem::create(const std::string& normalImage, const std::string& selectedImage, const cocos2d::ccMenuCallback& callback)
{
    PeerMenuItem *ret = new PeerMenuItem();
    if (ret && ret->initWithNormalImage(normalImage, selectedImage, "", callback))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
