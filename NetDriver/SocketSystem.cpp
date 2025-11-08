
#include "SocketSystem.h"
#include "FSocket.h"
#include "../Common/Net.h"

int SocketSystem::Init()
{
    Net_Init();
    
    return 0;
}


void SocketSystem::Shutdown()
{
    Net_Shutdown();
}


FSocket* SocketSystem::CreateSocket(IPSOCK_TYPE type)
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



