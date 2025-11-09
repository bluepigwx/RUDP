#pragma once

#include "EngeinInterface.h"
#include "../Core/Typedef.h"

class Launcher
{
private:
    Launcher() {}
public:

    static Launcher& Inst()
    {
        static Launcher ins;
        return ins;
    }
    
    int32 Init();

    void Finit();
};


extern IEngine* GEngine;