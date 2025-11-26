#pragma once
#include <vector>

#include "../Core/Typedef.h"


// 用于记录一个顶层属性的复制条件是否发生改变
class FRepChangedParent
{
public:
    FRepChangedParent():
    Active(1),
    OldActive(1),
    IsConditional(0)
    {
        
    }
    
    bool Active : 1;
    bool OldActive : 1;
    int32 IsConditional : 1;
};


// 在所有链接间共享属性复制元数据
// 跟踪属性的激活状态（Active State）
// 管理条件复制（Conditional Replication)
class FRepChangedPropertyTracker
{
public:
    FRepChangedPropertyTracker() {}
    // 顶层属性的复制条件跟踪
    std::vector<FRepChangedParent> Parent;
};
