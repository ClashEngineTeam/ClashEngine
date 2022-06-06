#pragma once

#include "MinDefines.hpp"
#pragma comment(lib, "Imm32.lib")

namespace ClashEngine
{
    //参考:https://www.zhihu.com/question/57864600
    //TestEditLine Github:https://github.com/guee/TestEditLine
    //stack over flow:https://stackoverflow.com/questions/434048/how-do-you-use-ime

    //TODO:当前存在一个问题，ShowIMEWindow只能在WndProc中生效，这也导致了第一次输入消息时输入法可能会在意想不到的地方出现。

    //Windows Input Method Editor class
    class WinIMEHelper
    {
    public:
        //注意:该函数只有在WndProc中调用才能生效
        static void ShowIMEWindow(HWND hInstance, int x, int y)
        {
            //取得与当前窗口相关联的输入法句柄
            HIMC imc = ImmGetContext(hInstance);
            //设置输入法提示框的位置
            COMPOSITIONFORM cf;
            cf.dwStyle = CFS_FORCE_POSITION;
            cf.ptCurrentPos.x = x;
            cf.ptCurrentPos.y = y;
            bool setSuccess = ImmSetCompositionWindow(imc, &cf);
            //释放相关内存
            ImmReleaseContext(hInstance, imc);
        }
    };
}