#include <WinSock2.h>
#include <ws2tcpip.h> 
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


int Net_Socket(IPSOCK_TYPE type)
{
    int soc = 0;

    if (type == IPSOCK_TYPE_DGRAM)
    {
        soc = (int)socket(AF_INET, SOCK_DGRAM, 0);
    }
    else if (type == IPSOCK_TYPE_STEAM)
    {
        soc = (int)socket(AF_INET, SOCK_STREAM, 0);
    }

    return soc;
}



void Net_CloseSocket(int Soc)
{
    closesocket(Soc);
}


int Net_Bind(int s, NetAddr* nadr)
{
    sockaddr_in sadr;
    memset(&sadr, 0, sizeof(sadr));
    
    sadr.sin_family = AF_INET;
    sadr.sin_addr.s_addr = *(int*)(&nadr->ip);
    sadr.sin_port = nadr->port;
    
    if (bind(s, (sockaddr*)&sadr, sizeof(sadr)) == -1)
    {
        closesocket(s);
        return -1;
    }

    return 0;
}


int Net_StringToNetAdr(const char* ip, unsigned short port, NetAddr* adr)
{
    in_addr tmp;
    if (inet_pton(AF_INET, ip, &tmp) != 1)
    {
        return -1;
    }

    adr->port = port;
    *(int*)&adr->ip = (int)tmp.s_addr;

    return 0;
}


// 地址转换
int Net_SockAdrToNetAdr(struct sockaddr* sadr, NetAddr* nadr)
{
    nadr->port = ((sockaddr_in*)sadr)->sin_port;
    *(int*)&nadr->ip = ((sockaddr_in*)sadr)->sin_addr.s_addr;
    
    return 0;
}


int Net_NetAdrToSockAdr(NetAddr* nadr, struct sockaddr* sadr)
{
    ((struct sockaddr_in*)sadr)->sin_family = AF_INET;
    ((struct sockaddr_in*)sadr)->sin_port = nadr->port;
    ((struct sockaddr_in*)sadr)->sin_addr.s_addr = *(int*)(&nadr->ip);

    return 0;
}



bool Net_CompareNetAdr(NetAddr* ladr, NetAddr* radr)
{
    if (!ladr || ladr != radr)
    {
        return false;
    }
    
    if (ladr->port != radr->port)
    {
        return false;
    }

    if (ladr->ip[0] != radr->ip[0] ||
        ladr->ip[1] != radr->ip[1] ||
        ladr->ip[2] != radr->ip[2] ||
        ladr->ip[3] != radr->ip[3])
    {
        return false;
    }

    return true;
}



