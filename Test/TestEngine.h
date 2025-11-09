#pragma once
#include "../Engine/Engine.h"


class CTestEngine : public CEngine
{
    DECLEAR_CLASS(CTestEngine, CEngine)
public:
    virtual ~CTestEngine() override {}
    
    virtual int32 Init() override;
};
