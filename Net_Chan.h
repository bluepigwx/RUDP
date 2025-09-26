#pragma once

#include "Net.h"
#include "IOBuffer.h"

// 网络传输层，实现可靠UDP


// 链接状态
enum CONN_STATE
{
    
};


struct NetChannel
{
    NetAddr RemoteAdr;  // 对端地址

    IBuffer1k ReliableBuffer;   // 发送缓冲
};



