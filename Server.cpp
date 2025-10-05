#include "Server.h"

#include "Net.h"


ServerStatic svs;



int SV_Init()
{
    svs.ServerSocket = -1;
    
    return 0;
}

void SV_Finish()
{
}


void SV_ReadPacket()
{
    static IBuffer2k buff;
    buff.Clear();
    
    NetAddr from;
    while (Net_Get(svs.ServerSocket, &buff, &from))
    {
        
    }
}

void SV_Frame(int Msec)
{
    
}