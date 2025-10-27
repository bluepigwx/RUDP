#include "Client.h"
#include "Command.h"
#include "CVar.h"
#include "FSocket.h"
#include "Net.h"
#include "Protocol.h"
#include "Log.h"

// 完成客户端登陆握手


float VarClientConnectIntervalue = 1000.0f; // 1秒尝试一次与服务器建立链接
CVar CVarClientConnectIntervalue("ClientConnectIntervalue", &VarClientConnectIntervalue);


void CL_StartConnect_f(CmdParam& Param)
{
    if (Param.size() != 2 || Param[0].empty())
    {
        return;
    }
    
    strncpy_s(cls.ServerURL, sizeof(cls.ServerURL), Param[0].c_str(), sizeof(cls.ServerURL));
    cls.ServerPort = atoi(Param[1].c_str());
    
    if (cls.ClinetSocket != -1)
    {
        Net_CloseSocket(cls.ClinetSocket);
        cls.ClinetSocket = -1;
    }

    cls.ClinetSocket = Net_Socket(IPSOCK_TYPE_DGRAM);
    if (cls.ClinetSocket == -1)
    {
        Log(LOG_CAT_ERR, "CL_StartConnect_f create socket failed");
        return;
    }

    cls.ConnState = CONN_STAT_CONNECTING;
}


void CL_CheckForConnect()
{
    if (cls.ConnState != CONN_STAT_CONNECTING)
    {
        return;
    }

    if (cls.ClinetSocket == -1)
    {
        return;
    }

    int Now = ::GetTickCount();
    if (Now - cls.LastConnectTime < VarClientConnectIntervalue)
    {
        return;
    }

    cls.LastConnectTime = Now;

    // 检测服务器地址是否有效
    NetAddr ServerAddr;
    if (Net_StringToNetAdr(cls.ServerURL, cls.ServerPort, &ServerAddr) != 0)
    {
        cls.ConnState = CONN_STATE_DISCONNECT;
        return;
    }

    char msg = CONN_LESS_MSG_GETCHALLENGE;  //客户端开始握手
    NetChan_SendConnetionLessMsg(cls.ClinetSocket, &msg, sizeof(msg), &ServerAddr);
}



static void CL_SendConnectRequest(OBuffer* ob, NetAddr* from)
{
    NetAddr SvrAdr;
    if (Net_StringToNetAdr(cls.ServerURL, cls.ServerPort, &SvrAdr) != 0)
    {
        cls.ConnState = CONN_STATE_DISCONNECT;
        return;
    }

    if (Net_CompareNetAdr(&SvrAdr, from) == false)
    {
        cls.ConnState = CONN_STATE_DISCONNECT;
        return;
    }

    char msg = CONN_LESS_MSG_CONNECT;
    NetChan_SendConnetionLessMsg(cls.ClinetSocket, &msg, sizeof(msg), &SvrAdr);
}


// 握手队列已满
static void CL_OnChallengeFull(OBuffer* ob, NetAddr* from)
{
    cls.ConnState = CONN_STATE_DISCONNECT;
}



static void CL_SetupNetChan(OBuffer* ob, NetAddr* from)
{
    // 初始化服务端通道
    NetChan_Setup(cls.ClinetSocket, &cls.Channel, from);
    // 已连上服务器，可以开局了
    cls.ConnState = CONN_STAT_CONNECTED;
}


// 服务器已满
static void CL_OnServerClientFull(OBuffer* ob, NetAddr* from)
{
    cls.ConnState = CONN_STATE_DISCONNECT;
}


void CL_ProcessConnectionLess(OBuffer* ob, NetAddr* from)
{
    char op;
    *ob >> op;

    switch (op)
    {
    case CONN_LESS_MSG_CHALLENGE_REP:
        CL_SendConnectRequest(ob, from);
        break;
    case CONN_LESS_MSG_CHALLENGE_FULL_REP:
        CL_OnChallengeFull(ob, from);
        break;
    case CONN_LESS_MSG_CONNECT_REP:
        CL_SetupNetChan(ob, from);
        break;
    case CONN_LESS_MSG_CONNECT_FULL_REP:
        CL_OnServerClientFull(ob, from);
        break;
    default:
        ;
    }
}