#include "NetConnection.h"
#include "NetDriver.h"


IMPLAMENT_CLASS_BEGIN(CNetConnection)
IMPLAMENT_CLASS_END(CNetConnection, CObject)



bool CNetConnection::Init(CNetDriver* InNetDriver)
{
    Channels.resize(32768, nullptr);

    NetDriver = InNetDriver;
    
    return true;
}


int CNetConnection::GetFreeIndex() const
{
    int NewIndex = -1;
    for (int i=0; i<Channels.size(); ++i)
    {
        if (!Channels[i])
        {
            NewIndex = i;
            break;
        }
    }

    return NewIndex;
}



CChannel* CNetConnection::CreateChannel(std::string& Name)
{
    int ChIdx = GetFreeIndex();
    if (ChIdx == -1)
    {
        return nullptr;
    }
    
    CChannel* NewChannel = NetDriver->CreateChannel(Name);
    if (NewChannel == nullptr)
    {
        return nullptr;
    }

    NewChannel->Init(this, ChIdx);

    return NewChannel;
}

