#pragma once

#include "NetConnection.h"


class FRepLayout;
class CObject;
class CNetConnection;
class FRepState;
class FReplicationChangelistMgr;
class CActorChannel;


// 单个对象的复制管理，负责单个对象在特定链接上的复制生命期管理
// - 检测属性变化（通过 RepLayout 和 ChangelistMgr）
// - 序列化变化的属性到网络包
// - 维护属性的影子状态（Shadow State）
// - 处理接收到的属性更新
// - 管理属性的可靠性和重传
class FObjectReplicator
{
public:
    FObjectReplicator():
    Replayout(nullptr),
    Object(nullptr),
    RepState(nullptr),
    Conn(nullptr),
    OwningChannel(nullptr),
    ChangelistMgr(nullptr)
    {
        
    }

    int32 InitWithObject(CObject* InObj, CNetConnection* InConn);

    // 启动属性复制
    void StartReplicating(CActorChannel* InCh);

private:
    void InitObjectProperty();

public:

    // 对象类型的属性布局，用于比较哪些属性发生变化
    FRepLayout* Replayout;
    // 关联的对象实例
    CObject* Object;
    // 跟踪这个链接上的复制状态
    FRepState* RepState;

    CNetConnection* Conn;

    CActorChannel* OwningChannel;

    // 引用的外部管理类，用来追踪对象哪些属性发生过变换，便于属性复制
    FReplicationChangelistMgr* ChangelistMgr;
};
