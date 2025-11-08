#pragma once
#include <functional>
#include <vector>
#include <string>


typedef std::vector<std::string> CmdParam;
typedef std::function<void(CmdParam&)> CmdFuc;


int Cmd_Init();

int Cmd_Register(std::string Cmd, CmdFuc Func);
CmdFuc Cmd_FindFunc(std::string Cmd);
