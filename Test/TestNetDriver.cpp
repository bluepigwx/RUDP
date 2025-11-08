#include "TestNetDriver.h"

#include "../NetDriver/IPNetConnection.h"


IMPLAMENT_CLASS_BEGIN(CTestNetDriver)
    IMPLAMENT_CLASS_END(CTestNetDriver, CNetDriver)



int CTestNetDriver::Init()
{
    CIPNetConnection* Conn = NewObject<CIPNetConnection>(CIPNetConnection::StaticClass());

    return 0;
}
