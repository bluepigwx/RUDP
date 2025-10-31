#include "Test.h"
#include "Net.h"
#include "Log.h"
#include "Core/ClassInfoUtile.h"
#include <cstddef>  // for offsetof



IMPLAMENT_CLASS_BEGIN(CTestAA, CObject)
    PROPERTY_DEFINE(CTestAA, aa, int, PropertyFlag_Net)
IMPLAMENT_CLASS_END(CTestAA, CObject)


IMPLAMENT_CLASS_BEGIN(CTestBB, CTestAA)
    PROPERTY_DEFINE(CTestBB, bb, int, PropertyFlag_Net)
IMPLAMENT_CLASS_END(CTestBB, CTestAA)


IMPLAMENT_CLASS_BEGIN(CTestCC, CTestBB)
    PROPERTY_DEFINE(CTestCC, cc, int, PropertyFlag_Net)
IMPLAMENT_CLASS_END(CTestCC, CTestBB)



CTestAA a;
CTestBB b;
CTestCC c;


void Test1()
{
    ClassInfo* Temp = CObject::CObjectClassInfo.First;
    while (Temp)
    {
        Temp->BuildupReplicationProps();
        Temp = Temp->Next;
    }
}


void Test2()
{
    NetAddr adr;
    Net_StringToNetAdr("127.0.0.1", 1998, &adr);
    Log(LOG_CAT_LOG, "hello %d",5);
}


void Test3()
{
    c.cc = 123;

    int Value = 0;
    bool Ret = ClassInfo_GetValue<int>(&c, "cc", Value);

    bool Ret2 = Ret;
}

void Test()
{
    Test1();

    Test2();

    Test3();
}







