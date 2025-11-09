#pragma once

#include "../Core/Typedef.h"

class IEngine
{
public:
    virtual ~IEngine() {}
    
    virtual int32 Init() = 0;

    virtual void Tick(float Seconds) = 0;

    virtual  void Finit() = 0;
};