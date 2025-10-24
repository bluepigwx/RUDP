#include "CVar.h"
#include "Command.h"

CVarpool CVar::VarPool;


CVar* CVar::Find(const char* Name)
{
    CVarpool::iterator it = VarPool.find((char*)Name);
    if (it == VarPool.end())
    {
        return nullptr;
    }

    return it->second;
}


CVar* CVar::SetValue(const char* Name, float NewValue)
{
    CVar* var = Find(Name);
    if (!var)
    {
        return nullptr;
    }

    *var->VarValue = NewValue;

    return var;
}


static void CVar_SetVar(CmdParam& Param)
{
    if (Param.size() != 2)
    {
        return;
    }

    float NewValue = (float)atof(Param[1].c_str());
    CVar::SetValue(Param[0].c_str(), NewValue);
}


void CVar_Init()
{
    Cmd_Register("set", CVar_SetVar);
}


