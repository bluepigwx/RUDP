#pragma once


struct FGenericPlatformTypes
{
    //8 bit unsigned integer
    typedef unsigned char uint8;

    //8 bit signed integer
    typedef signed char int8;

    typedef unsigned short uint16;

    typedef signed short int16;

    typedef unsigned int uint32;

    typedef signed int int32;

    typedef unsigned long long uint64;

    typedef signed long long int64;
};


// 各个平台相关的基本类型定义
#if defined(_WIN64) || defined(_WIN32)

struct FWindowsPlatformTypes : public FGenericPlatformTypes
{
    
};
typedef FWindowsPlatformTypes FPlatformTypes;

#endif


#if defined(_LINUX64) || defined(_LINUX32)

struct FLinuxPlatformTypes : public FGenericPlatformTypes
{
    
};
typedef FLinuxPlatformTypes FPlatformTypes;

#endif

