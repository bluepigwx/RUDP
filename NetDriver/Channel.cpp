#include "Channel.h"
#include <cassert>

#include "NetConnection.h"
#include "../Engine/Actor.h"

/////////////////////////////////////////////////////////////////////////////////
//CChannel
IMPLAMENT_CLASS_BEGIN(CChannel)
IMPLAMENT_CLASS_END(CChannel, CObject)


int CChannel::Init(CNetConnection* InConnection, int ChIdx)
{
    NetConnection = InConnection;
    Index = ChIdx;

    return 0;
}



/////////////////////////////////////////////////////////////////////////////////
//CActorChannel
IMPLAMENT_CLASS_BEGIN(CActorChannel)
IMPLAMENT_CLASS_END(CActorChannel, CChannel)


void CActorChannel::SetChannelActor(CActor* InActor)
{
    Actor = InActor;

    if (Actor)
    {
        NetConnection->AddActorChannel(Actor, this);

        ActorReplicator = FindOrCreateReplicator(InActor);
    }
}


void CActorChannel::ReplicateActor()
{
    assert(Actor);
    
}


FObjectReplicator* CActorChannel::FindOrCreateReplicator(CObject* InObject)
{
    if (nullptr == InObject)
    {
        return nullptr;
    }

    ObjectReplicatorMap::iterator it = ReplicateMap.find(InObject);
    if (it != ReplicateMap.end())
    {
        return it->second;
    }
    // else
    FObjectReplicator* Replicator = new FObjectReplicator();
    Replicator->InitWithObject(InObject, NetConnection);
    ReplicateMap[InObject] = Replicator;
    
    return Replicator;
}


void CActorChannel::Cleanup()
{
    Actor = nullptr;
    
    // 清理自己的Replicator
    ObjectReplicatorMap::iterator it = ReplicateMap.begin();
    for (; it != ReplicateMap.end(); ++it)
    {
        delete it->second;
    }
    ReplicateMap.clear();

    ActorReplicator = nullptr;
}

