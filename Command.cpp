#include "Command.h"

#include <map>
#include <iostream>

#include "Log.h"


std::map<std::string, CmdFuc> FuncMap;


static void Cmd_LsCmd_f(CmdParam& Param)
{
    std::map<std::string, CmdFuc>::const_iterator it = FuncMap.begin();
    for (; it != FuncMap.end(); ++it)
    {
        Log(LOG_CAT_LOG, it->first.c_str());
    }
}


int Cmd_Init()
{
    Cmd_Register("lscmd", Cmd_LsCmd_f);
    return 0;
}


int Cmd_Register(std::string Cmd, CmdFuc Func)
{
    auto it = FuncMap.find(Cmd);
    if (it != FuncMap.end())
    {
        Log(LOG_CAT_ERR, "duplicat cmd [%s] try register", Cmd.c_str());
        return -1;
    }

    FuncMap.emplace(Cmd, Func);
    return 0;
}


CmdFuc Cmd_FindFunc(std::string Cmd)
{
    auto it = FuncMap.find(Cmd);
    if (it == FuncMap.end())
    {
        return nullptr;
    }

    return it->second;
}


