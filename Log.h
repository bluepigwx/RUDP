#pragma once


enum LOG_CAT
{
    LOG_CAT_LOG,
    LOG_CAT_ERR,
};


void Log(LOG_CAT, const char* fmt, ...);