#pragma once
#include "NetConnection.h"


class FRepLayout;
class CObject;
class CNetConnection;

class FObjectReplicator
{
public:
    FObjectReplicator():
    Replayout(nullptr)
    {
        
    }

    int32 InitWithObject(CObject* InObj, CNetConnection* InConn);

public:

    FRepLayout* Replayout;
};
