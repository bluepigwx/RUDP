#include <string>
#include <conio.h>
#include <vector>
#include <iostream>

#include "Command.h"


// Cmd和参数buffer
const int MAX_COMMAND_ARGC = 64;
static char* Argv[MAX_COMMAND_ARGC];
static int Argc = 0;

// 控制台命令buffer
const int MAX_COMMAND_LINE_BUFFER = 256;
static char CommandLineBuffer[MAX_COMMAND_LINE_BUFFER];
static int BufferPos = 0;


static void Con_ParseCmd(char* CmdBuffer)
{
    if (!CmdBuffer)
        return;

    while (*CmdBuffer)
    {
        while (*CmdBuffer && *CmdBuffer == ' ') // 跳过开头的空格
            CmdBuffer++;

        if (*CmdBuffer)
        {
            Argv[Argc] = CmdBuffer;
            ++Argc;

            while (*CmdBuffer && *CmdBuffer != ' ') // 记录命令或参数
                CmdBuffer++;

            if (*CmdBuffer)
            {
                *CmdBuffer = 0;
                CmdBuffer++;
            }
        }
    }
}


static void Con_ExecCmd()
{
    if (Argc < 1)
        return;

    CmdFuc Func = Cmd_FindFunc(Argv[0]);
    if (Func == nullptr)
    {
        return;
    }

    // 如果有参数就解析参数
    CmdParam Param;
    if (Argc > 1)
    {
        for (int i = 1; i < Argc; ++i)
        {
            Param.push_back(Argv[i]);
        }
    }

    Func(Param);
}


int Con_Init()
{
    Argc = 0;
    
    BufferPos = 0;
    CommandLineBuffer[BufferPos] = 0;
    
    return 0;
}


void Con_Frame()
{
    if (_kbhit())
    {
        char c = _getch();
        if (c == '\n' || c == '\r')
        {
            CommandLineBuffer[BufferPos] = 0;
            // 解析命令
            Con_ParseCmd(CommandLineBuffer);
            // 执行命令
            Con_ExecCmd();
            // 执行完命令清空buffer
            BufferPos = 0;
            Argc = 0;

            std::cout << std::endl;

            return;
        }

        CommandLineBuffer[BufferPos++] = c;
        std::cout << c;
    }
}