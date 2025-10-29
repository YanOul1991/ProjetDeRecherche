#pragma once

#if defined(_WIN32) || defined(_WIN64)

#if !defined(UNICODE)
#define UNICODE
#endif

#if !defined(WIN_32_LEAN_AND_MEAN)
  #define WIN_32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <ShObjIdl.h>
#include <ShlGuid.h>
#include <objbase.h>
//#include <d2d1.h>
#include <ShellScalingApi.h>

#pragma comment(lib, "kernel32")
#pragma comment(lib, "user32")
#pragma comment(lib, "gdi32")
#pragma comment(lib, "Shell32")
#pragma comment(lib, "Ole32")
#pragma comment(lib, "d2d1")

// The window's current DPI.
#define DEBUG_DPI GetAwarenessFromDpiAwarenessContext(GetThreadDpiAwarenessContext())

#endif