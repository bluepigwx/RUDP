#pragma once

#include "../Core/CObject.h"
#include <string>

class CNetDriver;
class CNetConnection;

// 特定网络对象的传输层
class CChannel : public CObject
{
    friend CNetDriver;
    
    DECLEAR_CLASS(CChannel)
public:
    CChannel():
    NetConnection(nullptr),
    InUse(false),
    Index(-1)
    {
        
    }
    

    int Init(CNetConnection* InConnection, int ChIdx);

private:
    std::string Name;
    
    bool InUse;
    // 这个下标用来和客户端的Channel一一对应
    // 服务器只要下发这个下标即可
    int Index;

    CNetConnection* NetConnection;
};



// Actor对象的网络传输层
class CActorChannel : public CChannel
{
    DECLEAR_CLASS(CActorChannel)
};