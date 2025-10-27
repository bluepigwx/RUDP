#include "Net_Chan.h"
#include "IOBuffer.h"
#include "CVar.h"


float CVarPrintPacket = 0.0f;
static CVar test("printpak", &CVarPrintPacket);


void NetChan_Setup(int socket, NetChannel* ch, NetAddr* remote)
{
    ch->sock = socket;
    ch->RemoteAdr = *remote;
    ch->ReliableBuffer.Clear();
    ch->MessageBuffer.Clear();

    ch->outgoing_seq = 0;
    ch->incoming_seq = 0;
    ch->last_reliable_seq = 0;
    ch->ack_seq = 0;
}



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



// 判断是否需要将待确认可靠消息缓冲区的数据再按可靠upd发送一次
static bool NetChan_NeedReliable(NetChannel* ch)
{
    if (ch->ReliableBuffer.Size() > 0)
        return true;

    return false;
}


int NetChan_Transmit(NetChannel* ch, char* data, int len)
{
    static IBuffer1k SendBuffer;
    SendBuffer.Clear();
    
    if (!ch->MessageBuffer.IsValid())
    {
        return -1;  // 消息缓冲区已经满了，可以断开这条channel了
    }
    
    if (ch->ReliableBuffer.Size() ==0 && ch->MessageBuffer.Size() > 0)
    {
        // 如果可靠消息已经被对端确认过了那么本次可以继续发送可靠消息，否则消息继续缓存在ch->MessageBuffer中等待
        ch->ReliableBuffer.Serialize(ch->MessageBuffer.Get(), ch->MessageBuffer.Size());
        ch->MessageBuffer.Clear();
    }

    bool NeedReliable = NetChan_NeedReliable(ch);

    unsigned int h1 = (ch->outgoing_seq & ~(1<<31)) | (NeedReliable << 31);
    unsigned int h2 = (ch->incoming_seq & ~(1<<31));
    short port = 0;

    ch->outgoing_seq++;

    SendBuffer << h1;
    SendBuffer << h2;
    SendBuffer << port;

    if (ch->ReliableBuffer.Size() > 0)
    {
        SendBuffer.Serialize(ch->ReliableBuffer.Get(), ch->ReliableBuffer.Size());
        ch->last_reliable_seq = ch->outgoing_seq;   // 记录下最新的可靠消息序号
    }

    SendBuffer.Serialize(data, len);

    Net_Send(ch->sock, SendBuffer.Get(), SendBuffer.Size(), &ch->RemoteAdr);
    
    return 0;
}



int NetChan_Process(NetChannel* ch, char* data, int len)
{
    return 0;
}

