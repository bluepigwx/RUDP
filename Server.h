#pragma once

#include "IOBuffer.h"



typedef enum 
{
    CS_FREE,    // 可以建立链接
    CS_WAITE_TO_CLOSE,   // 等待关闭链接
    CS_CONNECTED,   // 已经建立链接
    CS_SPAWNED, // 已经进入游戏
}CLIENT_STATE;



// 一个客户端链接
typedef struct
{
    CLIENT_STATE cs;

    IBuffer1k datagram; // 发送给客户端的数据暂存区
}ClientConnection;



// 服务端状态实例
struct ServerStatic
{
    
};