#pragma once
#include <WinSock2.h>
#include "SocketSystem.h"

// 一个封装好的抽象Socket

class FSocket
{
public:
    FSocket(SOCKET s, SOCKET_TYPE t)
        :Socket(s)
        ,Type(t)
    {
        
    }

    int Bind();

    int Accept();

    int Connect();

    int SendTo();

    int RecevFrom();
    
private:
    SOCKET Socket;
    SOCKET_TYPE Type;
};
