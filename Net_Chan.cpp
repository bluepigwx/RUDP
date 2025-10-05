#include "Net_Chan.h"
#include "IOBuffer.h"

int NetChan_SendConnetionLessMsg(int sock, char* data, int len, NetAddr* naddr)
{
    char ConnetionLessHead = -1;
    IBuffer128 buff;
    buff << ConnetionLessHead; // 无连接消息头
    buff.Serialize((void*)data, len);
    if (!buff.IsValid())
    {
        return -1;
    }

    return Net_Send(sock, buff.Get(), buff.Size(), naddr);
}