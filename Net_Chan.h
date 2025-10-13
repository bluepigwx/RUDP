#pragma once

#include "Net.h"
#include "IOBuffer.h"

// 网络传输层，实现可靠UDP



struct NetChannel
{
    NetAddr RemoteAdr;  // 对端地址

    IBuffer1k ReliableBuffer;   // 发送缓冲
};


// 初始化网络通道
void NetChan_Setup(NetChannel* ch, NetAddr* remote);


// 发送无连接状态消息
int NetChan_SendConnetionLessMsg(int sock, char* data, int len, NetAddr* naddr);


