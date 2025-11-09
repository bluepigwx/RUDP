#include "TestEngine.h"

#include "TestNetDriver.h"

int32 CTestEngine::Init()
{
    NetDriver = NewObject<CNetDriver>(CTestNetDriver::StaticClass());
    return 0;
}

