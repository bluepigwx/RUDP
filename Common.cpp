#include "Common.h"
#include "Console.h"
#include "Client.h"
#include "FSocket.h"
#include "Command.h"

#include <Windows.h>
#include <mmsystem.h>
#include <iostream>

#include "Log.h"
#include "Server.h"


#pragma comment(lib, "winmm.lib")

#define MAX_FPS 60.0f	// 控制每秒最大帧率

bool Exit = false;


static void Common_Exit_f(CmdParam& Param)
{
	Exit = true;
}


int Common_Init()
{
	Cmd_Register("exit", Common_Exit_f);

	Cmd_Init();
	
	Con_Init();
	
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
	static int MsecPerFrame = ((1.0f / MAX_FPS) * 1000.0f);
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