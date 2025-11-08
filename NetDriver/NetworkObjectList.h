#pragma once
#include <map>
#include <set>
#include <vector>

class CNetDriver;
class CActor;


class FNetworkObjectInfo
{
public:
    FNetworkObjectInfo(CActor* InActor) :
    Actor(InActor),
    NextUpdateMSTime(0),
    LastUpdateMSTime(0)
    {
        
    }
    
    CActor* Actor;

    // 下次更新时刻
    int NextUpdateMSTime;

    // 上次更新时刻
    int LastUpdateMSTime;
};


typedef std::set<FNetworkObjectInfo*> NetworkObjectList;
typedef std::vector<FNetworkObjectInfo*> PrioritzeObjectList;


class FNetworkObjectList
{
    friend CNetDriver;
public:
    FNetworkObjectInfo* FindOrAdd(CActor* InActor);

    bool Remove(CActor* InActor);
    
private:
    // 所有可复制网络对象
    std::map<CActor*, FNetworkObjectInfo*> AllObjectList;

    // 激活的可复制对象
    NetworkObjectList ActiveObjectList;
};