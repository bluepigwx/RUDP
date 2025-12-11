#pragma once
#include "../Core/Typedef.h"


struct FReplicationFlags
{
    union 
    {
        struct
        {
            int8 bNetOwner : 1;
            int8 bNetInitial : 1;
        };
        
        int32 Value;
    };
    
    FReplicationFlags() { Value = 0;}
};
