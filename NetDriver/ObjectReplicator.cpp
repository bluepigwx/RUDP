#include "ObjectReplicator.h"
#include "NetDriver.h"


int32 FObjectReplicator::InitWithObject(CObject* InObj, CNetConnection* InConn)
{
    ClassInfo* Class = InObj->GetClassInfo();
    
    Replayout = InConn->NetDriver->GetObjectClassReplayout(Class);
    return 0;
}

