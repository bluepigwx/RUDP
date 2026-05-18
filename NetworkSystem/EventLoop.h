#pragma once

#include "../Core/Typedef.h"


class FEventLoop
{
public:
    FEventLoop();

    int32 Init();

    void FInit();

private:
    struct FEVImpl* m_EvImpl;
};