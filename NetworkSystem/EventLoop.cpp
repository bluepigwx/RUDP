#include "EventLoop.h"
#include "Ev_Impl.h"

FEventLoop::FEventLoop()
{
    m_EvImpl = nullptr;
}


int32 FEventLoop::Init()
{
    return 0;
}

void FEventLoop::FInit()
{
    m_EvImpl = nullptr;
}