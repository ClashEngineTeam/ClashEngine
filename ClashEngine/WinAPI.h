#pragma once

//Predefined macros:https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-170

#if defined(_MT) && !defined(_DLL) && !defined(_DEBUG)
#define MT
#elif defined(_MT) && !defined(_DLL) && defined(_DEBUG)
#define MT_DEBUG
#elif defined(_MT) && defined(_DLL) && !defined(_DEBUG)
#define MD
#elif defined(_MT) && defined(_DLL) && defined(_DEBUG)
#define MD_DEBUG
#endif

#if defined(_DEBUG)
#define DEBUG
#else
#define RELEASE
#endif

#if defined(_M_X64)
#define X64
#else
#define X86
#endif

#define UNICODE
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
