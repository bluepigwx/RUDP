#include "Engine.h"

#include "../NetDriver/NetDriver.h"


IMPLAMENT_CLASS_BEGIN(CEngine)
IMPLAMENT_CLASS_END(CEngine, CObject)


int32 CEngine::Init()
{
    NetDriver = NewObject<CNetDriver>(CNetDriver::StaticClass());
    return 0;
}


void CEngine::Tick(float Seconds)
{
    NetDriver->Tick(Seconds);
}


void CEngine::Finit()
{
    if (NetDriver)
    {
        NetDriver->Finit();
        
        DestroyObject(NetDriver);
        NetDriver = nullptr;
    }
}

