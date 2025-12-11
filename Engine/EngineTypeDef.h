#pragma once


// 禁止拷贝对象由此继承
class FNonCopyable
{
protected:
    FNonCopyable() {}
    ~FNonCopyable() {}
    
private:
    FNonCopyable(FNonCopyable&) {}
    FNonCopyable& operator==(FNonCopyable&);
};


// 无效索引
constexpr int32 INDEX_NONE = -1;