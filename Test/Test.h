#pragma once

#include "../Core/CObject.h"
#include "../Engine/Actor.h"


struct StructOne
{
    DECLEAR_STRUCT(StructOne)
    
    int StructOneInt;
};


class CTestAA : public CActor
{
    DECLEAR_CLASS(CTestAA, CActor)

public:
    int aa;
    int aaAray[3];
};


class CTestBB : public CTestAA
{
    DECLEAR_CLASS(CTestBB, CTestAA)

public:
    int bb;
};


class CTestCC : public CTestBB
{
    DECLEAR_CLASS(CTestCC, CTestBB)

public:
    int cc;
};

void Test();
