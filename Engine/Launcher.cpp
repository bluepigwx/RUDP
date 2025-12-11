#include "Launcher.h"

#include "Engine.h"
#include "../Core/CObject.h"
#include "../Test/TestEngine.h"


IEngine* GEngine = nullptr;

int32 Launcher::Init()
{
    //GEngine = NewObject<CEngine>(CEngine::StaticClass());
    // 测试引擎
    GEngine = NewObject<CEngine>(CTestEngine::StaticClass());

    GEngine->Init();
    return 0;
}


void Launcher::Finit()
{
    GEngine->Finit();
    DestroyObject(dynamic_cast<CObject*>(GEngine));
}


