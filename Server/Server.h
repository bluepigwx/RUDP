#pragma once

#include "../Common/Net_Chan.h"

// 服务端上的客户端实体
struct SvClient
{
    int Msec;
    
    NetChannel Channel;    
};


// 握手状态维护
struct SvChallenge
{
    int Challenge;
    int Msec;

    NetAddr adr;
};


#define MAX_CHALLENGE_NUM 128  // 最大同时握手数
#define MAX_CLIENT_NUM 128  // 最大服务人数

// 服务端状态实例，负责维护客户端连接
struct ServerStatic
{
    long Msec;  // 进程启动到现在的毫秒
    
    int ServerSocket;

    SvChallenge Challenge[MAX_CHALLENGE_NUM];
    int NumChallenge;

    SvClient Clients[MAX_CLIENT_NUM];
    int NumClients; // 客户端的实际数量
};
extern ServerStatic svs;


// 一局游戏
struct ServerInst
{
    
};
extern ServerInst svinst;

int SV_Init();

void SV_Finish();

void SV_Frame(int Msec);