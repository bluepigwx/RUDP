#pragma once

// 基本ip网络库


// ip地址描述
struct NetAddr
{
    unsigned char ip[4];
    unsigned short port;
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
bool Net_CompareNetAdr(NetAddr* ladr, NetAddr* radr);


// 打印地址
const char* Net_NetAdrToString(NetAddr* nadr);

const char* Net_SockAdtToString(struct sockaddr* sadr);





