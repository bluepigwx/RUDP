#pragma once

#include "../Core/CObject.h"
#include "Channel.h"
#include "../Common/Net.h"
#include <map>


class CActor;
class CChannel;
class CActorChannel;
class CNetDriver;


class CNetConnection : public CObject
{
    friend class CNetDriver;
    
    DECLEAR_CLASS(CNetConnection, CObject)
    
public:
    CNetConnection():
    NetDriver(nullptr)
    {
        memset(&RemoteAddr, 0, sizeof(RemoteAddr));
    }

    bool Init(CNetDriver* InNetDriver);

    CChannel* CreateChannel(std::string& Name);

    void AddActorChannel(CActor* InActor, CChannel* InCh);

    CActorChannel* GetActorChannel(CActor* InActor);

private:
    int GetFreeIndex() const;

public:
    CNetDriver* NetDriver;

private:

    // 对于同一个Actor的Channel，客户端和服务端拥有相同的下标
    std::vector<CChannel*> Channels;

    typedef std::map<CActor*, CActorChannel*> ActorChannelMap;
    ActorChannelMap ActorChannels;

    NetAddr RemoteAddr;
};