#include "ObjectReplicator.h"

#include <assert.h>

#include "NetDriver.h"
#include "Replayout.h"

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



