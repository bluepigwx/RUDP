#pragma once
#include <map>


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



class FNetworkObjectList
{
public:
    FNetworkObjectInfo* FindOrAdd(CActor* InActor);

    bool Remove(CActor* InActor);
    
private:
    // 所有可复制网络对象
    std::map<CActor*, FNetworkObjectInfo*> AllObjectList;

    // 激活的可复制对象
    std::map<CActor*, FNetworkObjectInfo*> ActiveObjectList;
};