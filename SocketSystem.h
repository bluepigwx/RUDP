#pragma once


enum SOCKET_TYPE
{
    SOCKET_STREAM, 
    SOCKET_DGRAM,
};


class FSocket;


// 初始化Socket环境，创建，销毁一个FSocket
class SocketSystem
{
public:
    int Init();

    void Shutdown();

    FSocket* CreateSocket(SOCKET_TYPE type);

    void ReleaseSocket(FSocket* s);
};


SocketSystem* CreateSocketSystem();