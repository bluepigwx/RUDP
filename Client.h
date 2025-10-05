#pragma once

// 客户端主流程控制


#include "Net_Chan.h"
#include "Common.h"

// 客户端状态的实例
struct ClientStatic
{
     CONN_STATE ConnState;   // 记录客户端的连接状态
     char ServerURL[MAX_OSPATH];   // 目标服务器地址
     unsigned short ServerPort;    // 目标服务器端口

     int ClinetSocket;
     NetChannel Channel; // 客户端网络通道
};
extern ClientStatic cls;


int CL_Init();

// 客户端一帧
void CL_Frame(int);
