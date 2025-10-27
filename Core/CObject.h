#pragma once

#include "ClassInfo.h"

// 对象模型定义，只能单继承，如果要多继承请使用接口


class CObject
{
public:
    CObject() = default;
    virtual ~CObject() = default;

    static ClassInfo CObjectClassInfo;
    virtual ClassInfo* GetClassInfo() const
    {
        return  &CObject::CObjectClassInfo;
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


