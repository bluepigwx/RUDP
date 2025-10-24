#include "Client.h"
#include "Command.h"

ClientStatic cls;


// 握手协议
extern void CL_StartConnect_f(CmdParam& Param);
extern void CL_CheckForConnect();
extern void CL_ProcessConnectionLess(OBuffer* ob, NetAddr* from);


int CL_Init()
{
    memset(&cls, 0, sizeof(cls));
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
        OBuffer ob(buff.Get(), buff.Size());
        char IsConnectionless = 0;
        ob >> IsConnectionless;

        // 处理无连接包
        if (IsConnectionless == -1)
        {
            CL_ProcessConnectionLess(&ob, &from);
            continue;
        }
        // else
    }
}



void CL_Frame(int)
{
    CL_ReadPacket();
    
    CL_CheckForConnect();
}

