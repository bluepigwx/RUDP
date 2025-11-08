#pragma once

#include <map>
#include <string>

// 配置变量

class CVar;
typedef std::map<std::string, CVar*> CVarpool;

class CVar
{
public:
    CVar(const char* Name, float* Value) :
    VarValue(Value)
    {
        GetVarPool().emplace(Name, this);
    }
    
    static CVar* Find(const char* Name);

    static CVar* SetValue(const char* Name, float NewValue);

    float GetValue() { return *VarValue; }

    // 使用函数内静态变量解决静态初始化顺序问题
    static CVarpool& GetVarPool()
    {
        static CVarpool VarPool;
        return VarPool;
    }

private:
    float* VarValue;
    
};


void CVar_Init();