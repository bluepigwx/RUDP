#include "TestEngine.h"

#include <assert.h>

#include "TestNetDriver.h"

IMPLAMENT_CLASS_BEGIN(CTestEngine)
IMPLAMENT_CLASS_END(CTestEngine, CEngine)

int32 CTestEngine::Init()
{
    NetDriver = NewObject<CNetDriver>(CTestNetDriver::StaticClass());
    assert(NetDriver);
    NetDriver->Init();
    
    return 0;
}

