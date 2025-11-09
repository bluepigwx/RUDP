#pragma once
#include "../Engine/Engine.h"


class CTestEngine : public CEngine
{
public:
    virtual ~CTestEngine() override {}
    
    virtual int32 Init() override;
};
