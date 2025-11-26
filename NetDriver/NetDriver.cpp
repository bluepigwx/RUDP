#include "NetDriver.h"
#include <assert.h>
#include "Channel.h"
#include "NetConnection.h"
#include "../Common/Log.h"
#include "../Engine/GlobalNames.h"
#include "Replayout.h"
#include "RepChangedPropertyTracker.h"


IMPLAMENT_CLASS_BEGIN(CNetDriver)
IMPLAMENT_CLASS_END(CNetDriver, CObject)


int CNetDriver::Init()
{
    // 构建Channel动态创建表
    ChannelConfig.clear();

    ChannelConfig[NAME_Actor] = FChannelDefine
    {
        CActorChannel::StaticClass(),
        NAME_Actor,
    };

    return true;
}


CChannel* CNetDriver::CreateChannel(std::string& Name)
{
    ChannelDefine::const_iterator it = ChannelConfig.find(Name);
    if (it == ChannelConfig.end())
    {
        // 不被支持的Channel类型
        Log(LOG_CAT_ERR, "CNetDriver::CreateChannel Invalide Channel class name %s", Name.c_str());
        return nullptr;
    }
    
    CChannel* Ret = nullptr;
    
    ChannelQueue& Queue = ClassChannelPool[Name];
    if (!Queue.empty())
    {
        Ret = Queue.front();
        assert(Ret->InUse == false);
        assert(Ret->Index == -1);
        
        Ret->InUse = true;
        Queue.pop();

        return Ret;
    }

    if (Ret == nullptr)
    {
        const FChannelDefine& Chdef = it->second;
        Ret = NewObject<CChannel>(Chdef.Class);
        assert(Ret != nullptr);
        if (Ret)
        {
            Ret = dynamic_cast<CChannel*>(Chdef.Class->CreateFuncPtr());
            Ret->Name = Name;
            Ret->InUse = true;
        }
        else
        {
            Log(LOG_CAT_ERR, "Invalid Channel class");
        }
    }

    return Ret;
}


void CNetDriver::DestroyChannel(CChannel* InCh, bool Reuse)
{
    if (nullptr == InCh)
    {
        return;
    }

    std::map<std::string, ChannelQueue>::iterator it = ClassChannelPool.find(InCh->Name);
    if (it == ClassChannelPool.end())
    {
        Log(LOG_CAT_ERR, "invalid ch class name %s", InCh->Name.c_str());
        // 未知类型不进入池子
        delete InCh;
        return;
    }

    if (Reuse)
    {
        ChannelQueue& Queue = it->second;
        InCh->Cleanup();
        InCh->InUse = false;
        InCh->Index = -1;
        Queue.push(InCh);
    }
    else
    {
        InCh->Cleanup();
        DestroyObject(InCh);
        InCh = nullptr;
    }
}


void CNetDriver::AddClientConnection(CNetConnection* ClientConn)
{
    NetAddr& addr = ClientConn->RemoteAddr;

    ClientConnectionMap::const_iterator it = ClientConnections.find(addr);
    if (it != ClientConnections.end())
    {
        Log(LOG_CAT_ERR, "dupicated addr %s", Net_NetAdrToString(&addr));
        return;
    }
    
    ClientConnections[addr] = ClientConn;
    Log(LOG_CAT_LOG, "insert net client connection %s", Net_NetAdrToString(&addr));
}


int32 CNetDriver::ServerReplicateActors(float DeltaSeconds)
{
    static NetworkObjectList ConsiderList;
    ConsiderList.clear();

    int Ret = ServerReplicateActors_BuildConsiderList(ConsiderList, DeltaSeconds);
    if (Ret <= 0)
    {
        return 0;
    }

    ClientConnectionMap::iterator it = ClientConnections.begin();
    for (; it != ClientConnections.end(); ++it)
    {
        static PrioritzeObjectList PrioritzeList;
        PrioritzeList.clear();

        // 排序所有与本连接相关的Actor
        ServerReplicateActors_PrioritzeActors(it->second, ConsiderList, PrioritzeList);
        // 真正的复制每个相关Actor到Connection
        ServerReplicateActors_ProcessPrioritze(it->second, PrioritzeList);
    }
    
    return 0;
}


FRepLayout* CNetDriver::GetObjectClassReplayout(ClassInfo* Class)
{
    ObjectReplayoutMap::iterator it = ReplayoutMap.find(Class);
    if (it != ReplayoutMap.end())
    {
        return it->second;
    }

    FRepLayout* NewReplayout = new FRepLayout();
    NewReplayout->InitFromClass(Class);
    ReplayoutMap[Class] = NewReplayout;

    return NewReplayout;
}


