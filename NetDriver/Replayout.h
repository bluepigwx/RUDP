#pragma once
#include <vector>

#include "NetType.h"
#include "../Engine/EngineTypeDef.h"
#include "../Core/Typedef.h"

class FRepChangelistState;
class FRepLayout;
class CObject;
class CProperty;
class ClassInfo;
class FRepChangedPropertyTracker;
class FReplicationChangelistMgr;

/*
 * FRepLayout有几项职责
 * 1.负责记录类的内存布局，便于进行比较
 */



// 对象属性变化对比结果返回枚举值
enum EReplayoutResult
{
    Success,
    Empty,
    Error,
    FatalError,
};


// 用于记录一次单独的历史变更记录
class FRepChangedHistory
{
public:
    std::vector<uint16> Changed;
};



// 对象上一帧的影子缓存，用来和当前实际属性做对比，得出这一帧实际变化的属性列表
class FRepStateStaticBuffer : public FNonCopyable
{
    friend FRepChangelistState;
    friend FRepLayout;
    
private:
    FRepStateStaticBuffer(const CObject* InObject) {}
    ~FRepStateStaticBuffer() {}
    
public:
    FRepStateStaticBuffer(FRepStateStaticBuffer&& Other)
    {
        Buffer = std::move(Other.Buffer);
    }
    
public:
    std::vector<uint8> Buffer;
};



// 记录一个对象实例整个生命期内所有属性的变更历史
// 使用一个环形队列来记录历史变更记录
// 每一帧的属性变更由对象当前属性和FRepStateStaticBuffer记录的影子属性对比得出
class FRepChangelistState : public FNonCopyable
{
    friend FReplicationChangelistMgr;
    
private:
    FRepChangelistState(const CObject* InObject, FRepLayout* Replayout) :
    HistoryStart(0),
    HistoryEnd(0),
    CompareIndex(0),
    StaticBuffer(InObject)
    {
        
    }
    
public:
    
    static const int32 MAX_CHANGED_HISTORY = 64;   // 环形队列最大长度，超过后合并最旧的记录
    
    // 环形队列的逻辑下标，可以一直自增，物理下标通过取模MAX_CHANGED_HISTORY获得
    int32 HistoryStart;
    int32 HistoryEnd;
    // 环形队列内存
    FRepChangedHistory ChangeHistory[MAX_CHANGED_HISTORY];
    
    // 属性比较的计数器，也能作为对象属性变更的版本号使用
    int32 CompareIndex;
    
    FRepStateStaticBuffer StaticBuffer;
};



// 对象属性变化记录器，每个对象一个全局共享的即可
// 将当前对象实际的属性变更列表存入RepChangelistState维护的环形队列中
class FReplicationChangelistMgr : public FNonCopyable
{
    friend FRepLayout;
    
private:
    
    FReplicationChangelistMgr(const CObject* InObject, FRepLayout* Replayout) :
    LastReplicationFrame(0),
    RepChangelistState(InObject, Replayout)
    {
        
    }
    
    FRepChangelistState RepChangelistState;
    
public:
    
    FRepChangelistState& GetRepChangelistState() { return RepChangelistState; }
    
    int32 LastReplicationFrame; // 上次比较的帧号，比较重复比较
};



// CObject类的详细内存布局的原子对象，展开为最基础的普通类型如int,float等
// 如果一级字段为普通类型，则FRepLayoutCmd与FRepParentCmd一一对应，否则
// 如果是复合类则是复合型展开为基础类型后的每个元素与之一一对应
class FRepLayoutCmd
{
public:
    FRepLayoutCmd(CProperty* InProperty):
    Property(InProperty),
    EndCmd(0),
    ElementSize(0),
    Offset(0),
    ShadowOffset(0),
    ParentIndex(0),
    RelativeHandle(0)
    {
        
    }
    
    CProperty* Property;

    unsigned short EndCmd;

    int ElementSize;
    
    int Offset;

    int ShadowOffset;

    int ParentIndex;

    unsigned short RelativeHandle;
};


// CObject类属性的一级字段，记录了这个类的一级字段布局
// 记录了这个字段的各类属性标签，一级字段可以是普通类型如int,float等，但也可以为struct等复合类型
// FRepParentCmd只存储到一级字段粒度的细节
class FRepParentCmd
{
public:
    FRepParentCmd(CProperty* InProperty, int InArrayIndex) :
    Property(InProperty),
    ArrayIndex(InArrayIndex),
    Offset(0),
    ShadowOffset(0),
    CmdStart(0),
    CmdEnd(0)
    {
    }
    
    CProperty* Property;    // 具体哪条属性

    int ArrayIndex;

    int Offset;

    int ShadowOffset;

    // 用于定位FRepLayoutCmd数组中的元素
    unsigned short CmdStart;

    unsigned short CmdEnd;
};



class FReceivingRepState
{
    
};



class FSendingRepState
{
    
};



// FRepState 是网络复制系统中每个对象在每个连接上的独立复制状态容器。它的核心职责是：
// 为单个对象在单个网络连接上维护发送和接收所需的所有状态信息
class FRepState
{
public:
    FRepState() {}

    FSendingRepState SendingRepState;

    FReceivingRepState ReceivingState;
};



class FRepLayout
{
public:
    FRepLayout() :
    ShadowBufferSize(0),
    Class(nullptr)
    {
        
    }
    
    // 通过类的类型信息构造出FReplayout布局信息
    bool InitFromClass(ClassInfo* InClass);

    bool InitChangedTracker(FRepChangedPropertyTracker* InTracker);

    // FRepState的创建过程需要用到FRepLayout的内部数据，因此创建的职责交给FRepLayout
    FRepState* CreateRepState(FRepChangedPropertyTracker* InTracker);
    
    // 创建指定对象的属性变更对比组件，一个对象一个
    FReplicationChangelistMgr* CreateReplicationChangelistMgr(const CObject* InObject);
    
    // 尝试对一个对象的FReplicationChangelistMgr进行属性变更更新
    EReplayoutResult UpdateChangelistMgr(
        const FRepLayout& Replayout,
        FSendingRepState* SendingState,
        FReplicationChangelistMgr& InChangelistMgr,
        const CObject* InObject,
        const int32 ReplicationFrame,
        const FReplicationFlags Flags,
        const bool bForceCompare
        );
    
    
    // 根据类型自身的内存布局对其实例InObject进行属性变更比较
    EReplayoutResult CompareProperty(
        FSendingRepState* RepState,
        FReplicationChangelistMgr* RepChangelistMgr,
        const CObject* InObject,
        const FReplicationFlags Flags
        );
    
    // 创建对象的影子缓存，用来和对象当前值进行属性变更比较
    FRepStateStaticBuffer CreateShadowBuffer(CObject* InObject);
    
    // 是否是个空字段的Layout
    bool IsEmpty();
    

private:
    void InitRepStaticBuffer(FRepStateStaticBuffer& StaticBuffer, CObject* InObject);
    void ConstructPropertys(FRepStateStaticBuffer& StaticBuffer);
    void CopyPropertys(FRepStateStaticBuffer& StaticBuffer, CObject* InObject);

private:
    // 一级字段描述类
    std::vector<FRepParentCmd> Parents;
    // 最终原子类型描述类
    std::vector<FRepLayoutCmd> Cmds;
    // 对象的大小描述
    int32 ShadowBufferSize;
    // 对象类型
    ClassInfo* Class;
};