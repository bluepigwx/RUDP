#pragma once

#include "../Core/CObject.h"
#include "EngeinInterface.h"


class CNetDriver;

class CEngine : public CObject, public IEngine
{
    DECLEAR_CLASS(CEngine, CObject)
public:
    CEngine():
    NetDriver(nullptr)
    {
        
    }

    virtual ~CEngine() override {}
    
    virtual int32 Init() override;

    virtual void Tick(float Seconds) override;

    virtual void Finit() override;

protected:
    CNetDriver* NetDriver;
};
