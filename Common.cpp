#include "Common.h"
#include "Console.h"
#include "Client.h"
#include "FSocket.h"
#include "Command.h"
#include "Server.h"


#include <Windows.h>
#include <mmsystem.h>
#include <iostream>

#include "Log.h"



#pragma comment(lib, "winmm.lib")

#define MAX_FPS 60.0f	// 控制每秒最大帧率

bool Exit = false;


static void Common_Exit_f(CmdParam& Param)
{
	Exit = true;
}


int Common_Init(int argc, char** argv)
{
	Cmd_Register("exit", Common_Exit_f);

	COM_Init(argc, argv);

	Cmd_Init();
	
	Con_Init();

	Net_Init();

	SV_Init();

	CL_Init();
	
	return 0;
}


static void Common_Frame(int Msec)
{
	// 处理控制台输入
	Con_Frame();

	SV_Frame(Msec);

	CL_Frame(Msec);

	//Log(LOG_CAT_LOG, "Msec %d", Msec);
	
}



void Common_Run()
{
	static int MsecPerFrame = (int)((1.0f / MAX_FPS) * 1000.0f);
	static int OldMsec = ::GetTickCount() - 1;
	
	while (!Exit)
	{
		// 尝试维持固定帧率运行
		::timeBeginPeriod(1);
		int CurrentMsec = ::GetTickCount();
		int DeltaMsec = CurrentMsec - OldMsec;
		
		int SleepMsec = MsecPerFrame - DeltaMsec;
		if (SleepMsec > 1)
		{
			::Sleep(SleepMsec);
			
			CurrentMsec = ::GetTickCount();
			DeltaMsec = CurrentMsec - OldMsec;
			
			//Log(LOG_CAT_LOG, "enter sleep");
		}
		::timeEndPeriod(1);
		OldMsec = CurrentMsec;
		
		// 开启主要帧逻辑
		Common_Frame(DeltaMsec);
	}
}


void Common_Finish()
{
	
}


////////////////////////////////////////////////////////////////
//COM
#define MAX_COM_ARG 128

int com_argc;
char* com_argv[MAX_COM_ARG];


void COM_Init(int argc, char** argv)
{
	if (argc > MAX_COM_ARG)
	{
		return;
	}

	com_argc = argc;
	for (int i=0; i<argc; ++i)
	{
		com_argv[i] = argv[i];
	}
}


int COM_Argc()
{
	return com_argc;
}


const char* COM_Argv(int i)
{
	if (i >= com_argc)
	{
		return nullptr;
	}

	return com_argv[i];
}


// 查看指定字符串是否存在
// 查找命中返回字符串下标，否则返回-1
int COM_FindArg(const char* arg)
{
	for (int i=0; i<com_argc; ++i)
	{
		if (!strcmp(arg, com_argv[i]))
		{
			return i;
		}
	}

	return -1;
}