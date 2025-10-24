#pragma once

#include <map>

// 配置变量

class CVar;
typedef std::map<char*, CVar*> CVarpool;

class CVar
{
public:
    CVar(const char* Name, float* Value) :
    VarValue(Value)
    {
        VarPool.emplace((char*)Name, this);
    }
    
    static CVar* Find(const char* Name);

    static CVar* SetValue(const char* Name, float NewValue);

private:
    float* VarValue;

    static CVarpool VarPool;
};


void CVar_Init();