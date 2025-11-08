#pragma once

#include <queue>
#include <string>

#include "../Core/CObject.h"
#include "NetworkObjectList.h"


class CChannel;


struct FChannelDefine
{
    ClassInfo* Class;

    std::string Name;
};


class CNetDriver : public CObject
{
    DECLEAR_CLASS(CNetDriver)

public:

    int Init();
    
    // 往客户端推送数据
    void TickFlush();

    CChannel* CreateChannel(std::string& Name);

    void DestroyChannel(CChannel* InCh);
    
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
};