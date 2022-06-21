#pragma once

#include "MinDefines.hpp"
#include <string>
#include <vector>

namespace ClashEngine
{
    class Process
    {
    private:
        Process()
        {
        }

    public:
        //Parameters : chrome.exe https://www.youtube.com/
        static HINSTANCE Execute(const std::wstring& file, const std::wstring& param = L"", const std::wstring& directory = L"");

        //Link:https://docs.microsoft.com/en-us/windows/win32/psapi/enumerating-all-processes
        static std::vector<DWORD> GetAllProcess();

        //返回进程名 某些进程无法打开则返回<unknown>
        static std::wstring GetName(DWORD pid);

        //返回进程句柄 必要时需要调用CloseHandle回收进程的句柄
        static HANDLE GetHandle(DWORD pid);

        //杀死一个进程
        static bool Kill(HANDLE handle);
    };
}