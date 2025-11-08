#include "NetworkObjectList.h"
#include "../Common/Log.h"

FNetworkObjectInfo* FNetworkObjectList::FindOrAdd(CActor* InActor)
{
    if (nullptr == InActor)
    {
        return nullptr;
    }

    std::map<CActor*, FNetworkObjectInfo*>::iterator it = AllObjectList.find(InActor);
    if (it == AllObjectList.end())
    {
        FNetworkObjectInfo* ObjInfo = new FNetworkObjectInfo(InActor);
        AllObjectList.emplace(InActor, ObjInfo);
        return ObjInfo;
    }

    return it->second;
}


bool FNetworkObjectList::Remove(CActor* InActor)
{
    if (nullptr == InActor)
    {
        Log(LOG_CAT_ERR, "FNetworkObjectList::Remove  null obj");
        return false;
    }

    std::map<CActor*, FNetworkObjectInfo*>::iterator it = AllObjectList.find(InActor);
    if (it != AllObjectList.end())
    {
        AllObjectList.erase(it);
        return true;
    }

    return false;
}

