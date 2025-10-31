#pragma once

#include "ClassInfo.h"
// 对象模型定义，只能单继承，如果要多继承请使用接口


class CObject
{
public:
    CObject() = default;
    virtual ~CObject() = default;

    static ClassInfo CObjectClassInfo;
    // 函数内静态变量解决static变量初始化顺序问题，因为Property变量访问ClassInfo之前需要先构建好ClassInfo变量
    static ClassInfo* StaticClass()
    {
        return &CObjectClassInfo;
    }
    
    virtual ClassInfo* GetClassInfo() const
    {
        return  CObject::StaticClass();
    }

    static CObject* CreateObject()
    {
        return new CObject();
    }
    
    bool IsA(ClassInfo* Class) const;
    
};


template<class T>
T* NewObject()
{
    return (T::CreateObject);
}




void Object_Init();

void Object_Finish();


