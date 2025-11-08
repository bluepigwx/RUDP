#pragma once

#include "../Core/CObject.h"


struct StructOne
{
    DECLEAR_STRUCT(StructOne)
    
    int StructOneInt;
};


class CTestAA : public CObject
{
    DECLEAR_CLASS(CTestAA)

public:
    int aa;
    int aaAray[3];
};


class CTestBB : public CTestAA
{
    DECLEAR_CLASS(CTestBB)

public:
    int bb;
};


class CTestCC : public CTestBB
{
    DECLEAR_CLASS(CTestCC)

public:
    int cc;
};

void Test();
