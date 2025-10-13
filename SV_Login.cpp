#include "Protocol.h"
#include "IOBuffer.h"
#include "Net.h"
#include "Server.h"

// 服务端处理登陆握手协议


static void SV_ClientGetChallenge(OBuffer* ob, NetAddr* from)
{
    static IBuffer64 ib;
    ib.Clear();
    
    SvChallenge* challenge = nullptr;
    for (int i=0; i<svs.NumChallenge; ++i)
    {
        challenge = &svs.Challenge[i];
        if (Net_CompareNetAdr(&challenge->adr, from))
        {
            // 重复发起握手，忽略
            return;
        }
    }

    if (svs.NumChallenge >= MAX_CHALLENGE_NUM)
    {
        // 通知握手队列已满，稍后再试
        ib << CONN_LESS_MSG_CHALLENGE_FULL_REP;
        NetChan_SendConnetionLessMsg(svs.ServerSocket, ib.Get(), ib.Size(), from);
        return;
    }
    
    challenge = &svs.Challenge[svs.NumChallenge];
    challenge->Msec = svs.Msec;
    challenge->Challenge = rand() % 1000000;
    challenge->adr = *from;
    svs.NumChallenge++;
    
    ib << CONN_LESS_MSG_CHALLENGE_REP;
    ib << challenge->Challenge;

    NetChan_SendConnetionLessMsg(svs.ServerSocket, ib.Get(), ib.Size(), from);
}


static void SV_ClientConnect(OBuffer* ob, NetAddr* from)
{
    static IBuffer64 ib;
    ib.Clear();
    
    SvChallenge* challenge = nullptr;

    for (int i=0; i<svs.NumChallenge; ++i)
    {
        if (Net_CompareNetAdr(&svs.Challenge[i].adr, from))
        {
            challenge = &svs.Challenge[i];
            break;
        }
    }

    if (challenge == nullptr)
    {
        return;
    }

    int challengeNum;
    *ob >> challengeNum;
    if (challenge->Challenge != challengeNum)
    {
        return;
    }

    // 尝试与这个客户端建立连接通道
    if (svs.NumClients >= MAX_CLIENT_NUM)
    {
        // 连接池已满
        ib << CONN_LESS_MSG_CONNECT_FULL_REP;
        NetChan_SendConnetionLessMsg(svs.ServerSocket, ib.Get(), ib.Size(), from);
        return;
    }

    SvClient* cl = &svs.Clients[svs.NumChallenge];
    svs.NumChallenge++;
    memset(cl, 0, sizeof(*cl));

    cl->Msec = svs.Msec;
    cl->Channel.RemoteAdr = *from;

    // 回复连接已建立
    ib << CONN_LESS_MSG_CONNECT_REP;
    NetChan_SendConnetionLessMsg(svs.ServerSocket, ib.Get(), ib.Size(), from);
}


// 握手队列超时处理
void SV_TickChallengeTimeout()
{
    for (int i=0; i<svs.NumChallenge; ++i)
    {
        SvChallenge* challenge = &svs.Challenge[i];
        if (svs.Msec - challenge->Msec > 10000) // 暂定10秒
        {
            if (svs.NumChallenge == 1)
            {
                svs.NumChallenge = 0;
            }
            else
            {
                svs.Challenge[i] = svs.Challenge[svs.NumChallenge -1];
                svs.NumChallenge--;
                continue;
            }
        }
    }
}


void SV_ProcessConnectionLess(OBuffer* ob, NetAddr* from)
{
    char op;
    *ob >> op;

    switch (op)
    {
    case CONN_LESS_MSG_GETCHALLENGE:
        SV_ClientGetChallenge(ob, from);
        break;
    case CONN_LESS_MSG_CONNECT:
        SV_ClientConnect(ob, from);
        break;
    default:
        // nothing to do
        ;
    }
}