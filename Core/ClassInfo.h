#pragma once


// 类运行时基本辅助信息


class CObject;
typedef CObject* (*CreateObjectFunc)(); 


class ClassInfo
{
public:
    ClassInfo() :
    BaseClass(nullptr),
    Next(nullptr),
    Name(nullptr),
    Size(0)
    {}

    ClassInfo* BaseClass;   //基类类型指针

    // 遍历用类型库链表，每次插入到头部
    static ClassInfo* First;
    ClassInfo* Next;
    
    const char* Name;
    int Size;

    CreateObjectFunc CreateFuncPtr;
};



// 用于初始化ClassInfo的辅助结构，便于明确给指定的字段赋值
struct DeferInitClassInfo
{
    DeferInitClassInfo(ClassInfo* ThisClass, ClassInfo* Base, const char* Name, int Size, CreateObjectFunc CreateFunc)
    {
        ThisClass->BaseClass = Base;
        ThisClass->Name = Name;
        ThisClass->Size = Size;
        ThisClass->CreateFuncPtr = CreateFunc;

        ThisClass->Next = ClassInfo::First;
        ClassInfo::First = ThisClass;
    }
};


#define GET_CLASSINFO(ThisClass) \
    (&ThisClass::ThisClass##ClassInfo)


// 运行时类型获取接口定义
#define DECLEAR_CLASS(ThisClass) \
    public: \
    static ClassInfo ThisClass##ClassInfo; \
    virtual ClassInfo* GetClassInfo() const override \
    {   \
        return &ThisClass##ClassInfo; \
    } \
    static CObject* CreateObject() \
    { \
        return new ThisClass; \
    }


// 初始化运行时类型
#define IMPLAMENT_CLASS(ThisClass, BaseClass) \
    ClassInfo ThisClass::ThisClass##ClassInfo; \
    static DeferInitClassInfo ThisClass##DeferInitClassInfo(&ThisClass::ThisClass##ClassInfo, GET_CLASSINFO(BaseClass), #ThisClass, sizeof(ThisClass), ThisClass::CreateObject);


