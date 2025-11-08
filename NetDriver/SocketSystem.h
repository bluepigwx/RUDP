#pragma once


#include "../Common/Net.h"


class FSocket;


// 初始化Socket环境，创建，销毁一个FSocket
class SocketSystem
{
public:
    int Init();

    void Shutdown();

    FSocket* CreateSocket(IPSOCK_TYPE type);

    void ReleaseSocket(FSocket* s);
};


SocketSystem* CreateSocketSystem();