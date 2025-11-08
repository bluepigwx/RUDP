#pragma once

#include "Net.h"
#include "IOBuffer.h"

/*
网络传输层，实现可靠UDP

传输层消息头定义，固定10字节
+================================+
|  序列号 (31位) | 可靠位 (1位)   |  4字节
+================================+
| 确认序列号(31位)| 确认位 (1位)  |  4字节
+================================+
|      qport (16位)              |  2字节（仅客户端）
+================================+ 
*/


struct NetChannel
{
    int sock;   // 发包用本地socket
    NetAddr RemoteAdr;  // 对端地址

    int outgoing_seq;   // 自己发出去的消息序号
    int incoming_seq;   // 对方发过来的对方的消息序号
    int ack_seq;    // 已经得到过对方确认的消息序号
    int last_reliable_seq;  // 最后一次发送过的可靠消息序号
    
    IBuffer512 MessageBuffer;   // 可靠消息发送缓冲区
    IBuffer512 ReliableBuffer;   // 待确认可靠消息缓冲区
};
// 发送一条可靠消息的方式为往NetChannel的MessageBuffer里写入数据即可，如：chan->MessageBuffer << short
// 发送一条不可靠消息的方式为直接调用NetChan_Transmit，指定data及其len即可


// 初始化网络通道
void NetChan_Setup(int socket, NetChannel* ch, NetAddr* remote);


// 发送无连接状态消息
int NetChan_SendConnetionLessMsg(int sock, char* data, int len, NetAddr* naddr);


// 通过传输层通道发送网络消息，打包传输层包头，自动处理可靠UPD重传
int NetChan_Transmit(NetChannel* ch, char* data, int len);


// 传输层接受网络消息，解包传输层包头，自动处理可靠UPD重传
int NetChan_Process(NetChannel* ch, char* data, int len);

