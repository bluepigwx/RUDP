#pragma once


#include "../Core/CObject.h"


// 最小的业务实体对象，整个游戏业务由无数CActor组成

class CActor : public CObject
{
    DECLEAR_CLASS(CActor)

    CActor():
    Replicated(false)
    {
        
    }
    
public:
    bool Replicated;
};