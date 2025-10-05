#include "Client.h"
#include "Command.h"
#include "Net.h"
#include "Protocol.h"

// 完成客户端登陆握手


void CL_StartConnect_f(CmdParam& Param)
{
    if (Param.size() != 2 || Param[0].empty())
    {
        return;
    }
    
    strncpy_s(cls.ServerURL, sizeof(cls.ServerURL), Param[0].c_str(), sizeof(cls.ServerURL));
    cls.ServerPort = atoi(Param[1].c_str());
    cls.ConnState = CONN_STAT_CONNECTING;

    if (cls.ClinetSocket != -1)
    {
        Net_CloseSocket(cls.ClinetSocket);
    }
    cls.ClinetSocket = Net_Socket(IPSOCK_TYPE_DGRAM);
}


void CL_CheckForConnect()
{
    if (cls.ConnState != CONN_STAT_CONNECTING)
    {
        return;
    }

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