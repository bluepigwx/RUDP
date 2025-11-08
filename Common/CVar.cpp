#include "CVar.h"
#include "Command.h"
#include "Log.h"


CVar* CVar::Find(const char* Name)
{
    CVarpool::iterator it = GetVarPool().find(Name);
    if (it == GetVarPool().end())
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



static void CVar_SetVar_f(CmdParam& Param)
{
    if (Param.size() != 2)
    {
        return;
    }

    float NewValue = (float)atof(Param[1].c_str());
    CVar::SetValue(Param[0].c_str(), NewValue);
    Log(LOG_CAT_LOG, "Set %s = %f", Param[0].c_str(), NewValue);
}


static void CVar_PrintVar_f(CmdParam& Param)
{
    CVarpool::const_iterator it = CVar::GetVarPool().begin();
    while (it != CVar::GetVarPool().end())
    {
        Log(LOG_CAT_LOG, "Cvar: %s value: %f", it->first.c_str(), it->second->GetValue());
        ++it;
    }
    
}


void CVar_Init()
{
    Cmd_Register("setvar", CVar_SetVar_f);
    Cmd_Register("lsvar", CVar_PrintVar_f);
}


