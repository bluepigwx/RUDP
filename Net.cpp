#include <WinSock2.h>
#include "Net.h"


int Net_Init()
{
    WORD Version = MAKEWORD(1, 1);

    static WSADATA WinSocketData;

    int Ret = ::WSAStartup(Version, &WinSocketData);
    if (Ret != 0)
    {
        return Ret;
    }

    return  Ret;
}


void Net_Shutdown()
{
    ::WSACleanup();
}

