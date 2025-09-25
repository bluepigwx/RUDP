#include <WinSock2.h>

#include "SocketSystem.h"
#include "FSocket.h"


int SocketSystem::Init()
{
    return 0;
}


void SocketSystem::Shutdown()
{
    
}


FSocket* SocketSystem::CreateSocket(SOCKET_TYPE type)
{
    return nullptr;
}


void SocketSystem::ReleaseSocket(FSocket* s)
{
    
}


SocketSystem* CreateSocketSystem()
{
    return new SocketSystem();
}



