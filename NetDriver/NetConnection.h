#pragma once

#include "../Core/CObject.h"
#include "Channel.h"
#include <map>


class CActor;
class CChannel;
class CNetDriver;

class CNetConnection : public CObject
{
    DECLEAR_CLASS(CNetConnection)

    bool Init(CNetDriver* InNetDriver);

    CChannel* CreateChannel(std::string& Name);

private:
    int GetFreeIndex() const;

private:

    // 对于同一个Actor的Channel，客户端和服务端拥有相同的下标
    std::vector<CChannel*> Channels;

    std::map<CActor*, CChannel*> ActorChannelMap;

    CNetDriver* NetDriver;
};