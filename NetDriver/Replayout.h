#pragma once

class CProperty;

/*
 * FRepLayout有几项职责
 * 1.负责记录类的内存布局，便于进行比较
 */

// CObject类的一级字段的具体描述
class FRepLayoutCmd
{
public:
    CProperty* Property;

    unsigned short EndCmd;

    int ElementSize;
    
    int Offset;

    int ShadowOffset;

    int ParentIndex;

    unsigned short RelativeHandle;
};


// CObject类属性的一级字段，记录了这个类的一级字段布局
// 记录了这个字段的各类属性标签
class FRepParentCmd
{
public:
    CProperty* Property;    // 具体哪条属性

    int ArrayIndex;

    int Offset;

    int ShadowOffset;

    // 用于定位FRepLayoutCmd数组中的元素
    unsigned short CmdStart;

    unsigned short CmdEnd;
};


class FRepLayout
{
    
};