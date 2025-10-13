#pragma once


#define	MAX_OSPATH  128


////////////////////////////////////////////////////////////////
//Common
int Common_Init(int argc, char** argv);

void Common_Run();

void Common_Finish();



////////////////////////////////////////////////////////////////
//COM 获取启动命令行参数
void COM_Init(int argc, char** argv);

int COM_Argc();

const char* COM_Argv(int i);

// 查看指定字符串是否存在
// 查找命中返回字符串下标，否则返回-1
int COM_FindArg(const char* arg);
