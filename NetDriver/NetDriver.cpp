#include "NetDriver.h"

#include <assert.h>

#include "Channel.h"
#include "../Log.h"
#include "../Engine/GlobalNames.h"


IMPLAMENT_CLASS_BEGIN(CNetDriver)
IMPLAMENT_CLASS_END(CNetDriver, CObject)




int CNetDriver::Init()
{
    // 构建Channel动态创建表
    ChannelConfig.clear();

    ChannelConfig[NAME_Actor] = FChannelDefine
    {
        CActorChannel::StaticClass(),
        NAME_Actor,
    };

    return true;
}



CChannel* CNetDriver::CreateChannel(std::string& Name)
{
    CChannel* Ret = nullptr;
    
    ChannelQueue& Queue = ClassChannelPool[Name];
    if (!Queue.empty())
    {
        Ret = Queue.front();
        assert(Ret->InUse == false);
        
        Ret->InUse = true;
        Queue.pop();

        return Ret;
    }

    if (Ret == nullptr)
    {
        ChannelDefine::const_iterator it = ChannelConfig.find(Name);
        if (it == ChannelConfig.end())
        {
            Log(LOG_CAT_ERR, "CNetDriver::CreateChannel Invalide Channel class name %s", Name.c_str());
            return nullptr;
        }

        const FChannelDefine& Chdef = it->second;
        if (Chdef.Class->IsAClass(CChannel::StaticClass()))
        {
            Ret = dynamic_cast<CChannel*>(Chdef.Class->CreateFuncPtr());
            Ret->Name = Name;
            Ret->InUse = true;
        }
        else
        {
            Log(LOG_CAT_ERR, "Invalid Channel class");
        }
    }

    return Ret;
}


void CNetDriver::DestroyChannel(CChannel* InCh)
{
    if (nullptr == InCh)
    {
        return;
    }

    std::map<std::string, ChannelQueue>::iterator it = ClassChannelPool.find(InCh->Name);
    if (it == ClassChannelPool.end())
    {
        Log(LOG_CAT_ERR, "invalid ch class name %s", InCh->Name.c_str());
        // 未知类型不进入池子
        delete InCh;
        return;
    }

    ChannelQueue& Queue = it->second;
    InCh->InUse = false;
    Queue.push(InCh);
}

