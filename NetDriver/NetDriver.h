#pragma once

#include <queue>
#include <string>


#include "../Core/CObject.h"
#include "NetworkObjectList.h"


struct NetAddr;
class CChannel;
class CNetConnection;
class FRepLayout;


struct FChannelDefine
{
    ClassInfo* Class;

    std::string Name;
};


class CNetDriver : public CObject
{
    DECLEAR_CLASS(CNetDriver)

public:

    virtual int Init();
    
    // 往客户端推送数据
    void TickFlush(float DeltaSeconds);

    CChannel* CreateChannel(std::string& Name);

    void DestroyChannel(CChannel* InCh);

    void AddClientConnection(CNetConnection* ClientConn);

    // 将所有具有网络相关性的CActor复制到NetDriver所包含的客户端链接中去
    // 首先构建网络相关性列表
    // 再次判断网络相关性列表中都Actor是否与Connection有相关性
    // 将Actor复制到Connection的连接中
    virtual int32 ServerReplicateActors(float DeltaSeconds);

    NetworkObjectList& GetActiveObjectList() { return NetObjList.ActiveObjectList; }

    FRepLayout* GetObjectClassReplayout(ClassInfo* Class);

protected:
    //挑选出本帧需要进行复制的Actor
    int32 ServerReplicateActors_BuildConsiderList(NetworkObjectList& OutObjList, float DeltaSeconds);
    // 将筛选出来的Actor按相关性和优先级进行排序
    int32 ServerReplicateActors_PrioritzeActors(CNetConnection* Conn, NetworkObjectList& ObjList, PrioritzeObjectList& OutList);
    // 实际进行网络复制处理
    int32 ServerReplicateActors_ProcessPrioritze(CNetConnection* Conn, PrioritzeObjectList& ObjList);
    
private:
    FNetworkObjectList NetObjList;
    
    // 循环利用，避免频繁构造对象
    typedef std::queue<CChannel*> ChannelQueue;
    ChannelQueue ChannelPool;

    // 分类过的Channel
    std::map<std::string, ChannelQueue> ClassChannelPool;

    // 支持通过名字创建Channel
    typedef std::map<std::string, FChannelDefine> ChannelDefine;
    ChannelDefine ChannelConfig;

    // 如果是服务器的Driver，这里存放所有客户端的连接
    typedef std::map<NetAddr, CNetConnection*> ClientConnectionMap;
    ClientConnectionMap ClientConnections;

    // 每个类型对应一个Replayout
    typedef std::map<ClassInfo*, FRepLayout*> ObjectReplayoutMap;
    ObjectReplayoutMap ReplayoutMap;
};