#pragma once
#include <vector>

class CProperty;
class ClassInfo;

/*
 * FRepLayout有几项职责
 * 1.负责记录类的内存布局，便于进行比较
 */


// CObject类的详细内存布局的原子对象，展开为最基础的普通类型如int,float等
// 如果一级字段为普通类型，则FRepLayoutCmd与FRepParentCmd一一对应，否则
// 如果是复合类则是复合型展开为基础类型后的每个元素与之一一对应
class FRepLayoutCmd
{
public:
    FRepLayoutCmd(CProperty* InProperty):
    Property(InProperty),
    EndCmd(0),
    ElementSize(0),
    Offset(0),
    ShadowOffset(0),
    ParentIndex(0),
    RelativeHandle(0)
    {
        
    }
    
    CProperty* Property;

    unsigned short EndCmd;

    int ElementSize;
    
    int Offset;

    int ShadowOffset;

    int ParentIndex;

    unsigned short RelativeHandle;
};


// CObject类属性的一级字段，记录了这个类的一级字段布局
// 记录了这个字段的各类属性标签，一级字段可以是普通类型如int,float等，但也可以为struct等复合类型
// FRepParentCmd只存储到一级字段粒度的细节
class FRepParentCmd
{
public:
    FRepParentCmd(CProperty* InProperty, int InArrayIndex) :
    Property(InProperty),
    ArrayIndex(InArrayIndex),
    Offset(0),
    ShadowOffset(0),
    CmdStart(0),
    CmdEnd(0)
    {
    }
    
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
public:
    bool InitFromClass(ClassInfo* InClass);

private:
    // 一级字段描述类
    std::vector<FRepParentCmd> Parents;
    // 最终原子类型描述类
    std::vector<FRepLayoutCmd> Cmds;
};