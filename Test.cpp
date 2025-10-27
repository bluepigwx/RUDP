#include "Test.h"
#include "Net.h"
#include "Log.h"


void Test()
{
    NetAddr adr;
    Net_StringToNetAdr("127.0.0.1", 1998, &adr);
    Log(LOG_CAT_LOG, "hello %d",5);
}


IMPLAMENT_CLASS(CTest, CObject);


CTest cc;