#pragma once
#include <vector>


// 类运行时基本辅助信息


enum PropertyType
{
    PropertyTYpe_bool,
    PropertyType_char,
    PropertyType_short,
    PropertyType_int,
    PropertyType_long,
    PropertyType_int64,
    PropertyType_float,
    PropertyType_double,
};


enum PropertyFlag : int
{
    PropertyFlag_None = 0,
    PropertyFlag_Net = 1 << 0,  // 用于网络复制的属性
};


// 字段信息描述
// 一个字段可以是一个原生类型如int, float
// 字段也可以是一个固定数组如int [5]，如果是数字字段，则可以通过ArrayDim来区分
class CProperty
{
public:
    CProperty(int InOffset, int InSize, const char* InName, PropertyType InType, PropertyFlag InFlag) :
    Offset(InOffset),
    Size(InSize),
    Name(InName),
    Type(InType),
    Flag(InFlag),
    ArrayDim(0),
    RepIndex(0)
    {
        
    }
    
public:
    int Offset;
    int Size;
    const char* Name;
    PropertyType Type;
    PropertyFlag Flag;
    
    // 指明这个属性字段带有有多少个连续的元素，
    // 如果是单一字段，那么就是1，int a 等价于 int c[1]
    // 否则大于1的情况：int c[3] -> ArrayDim = 3
    int ArrayDim;
    // 指明这个属性在所有属性展开列表中的序号，如果是数组字段那么就是首个元素的Index位置，
    // 其余元素结合ArrayDim字段进行数组字段中每个元素的访问
    int RepIndex;
};


// 用于表示所有元素在内存中实际展开以后的布局情况
// 便于属性比较的时候快速找到要对比的属性
// int a[3] 展开后实际记录为三个元素：
// int a0 FRepRecord
// int a1 FRepRecord
// int a2 FRepRecord
// 无论是结构体类型还是数组类型，都可以通过直接的偏移量来寻址要找的字段，哪怕是嵌套的字段
struct FRepRecord
{
    // 对于展开后的FRepRecord记录，可能会多个Record中的Property指针都是同一个数组字段的Property
    // 但是Record的Index会不同来定位数组中的具体元素
    CProperty* Property;
    //如果是数组字段，用于定位具体的数组元素，否则Index为0
    //定位逻辑：字段首偏移量 + 字段size * Index = 要寻址的字段偏移量 
    int Index;

    FRepRecord(CProperty* InProperty, int InIndex):
    Property(InProperty),
    Index(InIndex)
    {
        
    }
};


enum ClassInfoFlag : int
{
    ClassInfoFlag_None = 0,
    ClassInfoFlag_InitReplicationData = 1 << 0, // 已经完成网络复制属性相关初始化
};


class CObject;
class ClassInfo;

typedef CObject* (*CreateObjectFunc)();
typedef void (*InitClassProperty)(ClassInfo* InClass);


// 一个基于CObject类的类型信息
class ClassInfo
{
public:
    ClassInfo() :
    BaseClass(nullptr),
    Next(nullptr),
    Name(nullptr),
    Size(0),
    CreateFuncPtr(nullptr),
    ClassFlag(ClassInfoFlag_None)
    {
        
    }

    ClassInfo* BaseClass;   //基类类型指针

    // 遍历用类型库链表，每次插入到头部
    static ClassInfo* First;
    ClassInfo* Next;
    
    const char* Name;
    int Size;

    // 类型的默认工厂方法
    CreateObjectFunc CreateFuncPtr;

    // 类型的字段属性
    std::vector<CProperty*> Properties;
    // 记录用于进行属性网络复制的字段
    std::vector<FRepRecord> ClassReps;

    int ClassFlag;

public:
    inline bool HasAnyFlags(int Flags) const
    {
        return ClassFlag & Flags;
    }

    int GetOffset(const char* FieldName) const;
    
    // 构建专用于网络复制的属性列表，就是将Property数组展开为FRepRecord数组
    void BuildupReplicationProps();
};



// 用于初始化ClassInfo的辅助结构，便于明确给指定的字段赋值
struct DeferInitClassInfo
{
    DeferInitClassInfo(ClassInfo* ThisClass, ClassInfo* Base, const char* Name, int Size, CreateObjectFunc CreateFunc, InitClassProperty InitPropertyPtr)
    {
        ThisClass->BaseClass = Base;
        ThisClass->Name = Name;
        ThisClass->Size = Size;
        ThisClass->CreateFuncPtr = CreateFunc;

        ThisClass->Next = ClassInfo::First;
        ClassInfo::First = ThisClass;

        // 完成反射类型的属性构建
        InitPropertyPtr(ThisClass);
    }
};


#define GET_CLASSINFO(ThisClass) \
    (&ThisClass::ThisClass##ClassInfo)


// 运行时类型获取接口定义
#define DECLEAR_CLASS(ThisClass) \
    public: \
    static ClassInfo ThisClass##ClassInfo; \
    static ClassInfo* StaticClass() \
    { \
        return &ThisClass##ClassInfo; \
    } \
    virtual ClassInfo* GetClassInfo() const override \
    {   \
        return ThisClass::StaticClass(); \
    } \
    static CObject* CreateObject() \
    { \
        return new ThisClass; \
    }


// 运行时类型构建
#define IMPLAMENT_CLASS_BEGIN(ThisClass, BaseClass) \
    static void DefferInit_##ThisClass##ClassInfo(ClassInfo* InClass) \
    {

// 普通类型
#define PROPERTY_DEFINE(ThisClass, FieldName, FieldType, FieldFlag) \
        CProperty* FieldName##FieldType = new CProperty(offsetof(ThisClass, FieldName), sizeof(ThisClass::FieldName), #FieldName, PropertyType_##FieldType, FieldFlag);\
        FieldName##FieldType->ArrayDim = 1; \
        InClass->Properties.push_back(FieldName##FieldType);

// 固定长度数组类型
#define PROPERTY_ARRAY_DEFINE(ThisClass, FieldName, ElementType, FieldFlag, ArraySize) \
        CProperty* FieldName##FieldType = new CProperty(offsetof(ThisClass, FieldName), sizeof(ThisClass::FieldName), #FieldName, PropertyType_##ElementType, FieldFlag);\
        FieldName##FieldType->ArrayDim = ArraySize; \
        InClass->Properties.push_back(FieldName##FieldType);

#define IMPLAMENT_CLASS_END(ThisClass, BaseClass) \
    } \
    ClassInfo ThisClass::ThisClass##ClassInfo; \
    static DeferInitClassInfo ThisClass##DeferInitClassInfo(&ThisClass::ThisClass##ClassInfo, GET_CLASSINFO(BaseClass), #ThisClass, sizeof(ThisClass), ThisClass::CreateObject, DefferInit_##ThisClass##ClassInfo);



// 清理ClassInfo里的堆内存
void ClassInfo_Cleanup(ClassInfo* InClass);