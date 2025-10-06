#pragma once

#include <cstring>
#include <string>

// 方便操作基本数据类型打包/解包的工具类

class BufferBasic
{
protected:
    enum state { _good=0, _error=1 };
    
    BufferBasic() : _state(_error), _buffer(nullptr), _max_size(0), _pos(0) {}

public:
    BufferBasic(char* data, int max_size) : _state(_good), _buffer(data), _max_size(max_size), _pos(0) {}

    char* Get()
    {
        return _buffer;
    }

    void Clear()
    {
        _pos = 0;
    }

    int GetMaXSize()
    {
        return _max_size;
    }

    char* Current()
    {
        return _buffer + _pos;
    }

    bool IsValid()
    {
        return _state == _good;
    }

protected:
    void SetState(state NewState)
    {
        _state = NewState;
    }
    
    state _state;
    char* _buffer;
    int _max_size;
    int _pos;
};


// 数据输入类 IBuffer << data
class IBuffer : public BufferBasic
{
protected:
    IBuffer() {}

public:
    IBuffer(char* data, int size) : BufferBasic(data, size) {}

    int Size() const
    {
        return _pos;
    }

    void SetSize(int size)
    {
        if (size + _pos > _max_size)
        {
            SetState(_error);
            return;
        }

        _pos = size;
    }
    
    template<class T>
    IBuffer& operator << (T value)
    {
        return Serialize(&value, sizeof(T));
    }
    
    // C++字符串特化处理
    IBuffer& operator << (std::string& value)
    {
        return Serialize((char*)(value.c_str()), value.size() + 1);
    }
    
    IBuffer& Serialize(void* data, int size)
    {
        if (!data || size == 0 || size > _max_size)
        {
            return *this;
        }

        if (_pos + size > _max_size)
        {
            SetState(_error);
        }
        else
        {
            memcpy((char*)_buffer + _pos, data, size);
            _pos += size;
        }

        return *this;
    }
};


// 自带缓冲区的IBuffer
template<size_t MaxBytes=256>
class FixIBuffer : public IBuffer
{
public:
    FixIBuffer()
    {
        _buffer = _data;
        _max_size = BufferSize;
        _pos = 0;
        _state = BufferBasic::_good;
    }

protected:
    enum  { BufferSize = MaxBytes };
    char _data[BufferSize];
};


typedef FixIBuffer<16> IBuffer16;
typedef FixIBuffer<32> IBuffer32;
typedef FixIBuffer<64> IBuffer64;
typedef FixIBuffer<128> IBuffer128;
typedef FixIBuffer<256> IBuffer256;
typedef FixIBuffer<512> IBuffer512;
typedef FixIBuffer<1024> IBuffer1k;
typedef FixIBuffer<2048> IBuffer2k;
typedef FixIBuffer<4096> IBuffer4k;
typedef FixIBuffer<8192> IBuffer8k;
typedef FixIBuffer<16384> IBuffer16k;
typedef FixIBuffer<32768> IBuffer32k;
typedef FixIBuffer<65536> IBuffer64k;


// 数据输出类 OBuffer >> data
class OBuffer : public BufferBasic
{
protected:
    OBuffer() {}
    
public:
    OBuffer(char* data, int size) : BufferBasic(data, size) {}

    int Size() const
    {
        return _max_size - _pos;
    }

    //C++字符串特化处理
    OBuffer& operator >> (std::string& value)
    {
        const char* str = _buffer + _pos;
        value = str;

        _pos += value.size() + 1;
        if (_pos > _max_size)
        {
            SetState(_error);
        }

        return *this;
    }

    template<typename T>
    OBuffer& operator >> (T& value)
    {
        if (_pos + sizeof(T) < _max_size)
        {
            value = *(T*)(_buffer + _pos);
            _pos += sizeof(T);
        }
        else
        {
            SetState(_error);
        }
        
        return *this;
    }

    OBuffer& Serialize(char* data, int size)
    {
        if (data == nullptr || size <= 0)
        {
            // 等于没用过
            return *this;
        }

        if (_pos + size < _max_size)
        {
            memcpy(data, _buffer + _pos, size);
            _pos += size;
        }
        else
        {
            SetState(_error);
        }

        return *this;
    }
};