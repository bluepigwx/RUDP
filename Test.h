#pragma once

#include "Core/CObject.h"


class CTestAA : public CObject
{
    DECLEAR_CLASS(CTestAA)

public:
    int aa;
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
