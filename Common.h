#pragma once


#include "IOBuffer.h"


typedef struct
{
    char ip[4];
    unsigned short port;
}NetAddr;


// 网络通道
typedef struct
{
    NetAddr RemoteAddr; // 通道对端的地址
    
    IBuffer1k Message;
}NetChan;