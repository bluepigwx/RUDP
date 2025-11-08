#pragma once

#include "IObuffer.h"
#include "Net.h"
// 基本ip网络库


enum ADDR_TYPE : char
{
    ADDR_TYPE_IP,       //ip地址类型
    ADDR_TYPE_LOOPBACK, //本地回环地址类型
};

struct NetAddr;
bool Net_CompareNetAdr(const NetAddr* ladr, const NetAddr* radr);

// ip地址描述
struct NetAddr
{
    ADDR_TYPE AddrType;
    unsigned char ip[4];
    unsigned short port;

    bool operator==(const NetAddr& Other) const
    {
        return Net_CompareNetAdr(this, &Other);
    }

    bool operator<(const NetAddr& Other) const
    {
        // 首先比较地址类型
        if (AddrType != Other.AddrType)
            return AddrType < Other.AddrType;
        
        // 然后比较IP地址（逐字节比较）
        for (int i = 0; i < 4; ++i)
        {
            if (ip[i] != Other.ip[i])
                return ip[i] < Other.ip[i];
        }
        
        // 最后比较端口
        return port < Other.port;
    }
};


// ip包类型
enum IPSOCK_TYPE
{
    IPSOCK_TYPE_STEAM,
    IPSOCK_TYPE_DGRAM,
};


// 初始化网络库
int Net_Init();

// 反初始化
void Net_Shutdown();

// 创建一个指定类型的Socket
int Net_Socket(IPSOCK_TYPE type);

// 释放指定的Socket
void Net_CloseSocket(int Soc);


int Net_Bind(int s, NetAddr* nadr);


// 地址转换
int Net_SockAdrToNetAdr(struct sockaddr* sadr, NetAddr* nadr);

int Net_NetAdrToSockAdr(NetAddr* nadr, struct sockaddr* sadr);

int Net_StringToNetAdr(const char* ip, unsigned short port, NetAddr* adr);


// 两个地址是否相等
bool Net_CompareNetAdr(const NetAddr* ladr, const NetAddr* radr);


// 打印地址
const char* Net_NetAdrToString(const NetAddr* nadr);
const char* Net_SockAdtToString(const struct sockaddr* sadr);


// 发送
int Net_Send(int sock, char* data, int len, NetAddr* nadr);
// 接受
bool Net_Get(int sock, IBuffer2k* buff, NetAddr* from);