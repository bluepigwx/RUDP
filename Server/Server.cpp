// 服务端主控流程


#include "Server.h"
#include "../Common/Common.h"
#include "../NetDriver/FSocket.h"
#include "../Common/Net.h"
#include "../Common/Protocol.h"

ServerStatic svs;


extern void SV_ProcessConnectionLess(OBuffer* ob, NetAddr* from);
extern void SV_TickChallengeTimeout();


int SV_Init()
{
    memset(&svs, 0, sizeof(svs));
    svs.ServerSocket = -1;
    svs.Msec = ::GetTickCount();

    // 判断是否启动服务器
    int i = COM_FindArg("maxclients");
    if (i>=0)
    {
        const char* StrNumClients = COM_Argv(i+1);
        int NumClients = atoi(StrNumClients);
        if (NumClients > 1 && NumClients <= MAX_CLIENT_NUM)
        {
            // 创建服务器
            svs.ServerSocket = Net_Socket(IPSOCK_TYPE_DGRAM);

            NetAddr adr;
            Net_StringToNetAdr("0.0.0.0", DEFAULT_SVR_PORT, &adr);
            Net_Bind(svs.ServerSocket, &adr);
        }
    }

    // 是否加载指定地图
    i = COM_FindArg("map");
    if (i>=0)
    {
        // todo 加载地图....
    }
    
    return 0;
}


void SV_Finish()
{
    if (svs.ServerSocket != -1)
    {
        Net_CloseSocket(svs.ServerSocket);
        svs.ServerSocket = -1;
    }
}


void SV_ReadPacket()
{
    static IBuffer2k buff;
    buff.Clear();
    
    NetAddr from;
    while (Net_Get(svs.ServerSocket, &buff, &from))
    {
        // 先看看是不是无连接包
        OBuffer ob(buff.Get(), buff.Size());
        char IsConnectionLess = 0;
        ob >> IsConnectionLess;

        if (IsConnectionLess == -1)
        {
            // 无连接包
            SV_ProcessConnectionLess(&ob, &from);
            continue;
        }

        // 有连接包
        for (int i=0; i<svs.NumClients; ++i)
        {
            //看看是谁发的包
            SvClient* cl = &svs.Clients[i];
            if (Net_CompareNetAdr(&from, &cl->Channel.RemoteAdr))
            {
                // todo process 有连接包，传输层处理好
                break;
            }
        }
    }
}


void SV_Frame(int Msec)
{
    svs.Msec = ::GetTickCount();

    // 握手队列超时处理
    SV_TickChallengeTimeout();
    
    // 服务端收报处理
    SV_ReadPacket();
}