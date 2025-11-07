#include "Channel.h"


IMPLAMENT_CLASS_BEGIN(CChannel)
IMPLAMENT_CLASS_END(CChannel, CObject)


int CChannel::Init(CNetConnection* InConnection, int ChIdx)
{
    NetConnection = InConnection;
    Index = ChIdx;

    return 0;
}




IMPLAMENT_CLASS_BEGIN(CActorChannel)
IMPLAMENT_CLASS_END(CActorChannel, CChannel)
