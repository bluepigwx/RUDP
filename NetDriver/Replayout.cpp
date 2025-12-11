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


bool FRepLayout::IsEmpty()
{
    return Parents.empty();
}


FRepStateStaticBuffer FRepLayout::CreateShadowBuffer(CObject* InObject)
{
    assert(InObject);
    
    FRepStateStaticBuffer ShadowBuffer(InObject);
    
    InitRepStaticBuffer(ShadowBuffer, InObject);
    
    return ShadowBuffer;
}


void FRepLayout::InitRepStaticBuffer(FRepStateStaticBuffer& StaticBuffer, CObject* InObject)
{
    StaticBuffer.Buffer.resize(ShadowBufferSize);
    
    // 清理
    ConstructPropertys(StaticBuffer);
    // 拷贝
    CopyPropertys(StaticBuffer, InObject);
}


void FRepLayout::ConstructPropertys(FRepStateStaticBuffer& StaticBuffer)
{
    uint8* Data = StaticBuffer.Buffer.data();
    
    std::vector<FRepParentCmd>::const_iterator cit = Parents.begin();
    for (; cit!= Parents.end(); ++cit)
    {
        const FRepParentCmd& Parent = *cit;
        
        if (Parent.ArrayIndex == 0)
        {
            assert(Parent.Offset + Parent.Property->GetSize() <= StaticBuffer.Buffer.size());
            Parent.Property->InitializeValue(Data + Parent.Offset);
        }
    }
}


void FRepLayout::CopyPropertys(FRepStateStaticBuffer& StaticBuffer, CObject* InObject)
{
    uint8* Data = StaticBuffer.Buffer.data();
    
    std::vector<FRepParentCmd>::const_iterator cit = Parents.begin();
    for (; cit!=Parents.end(); ++cit)
    {
        const FRepParentCmd& Parent = *cit;
        
        if (Parent.ArrayIndex == 0)
        {
            Parent.Property->CopyCompleteValue(Data, (uint8*)InObject);
        }
    }
}


FRepState* FRepLayout::CreateRepState(FRepChangedPropertyTracker* InTracker)
{
    FRepState* NewState = new FRepState();
    return NewState;
}


FReplicationChangelistMgr* FRepLayout::CreateReplicationChangelistMgr(const CObject* InObject)
{
    assert(InObject);
    
    FReplicationChangelistMgr* NewReplicationChangelistMgr = new FReplicationChangelistMgr(InObject, this);
    return NewReplicationChangelistMgr;
}


EReplayoutResult FRepLayout::UpdateChangelistMgr(
    const FRepLayout& Replayout, 
    FSendingRepState* SendingState, 
    FReplicationChangelistMgr& InChangelistMgr, 
    const CObject* InObject, 
    const int32 ReplicationFrame, 
    const FReplicationFlags Flags, 
    const bool bForceCompare)
{
    EReplayoutResult Result = Success;
    
    if (!bForceCompare && InChangelistMgr.LastReplicationFrame == ReplicationFrame)
    {
        return Result;
    }
    
    
    return CompareProperty(SendingState, &InChangelistMgr, InObject, Flags);
}



struct FComparePropertySharedParams
{
    const std::vector<FRepParentCmd>& Parents;
    const std::vector<FRepLayoutCmd>& Cmds;
    
    FSendingRepState* const RepState;
    FRepChangelistState* const Changelist;
};


struct FComparePropertyStackParams
{
    const CObject* Object;
    
};



EReplayoutResult FRepLayout::CompareProperty(
    FSendingRepState* SendingState, 
    FReplicationChangelistMgr* RepChangelistMgr, 
    const CObject* InObject, 
    const FReplicationFlags Flags)
{
    if (IsEmpty())
    {
        return EReplayoutResult::Empty;
    }
    
    FRepChangelistState& RepChangelistState = RepChangelistMgr->RepChangelistState;
    RepChangelistState.CompareIndex++;
    
    int32 LeftSize = RepChangelistState.HistoryEnd - RepChangelistState.HistoryStart;
    assert(LeftSize < FRepChangelistState::MAX_CHANGED_HISTORY);
    
    FRepChangedHistory& NewHistory = RepChangelistState.ChangeHistory[RepChangelistState.HistoryEnd];
    std::vector<uint16>& Changed = NewHistory.Changed;
    Changed.clear();
    
    // 开始进行比较与更新
    
    
    if (Changed.size() == 0)
    {
        return EReplayoutResult::Empty;
    }
    
    // 实际使用了才进行内存分配
    if (NewHistory.Changed.size() > 0)
    {
        RepChangelistState.HistoryEnd++;
    }
    
    LeftSize = RepChangelistState.HistoryEnd - RepChangelistState.HistoryStart;
    if (LeftSize == FRepChangelistState::MAX_CHANGED_HISTORY)
    {
        // 缓存队列已经用完，进行存量合并
    }
    
    // 最后回绕一下免得溢出
    RepChangelistState.HistoryEnd = RepChangelistState.HistoryEnd % FRepChangelistState::MAX_CHANGED_HISTORY;
    RepChangelistState.HistoryStart = RepChangelistState.HistoryStart % FRepChangelistState::MAX_CHANGED_HISTORY;
    
    return EReplayoutResult::Success;
}