FRepChangedPropertyTracker* CNetDriver::FindOrCreatePropertyTracker(CObject* InObject)
{
    ObjectChangedPropertyTrackerMap::iterator it = ObjectChangedPropertyMap.find(InObject);
    if (it != ObjectChangedPropertyMap.end())
    {
        return it->second;
    }

    FRepChangedPropertyTracker* NewTracker = new FRepChangedPropertyTracker();
    ObjectChangedPropertyMap[InObject] = NewTracker;

    FRepLayout* ReplayOut = GetObjectClassReplayout(InObject->GetClassInfo());
    ReplayOut->InitChangedTracker(NewTracker);
    
    return NewTracker;
}


FReplicationChangelistMgr* CNetDriver::GetReplicationChangelistMgr(CObject* InObject)
{
    ObjectReplicationChangelistMgrMap::iterator it = ReplicationChangelistMap.find(InObject);
    if (it != ReplicationChangelistMap.end())
    {
        return it->second;
    }

    FReplicationChangelistMgr* NewMgr = new FReplicationChangelistMgr();
    ReplicationChangelistMap[InObject] = NewMgr;

    return NewMgr;
}



int32 CNetDriver::ServerReplicateActors_BuildConsiderList(NetworkObjectList& OutObjList, float DeltaSeconds)
{
    int32 Size = 0;

    NetworkObjectList& ActiveList = GetActiveObjectList();
    if (ActiveList.empty())
    {
        return Size;
    }

    NetworkObjectList::iterator it = ActiveList.begin();
    for (; it!=ActiveList.end(); ++it)
    {
        OutObjList.insert(*it);
        ++Size;
    }

    return Size;
}



int32 CNetDriver::ServerReplicateActors_PrioritzeActors(CNetConnection* Conn, NetworkObjectList& ObjList, PrioritzeObjectList& OutList)
{
    int Size = 0;
    if (ObjList.empty())
    {
        return 0;
    }

    NetworkObjectList::iterator it = ObjList.begin();
    for (; it!=ObjList.end(); ++it)
    {
        OutList.push_back(*it);
        ++Size;
    }
    
    return Size;
}


int32 CNetDriver::ServerReplicateActors_ProcessPrioritze(CNetConnection* Conn, PrioritzeObjectList& ObjList)
{
    if (ObjList.empty())
    {
        return 0;
    }

    int32 ProcessCnt = 0;

    PrioritzeObjectList::iterator it = ObjList.begin();
    for (; it!=ObjList.end(); ++it)
    {
        FNetworkObjectInfo* ActorInfo = (*it);
        CActor* Actor = ActorInfo->Actor;
        if (!Actor)
        {
            continue;
        }

        CActorChannel* ActorChannel = Conn->GetActorChannel(Actor);
        if (ActorChannel == nullptr)
        {
            // 如果发现并没有与Conn相关的Channel，这里创建一个
            ActorChannel = dynamic_cast<CActorChannel*>(Conn->CreateChannel(NAME_Actor));
            if (ActorChannel)
            {
                ActorChannel->SetChannelActor(Actor);
            }
        }

        if (ActorChannel)
        {
            ActorChannel->ReplicateActor();

            ++ProcessCnt;
        }
    }
    
    return ProcessCnt;
}


void CNetDriver::Tick(float DeltaSeconds)
{
    TickFlush(DeltaSeconds);
}


void CNetDriver::TickFlush(float DeltaSeconds)
{
    ServerReplicateActors(DeltaSeconds);
}


void CNetDriver::Finit()
{
    while (!ChannelPool.empty())
    {
        CChannel* Ch = ChannelPool.front();
        ChannelPool.pop();
        
        DestroyChannel(Ch, false);
        Ch = nullptr;
    }


    {
        ObjectReplayoutMap::iterator it = ReplayoutMap.begin();
        for (; it!=ReplayoutMap.end(); ++it)
        {
            FRepLayout* Replayout = it->second;
            delete Replayout;
            Replayout = nullptr;
        }
        ReplayoutMap.clear();
    }
    

    {
        ObjectChangedPropertyTrackerMap::iterator it = ObjectChangedPropertyMap.begin();
        for (; it!=ObjectChangedPropertyMap.end(); ++it)
        {
            FRepChangedPropertyTracker* Tracker = it->second;
            delete Tracker;
            Tracker = nullptr;
        }
        ObjectChangedPropertyMap.clear();
    }

    {
        ObjectReplicationChangelistMgrMap::iterator it = ReplicationChangelistMap.begin();
        for (; it != ReplicationChangelistMap.end(); ++it)
        {
            FReplicationChangelistMgr* Mgr = it->second;
            delete Mgr;
            Mgr = nullptr;
        }
        ReplicationChangelistMap.clear();
    }
    
}


