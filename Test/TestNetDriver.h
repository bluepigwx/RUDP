#pragma once

#include "../NetDriver/NetDriver.h"

class CTestNetDriver : public CNetDriver
{
    DECLEAR_CLASS(CTestNetDriver, CNetDriver)
public:

    virtual int Init() override;
    
};
