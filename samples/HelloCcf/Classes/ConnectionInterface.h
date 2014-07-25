//
//  ConnectionInterface.h
//  HelloCcf
//
//  Implement interface of Intel® Common Connectivity Framework
//  eg. PeerUpdate Invite ReceiveInvite SendMessage ReceiveMessage Disconnect
//
//  Created by calf on 14-7-24.
//
//

#ifndef __HelloCcf__ConnectionInterface__
#define __HelloCcf__ConnectionInterface__

#include <iostream>
#include <list>
#include <iterator>

typedef struct tagPEER{
    std::string _sessionName;
    std::string _sessionId;
    bool _availability;
    bool _availability_cloud;
    bool _availability_proximity;
    bool add;
};

class ConnectionInterface {
public:
    static void OnPeerUpdate(tagPEER peer);

    static void InvitePeer(tagPEER peer);
    
    static void ReciveInvite(std::string sessionId);
    
    static void SendMessage(std::string msg);
    
    static void ReceiveMessage(std::string message);
    
    static void Disconnect();
    
    static std::list<tagPEER> getPeerList(){_mutex.lock(); return _listPeer; _mutex.unlock();}
    
    static void sendInviteResponse(bool value);
protected:
    static std::list<tagPEER>::iterator queryPeer(tagPEER &peer);
    static std::mutex _mutex;
    static std::list<tagPEER> _listPeer;
    static bool _bConnect;
};

#endif /* defined(__HelloCcf__ConnectionInterface__) */
