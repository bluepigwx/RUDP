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

    std::vector<CChannel*> Channels;

    std::map<CActor*, CChannel*> ActorChannelMap;

    CNetDriver* NetDriver;
};