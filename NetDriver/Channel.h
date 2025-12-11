#pragma once

#include <map>

#include "../Core/CObject.h"
#include <string>

#include "ObjectReplicator.h"
#include "../Common/IOBuffer.h"

class CNetDriver;
class CNetConnection;
class CActor;
class FObjectReplicator;


class FOutBunch : public IBuffer1k
{
public:
    
};


// 特定网络对象的传输层
class CChannel : public CObject
{
    friend CNetDriver;
    
    DECLEAR_CLASS(CChannel, CObject)
public:
    CChannel():
    NetConnection(nullptr),
    InUse(false),
    Index(-1)
    {
        
    }

    int Init(CNetConnection* InConnection, int ChIdx);

    virtual void Cleanup() {}

private:
    std::string Name;
    
    bool InUse;
    // 这个下标用来和客户端的Channel一一对应
    // 服务器只要下发这个下标即可
    int32 Index;

public:
    CNetConnection* NetConnection;
};



// Actor对象的网络传输层
class CActorChannel : public CChannel
{
    DECLEAR_CLASS(CActorChannel, CChannel)

public:
    CActorChannel() :
    Actor(nullptr),
    ActorReplicator(nullptr),
    bReplicatingActor(false)
    {
        
    }
    
    void SetChannelActor(CActor* InActor);

    void ReplicateActor();

    virtual void Cleanup() override;

private:
    FObjectReplicator* FindOrCreateReplicator(CObject* InObject);

private:
    CActor* Actor;

    // Actor自己的Replicator，用于驱动在这个通道上的属性比较和数据收发状态维护
    FObjectReplicator* ActorReplicator;
    // Actor以及自己所有组件的
    typedef std::map<CObject*, FObjectReplicator*> ObjectReplicatorMap;
    ObjectReplicatorMap ReplicateMap;
    
    bool bReplicatingActor;
};