#pragma once

// 基本网络库

// 初始化网络库
int Net_Init();

// 反初始化
void Net_Shutdown();

// 创建一个指定类型的Socket
int Net_Socket();

// 释放指定的Socket
void Net_CloseSocket(int Soc);



