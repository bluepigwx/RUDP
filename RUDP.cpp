// RUDP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "Common.h"
#include "Test.h"



int main(int argc, char** argv)
{
    Common_Init(argc, argv);

    /////////////////////////////////////
    //Test Begin
    
    Test();
    
    //Test End
    ////////////////////////////////////

    Common_Run();

    Common_Finish();
}
