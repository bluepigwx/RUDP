#include "CObject.h"
#include "../Log.h"
#include "../Command.h"

ClassInfo CObject::CObjectClassInfo;
DeferInitClassInfo CObjectDeferInitClassInfo(&CObject::CObjectClassInfo, nullptr, "CObject", sizeof(CObject), CObject::CreateObject);


bool CObject::IsA(ClassInfo* Class) const
{
    if (Class == nullptr)
    {
        return false;
    }

    ClassInfo* Tmp = Class;
    while (Tmp)
    {
        if (this->GetClassInfo() == Tmp)
        {
            return true;
        }
        // else
        Tmp = Tmp->BaseClass;
    }

    return false;
}


static void Object_PrintAll_f(CmdParam& Param)
{
    ClassInfo* tmp = CObject::CObjectClassInfo.First;
    while (tmp)
    {
        Log(LOG_CAT_LOG, "%s Size: %d Base: %s", tmp->Name, tmp->Size, tmp->BaseClass ? tmp->BaseClass->Name : "");
        tmp = tmp->BaseClass;
    }
}


void Object_Init()
{
    Cmd_Register("lsclass", Object_PrintAll_f);
}

