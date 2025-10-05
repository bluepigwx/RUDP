#pragma once

#include "Net.h"
#include "IOBuffer.h"

// 网络传输层，实现可靠UDP


// 链接状态
enum CONN_STATE
{
    CONN_STATE_DISCONNECT,  // 未连接
    CONN_STAT_CONNECTING, // 握手连接中
    CONN_STAT_CONNECTED, // 已经连接上服务器，等待开启游戏
    CONN_STAT_ACTIVE, // 游戏正式开始
};



struct NetChannel
{
    NetAddr RemoteAdr;  // 对端地址

    IBuffer1k ReliableBuffer;   // 发送缓冲
};


// 发送无连接状态消息
int NetChan_SendConnetionLessMsg(int sock, char* data, int len, NetAddr* naddr);


