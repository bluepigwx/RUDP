#pragma once


// 握手协议
enum CONN_LESS_MSG
{
    CONN_LESS_MSG_GETCHALLENGE, // client to server
    CONN_LESS_MSG_CHALLENGE_REP,    // server to client
    CONN_LESS_MSG_CONNECT,  // client to server
    CONN_LESS_MSG_CONNECT_REP,  // server to client
};