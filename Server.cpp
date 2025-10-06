// 服务端主控流程


#include "Server.h"

#include "FSocket.h"
#include "Net.h"

ServerStatic svs;


extern void SV_ProcessConnectionLess(OBuffer* ob, NetAddr* from);
extern void SV_TickChallengeTimeout();


int SV_Init()
{
    memset(&svs, 0, sizeof(svs));
    svs.ServerSocket = -1;
    
    return 0;
}


void SV_Finish()
{
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