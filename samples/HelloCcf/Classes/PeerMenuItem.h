//
//  PeerMenuItem.h
//  HelloCcf
//
//  Created by calf on 14-7-23.
//
//

#ifndef __HelloCcf__PeerMenuItem__
#define __HelloCcf__PeerMenuItem__

#include <iostream>
#include "cocos2d.h"
#include "ConnectionInterface.h"

class PeerMenuItem : public cocos2d::MenuItemImage
{
public:
    static PeerMenuItem* create(const std::string&normalImage, const std::string&selectedImage, const cocos2d::ccMenuCallback& callback);
    tagPEER getPeer(){return _peer;}
    void setPeer(tagPEER peer){_peer = peer;}
protected:
    tagPEER _peer;
};
#endif /* defined(__HelloCcf__PeerMenuItem__) */
