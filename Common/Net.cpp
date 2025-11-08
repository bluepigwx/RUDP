#include <WinSock2.h>
#include <ws2tcpip.h> 
#include "Net.h"
#include "IOBuffer.h"

#include <iostream>

#include "Log.h"


static const char* Net_ErrToString(int err)
{
    switch (err)
	{
	case WSAEINTR: return "WSAEINTR";
	case WSAEBADF: return "WSAEBADF";
	case WSAEACCES: return "WSAEACCES";
	case WSAEDISCON: return "WSAEDISCON";
	case WSAEFAULT: return "WSAEFAULT";
	case WSAEINVAL: return "WSAEINVAL";
	case WSAEMFILE: return "WSAEMFILE";
	case WSAEWOULDBLOCK: return "WSAEWOULDBLOCK";
	case WSAEINPROGRESS: return "WSAEINPROGRESS";
	case WSAEALREADY: return "WSAEALREADY";
	case WSAENOTSOCK: return "WSAENOTSOCK";
	case WSAEDESTADDRREQ: return "WSAEDESTADDRREQ";
	case WSAEMSGSIZE: return "WSAEMSGSIZE";
	case WSAEPROTOTYPE: return "WSAEPROTOTYPE";
	case WSAENOPROTOOPT: return "WSAENOPROTOOPT";
	case WSAEPROTONOSUPPORT: return "WSAEPROTONOSUPPORT";
	case WSAESOCKTNOSUPPORT: return "WSAESOCKTNOSUPPORT";
	case WSAEOPNOTSUPP: return "WSAEOPNOTSUPP";
	case WSAEPFNOSUPPORT: return "WSAEPFNOSUPPORT";
	case WSAEAFNOSUPPORT: return "WSAEAFNOSUPPORT";
	case WSAEADDRINUSE: return "WSAEADDRINUSE";
	case WSAEADDRNOTAVAIL: return "WSAEADDRNOTAVAIL";
	case WSAENETDOWN: return "WSAENETDOWN";
	case WSAENETUNREACH: return "WSAENETUNREACH";
	case WSAENETRESET: return "WSAENETRESET";
	case WSAECONNABORTED: return "WSWSAECONNABORTEDAEINTR";
	case WSAECONNRESET: return "WSAECONNRESET";
	case WSAENOBUFS: return "WSAENOBUFS";
	case WSAEISCONN: return "WSAEISCONN";
	case WSAENOTCONN: return "WSAENOTCONN";
	case WSAESHUTDOWN: return "WSAESHUTDOWN";
	case WSAETOOMANYREFS: return "WSAETOOMANYREFS";
	case WSAETIMEDOUT: return "WSAETIMEDOUT";
	case WSAECONNREFUSED: return "WSAECONNREFUSED";
	case WSAELOOP: return "WSAELOOP";
	case WSAENAMETOOLONG: return "WSAENAMETOOLONG";
	case WSAEHOSTDOWN: return "WSAEHOSTDOWN";
	case WSASYSNOTREADY: return "WSASYSNOTREADY";
	case WSAVERNOTSUPPORTED: return "WSAVERNOTSUPPORTED";
	case WSANOTINITIALISED: return "WSANOTINITIALISED";
	case WSAHOST_NOT_FOUND: return "WSAHOST_NOT_FOUND";
	case WSATRY_AGAIN: return "WSATRY_AGAIN";
	case WSANO_RECOVERY: return "WSANO_RECOVERY";
	case WSANO_DATA: return "WSANO_DATA";
	default: return "NO ERROR";
	}
}


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

    if (soc == -1)
    {
        int err = ::WSAGetLastError();
        const char* StrErr = Net_ErrToString(err);
        Log(LOG_CAT_ERR, "Net_Socket fail err[%d] str[%s]", err, StrErr);
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



int Net_Send(int sock, char* data, int len, NetAddr* nadr)
{
    sockaddr saddr;

    Net_NetAdrToSockAdr(nadr, &saddr);

    int ret = sendto(sock, data, len, 0, &saddr, sizeof(saddr));
    if (ret == -1)
    {
        int err = ::WSAGetLastError();
        if (err == WSAEWOULDBLOCK)
        {
            return 0;
        }
    	
    	Log(LOG_CAT_ERR, "net err code[%s] str[%s]", err, Net_ErrToString(err));
        return -1;
    }

    return 0;
}


// 进程内回环，例如Listen Server的主机端，或者纯单机模式获取本地输入
static bool Net_LoopbackGet(IBuffer2k* buff, NetAddr* from)
{
	from->AddrType = ADDR_TYPE_LOOPBACK;
	return false;
}


bool Net_Get(int sock, IBuffer2k* buff, NetAddr* from)
{
	if (Net_LoopbackGet(buff, from))
	{
		return  true;
	}
	
	if (sock == -1)
	{
		return false;
	}
	
	sockaddr saddr;
	int fromelen = sizeof(saddr);
	int ret = recvfrom(sock, buff->Get(), buff->GetMaXSize(), 0, &saddr, &fromelen);
	if (ret == -1)
	{
		if (ret == WSAEWOULDBLOCK)
		{
			return false;
		}

		// 报错的话后面再考虑如何细化处理
		int err = ::WSAGetLastError();
		const char* StrErr = Net_ErrToString(err);
        
		return false;
	}

	buff->SetSize(ret);
	
	Net_SockAdrToNetAdr(&saddr, from);

    return true;
}


