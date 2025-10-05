#pragma once

#include "Net_Chan.h"

// 服务端上的客户端实体
struct SvClient
{
    NetChannel Channel;    
};


// 服务端状态实例，负责维护客户端连接
struct ServerStatic
{
    int ServerSocket;

    SvClient* Clients;
    int NumClients; // 客户端的实际数量
};
extern ServerStatic svs;


int SV_Init();

void SV_Finish();

void SV_Frame(int Msec);