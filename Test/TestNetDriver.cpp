#include "TestNetDriver.h"
#include "Test.h"
#include "../NetDriver/IPNetConnection.h"


IMPLAMENT_CLASS_BEGIN(CTestNetDriver)
IMPLAMENT_CLASS_END(CTestNetDriver, CNetDriver)



int CTestNetDriver::Init()
{
    Super::Init();
    
    CIPNetConnection* Conn = NewObject<CIPNetConnection>(CIPNetConnection::StaticClass());
    AddClientConnection(Conn);


    CTestAA* AA = NewObject<CTestAA>(CTestAA::StaticClass());

    GetNetworkObjectList().FindOrAdd(AA);
    
    return 0;
}
