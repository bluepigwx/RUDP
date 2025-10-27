#pragma once

#include "Core/CObject.h"


class CTest : public CObject
{
    DECLEAR_CLASS(CTest)

public:
    int aa;
};


void Test();