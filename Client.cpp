#include "Client.h"
#include "Command.h"

ClientStatic cls;


// 握手协议
extern void CL_StartConnect_f(CmdParam& Param);
extern void CL_CheckForConnect();


int CL_Init()
{
    cls.ConnState = CONN_STATE_DISCONNECT;
    cls.ServerURL[0] = 0;
    cls.ClinetSocket = -1;

    Cmd_Register("connect", CL_StartConnect_f);

    return 0;
}



static void CL_ReadPacket()
{
    static IBuffer2k buff;
    buff.Clear();

    NetAddr from;
    while (Net_Get(cls.ClinetSocket, &buff, &from))
    {
        
    }
}



void CL_Frame(int)
{
    CL_ReadPacket();
    
    CL_CheckForConnect();
}

