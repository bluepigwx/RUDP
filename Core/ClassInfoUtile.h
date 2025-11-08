#pragma once

#include "CObject.h"


// 通过字段名字获取字段值
template<class T>
bool ClassInfo_GetValue(CObject* InObject, const char* FieldName, T& OutValue)
{
    ClassInfo* InClass = InObject->GetClassInfo();
    int Offset = InClass->GetOffset(FieldName);
    if (Offset < 0)
    {
        return false;
    }

    OutValue = (T)*((char*)InObject + Offset);
    return true;
}