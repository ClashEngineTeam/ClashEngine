#pragma once

//Predefined macros:https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-170

//MT/MT_DEBUG/MD/MD_DEBUG
#if defined(_MT) && !defined(_DLL) && !defined(_DEBUG)
#define MT
#elif defined(_MT) && !defined(_DLL) && defined(_DEBUG)
#define MT_DEBUG
#elif defined(_MT) && defined(_DLL) && !defined(_DEBUG)
#define MD
#elif defined(_MT) && defined(_DLL) && defined(_DEBUG)
#define MD_DEBUG
#endif

//DEBUG/RELEASE
#if defined(_DEBUG)
#define DEBUG
#else
#define RELEASE
#endif

//X86/X64
#if defined(_M_X64)
#define X64
#else
#define X86
#endif

//Enable Unicode
#if !defined(UNICODE)
#define UNICODE
#endif
//Disable warning
#define _CRT_SECURE_NO_WARNINGS
//Disable network library warning
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//No min/max
#define NOMINMAX
//Reduces the size of the Win32 header files by excluding some of the less frequently used APIs
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>    //Windows API
#include <combaseapi.h> //CoTaskMemAlloc CoTaskMemFree

#define LEN(arr) (sizeof(arr) / sizeof(arr[0]))
#define _T(x) TEXT(x)
#define NEW_LINE "\r\n"
#define NEW_LINEW L"\r\n"

#if defined(ENABLE_EXPORT_FUNC)
//Macros for export functions
#if defined(__cplusplus)
#define EXPORT_FUNC_EX(return_type) extern "C" __declspec(dllexport) return_type __stdcall
#else
#define EXPORT_FUNC_EX(return_type) extern __declspec(dllexport) return_type __stdcall
#endif
#else
#define EXPORT_FUNC_EX(return_type) return_type
#endif

//Macros for EXPORT_FUNC_EX
#define _IN_
#define _OUT_
#define _REF_
#define _ARRAY_

//Macros for NativeFunctionTranslator
#define EXPORT_ENUM_CLASS enum class
#define EXPORT_STRUCT struct
#define EXPORT_STRUCT_MEMBER
#define EXPORT_DELEGATE
#define EXPORT_CONSTEXPR constexpr

//CUI/WCUI/GUI/WGUI
#if defined(CUI)        //(Command User Interface)
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#define main main(int argc, const char** argv)
#endif

#if defined(WCUI)       //(Command User Interface, wide character version)
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#define main wmain(int argc, const wchar_t** argv)
#endif

#if defined(GUI)        //(Graphical User Interface)
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#define main WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* pCmdLine, int nCmdShow)
#endif

#if defined(WGUI)       //(Graphical User Interface, wide character version)
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#define main WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, wchar_t* pCmdLine, int nCmdShow)
#endif

namespace ClashEngine
{
    typedef wchar_t wchar;

    typedef unsigned char byte;

    typedef unsigned short ushort;

    typedef unsigned int uint;

    typedef unsigned long long ulong;

    //if fail throw "Extern Alloc Exception"
    template<typename T> inline T* ExternAlloc(int count)
    {
        T* ptr = (T*)::CoTaskMemAlloc(count * sizeof(T));
        if (ptr == nullptr) throw "Extern Alloc Exception";
        return ptr;
    }

    inline void ExternFree(void* ptr)
    {
        ::CoTaskMemFree(ptr);
    }
}