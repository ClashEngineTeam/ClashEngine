#include "Process.hpp"
#include <shellapi.h>
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

namespace ClashEngine
{
    HINSTANCE Process::Execute(const std::wstring& file, const std::wstring& param, const std::wstring& directory)
    {
        return ::ShellExecute(nullptr, _T("open"), file.c_str(), param.c_str(), directory.c_str(), SW_SHOW);
    }

    std::vector<DWORD> Process::GetAllProcess()
    {
        const int PROCESS_ARRAY_SIZE = 1024;
        DWORD processes[PROCESS_ARRAY_SIZE];
        DWORD needed;
        bool suc = EnumProcesses(processes, PROCESS_ARRAY_SIZE * sizeof(DWORD), &needed);
        int processCount = needed / sizeof(DWORD);

        std::vector<DWORD> allProcess;
        for (int i = 0; i < processCount; i++)
        {
            allProcess.push_back(processes[i]);
        }
        return allProcess;
    }

    std::wstring Process::GetName(DWORD pid)
    {
        TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

        // Get a handle to the process.
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
            PROCESS_VM_READ,
            FALSE, pid);

        // Get the process name.
        if (NULL != hProcess)
        {
            HMODULE hMod;
            DWORD cbNeeded;

            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
                &cbNeeded))
            {
                GetModuleBaseName(hProcess, hMod, szProcessName,
                    sizeof(szProcessName) / sizeof(TCHAR));
            }

            // Release the handle to the process.
            CloseHandle(hProcess);
        }

        return std::wstring(szProcessName);
    }

    HANDLE Process::GetHandle(DWORD pid)
    {
        // Get a handle to the process.
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        return hProcess;
    }

    bool Process::Kill(HANDLE handle)
    {
        //句柄必须具有PROCESS_TERMINATE访问权限。
        return TerminateProcess(handle, 0);
    }
}