#include "Replayout.h"

#include <assert.h>

#include "RepChangedPropertyTracker.h"
#include "../Core/ClassInfo.h"



struct FSharedInitProperty
{
    FRepParentCmd& Parent;
    std::vector<FRepLayoutCmd>& Cmds;
    int ParentHandle;
};


struct FStackInitProperty
{
    CProperty* Property;

    int Offset;
};



static int InitProperty(FSharedInitProperty& Shared, FStackInitProperty Stack);


static int AddParentProperty(std::vector<FRepParentCmd>& InParents, CProperty* InProperty, int InIndex)
{
    InParents.emplace_back(InProperty, InIndex);
    return (int)InParents.size() - 1;
}



// 将复合类型递归展开
static int InitStructProperty(FSharedInitProperty& Shared, FStackInitProperty OuterStack)
{
    assert(OuterStack.Property->TypeClassInfo);
    assert(OuterStack.Property->Type == PropertyType_class);
    
    ClassInfo* StructClassInfo = OuterStack.Property->TypeClassInfo;
    
    StructClassInfo->BuildupReplicationProps();

    // 注意BuildupReplicationProps构造出来关于属性的偏移量是相对偏移
    // 下面需要进行累加得到在属性在实际的复合结构体中的绝对偏移量
    for (auto it=StructClassInfo->ClassReps.begin(); it!=StructClassInfo->ClassReps.end(); ++it)
    {
        FStackInitProperty InnerStack
        {
            /*Property=*/it->Property,
            /*Offset=*/it->Property->Offset + OuterStack.Offset,
        };
        
        InitProperty(Shared, InnerStack);
    }
    
    return 0;
}



// 递归展开，直到基本类型为止
static int InitProperty(FSharedInitProperty& Shared, FStackInitProperty Stack)
{
    if (Stack.Property->Type == PropertyType_class)
    {
        // 复合类型
        InitStructProperty(Shared, Stack);
    }
    else
    {
        // 普通类型
        assert(Stack.Property->Type < PropertyType_End);
        assert(Stack.Property->TypeClassInfo == nullptr);
        
        Shared.Cmds.emplace_back(FRepLayoutCmd(Stack.Property));
        FRepLayoutCmd& Cmd = Shared.Cmds[Shared.Cmds.size() - 1];

        Cmd.Offset = Stack.Offset;
        Cmd.ElementSize = Stack.Property->Size;
        Cmd.ParentIndex = Shared.ParentHandle;
    }
    return 0;
}


static void BuildShadowOffset(std::vector<FRepParentCmd>& ParentCmd, std::vector<FRepLayoutCmd>& Cmds)
{
    for (auto it=ParentCmd.begin(); it !=ParentCmd.end(); ++it)
    {
        (*it).ShadowOffset = (*it).Offset;
    }

    for (auto cit=Cmds.begin(); cit!=Cmds.end(); ++cit)
    {
        (*cit).ShadowOffset = (*cit).Offset;
    }
}


bool FRepLayout::InitFromClass(ClassInfo* InClass)
{
    // 先生成网络复制字段的列表
    InClass->BuildupReplicationProps();

    for (size_t i=0; i<InClass->ClassReps.size(); ++i)
    {
        // 创建一级字段描述类
        CProperty* Property = InClass->ClassReps[i].Property;
        int ArrayIndex = InClass->ClassReps[i].Index;

        int ParentHandle = AddParentProperty(Parents, Property, ArrayIndex);
        assert(ParentHandle == i);
        
        int ParentOffset = Property->Offset + Property->Size * ArrayIndex;

        // 递归调用过程的全局变量
        FSharedInitProperty SharedParams
        {
            /*Parent=*/Parents[ParentHandle],
            /*Cmds=*/Cmds,
            /*ParentHandle=*/ParentHandle,
        };

        // 递归调用过程中的栈变量
        FStackInitProperty StackParams
        {
            /*Property=*/Property,
            /*Offset=*/ParentOffset,
        };
        
        // 创建二级字段，如果一级字段是复合类型则进行递归展开
        Parents[ParentHandle].CmdStart = (int)Cmds.size();
        InitProperty(SharedParams, StackParams);
        Parents[ParentHandle].CmdEnd = (int)Cmds.size();
    }

    BuildShadowOffset(Parents, Cmds);
    ShadowBufferSize = InClass->Size;
    Class = InClass;

    return true;
}


bool FRepLayout::InitChangedTracker(FRepChangedPropertyTracker* InTracker)
{
    InTracker->Parent.resize(Parents.size());

    auto it = InTracker->Parent.begin();
    for (; it!=InTracker->Parent.end(); ++it)
    {
        (*it).IsConditional = false;
    }

    return true;
}


FRepState* FRepLayout::CreateRepState(FRepChangedPropertyTracker* InTracker)
{
    FRepState* NewState = new FRepState();
    return NewState;
}


