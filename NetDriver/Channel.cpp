#include "Channel.h"
#include <cassert>

#include "NetConnection.h"
#include "NetType.h"
#include "../Common/Log.h"
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
    assert(ActorReplicator);
    
    // 避免递归处理
    if (bReplicatingActor)
    {
        Log(LOG_CAT_ERR, "bReplicatingActor");
        return;
    }
    
    bReplicatingActor = true;
    
    // do replication stuff...
    FOutBunch Bunch;
    FReplicationFlags RepFlags;
    
    bool WroteSomething = ActorReplicator->ReplicatePropertes(Bunch, RepFlags);
    
    if (WroteSomething)
    {
        //Send Stuff
    }
    
    bReplicatingActor = false;
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
    // else 创建新的对象复制器
    FObjectReplicator* Replicator = new FObjectReplicator();
    Replicator->InitWithObject(InObject, NetConnection);
    ReplicateMap[InObject] = Replicator;
    
    Replicator->StartReplicating(this);
    
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


