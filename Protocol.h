#pragma once


// 握手协议
enum CONN_LESS_MSG : char
{
    CONN_LESS_MSG_GETCHALLENGE, // client to server 首次握手
    CONN_LESS_MSG_CHALLENGE_REP,    // server to client 同意握手
    CONN_LESS_MSG_CHALLENGE_FULL_REP,   // server to client 握手队列已经满
    CONN_LESS_MSG_CONNECT,  // client to server 收到challenge确认，请求建立连接
    CONN_LESS_MSG_CONNECT_REP,  // server to client 通知客户连接通道已经建立
    CONN_LESS_MSG_CONNECT_FULL_REP, // server to client 连接池已满，无法提供服务
};