#include "ObjectReplicator.h"

#include <assert.h>

#include "NetDriver.h"
#include "Replayout.h"
#include "../Core/CoreNet.h"

class FNetSerializeCB : public INetSerializeCB
{
public:
    // 更新对象的属性变更列表
    static EReplayoutResult UpdateChangelistMgr(
        const FRepLayout& Replayout,
        FSendingRepState* SendingState,
        FReplicationChangelistMgr& InChangelistMgr,
        const CObject* InObject,
        const uint32 ReplicationFrame,
        const FReplicationFlags Flags,
        const bool bForceCompare
        )
    {
        EReplayoutResult Result = Success;
        
        
        
        return Result;
    }
};


int32 FObjectReplicator::InitWithObject(CObject* InObj, CNetConnection* InConn)
{
    ClassInfo* Class = InObj->GetClassInfo();

    Object = InObj;
    Conn = InConn;
    
    Replayout = Conn->NetDriver->GetObjectClassReplayout(Class);

    InitObjectProperty();
    
    return 0;
}


void FObjectReplicator::InitObjectProperty()
{
    FRepChangedPropertyTracker* Tracker = Conn->NetDriver->FindOrCreatePropertyTracker(Object);
    
    RepState = Replayout->CreateRepState(Tracker);
    
}


void FObjectReplicator::StartReplicating(CActorChannel* InCh)
{
    assert(InCh);
    assert(OwningChannel == nullptr);
    assert(InCh->NetConnection == Conn);

    OwningChannel = InCh;

    ChangelistMgr = Conn->NetDriver->GetReplicationChangelistMgr(Object);
    assert(ChangelistMgr);
}


bool FObjectReplicator::ReplicatePropertes(FOutBunch& Bunch, FReplicationFlags RepFlags)
{
    return true;
}



