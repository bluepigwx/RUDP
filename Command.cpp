#include "Command.h"

#include <map>
#include <iostream>


std::map<std::string, CmdFuc> FuncMap;


int Cmd_Register(std::string Cmd, CmdFuc Func)
{
    auto it = FuncMap.find(Cmd);
    if (it != FuncMap.end())
    {
        std::cout << "duplicat cmd " << Cmd << " register" << std::endl;
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


