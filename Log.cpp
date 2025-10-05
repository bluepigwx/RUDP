#include "Log.h"

#include <cstdarg>
#include <cstdio>
#include <iostream>


void Log(LOG_CAT, const char* fmt, ...)
{
#define MAXPRINTMSG 1024

    va_list argptr;
    static char msg[MAXPRINTMSG];

    va_start (argptr,fmt);
    vsprintf_s (msg,fmt,argptr);
    va_end (argptr);

    std::cout << msg << std::endl;
}
