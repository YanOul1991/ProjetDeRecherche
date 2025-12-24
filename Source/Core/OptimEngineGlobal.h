/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      + OptimEngineGlobal.h :
          Optim Engine global definitions.

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#pragma once

#include <cstdint>

using int8    = int8_t;
using int16   = int16_t;
using int32   = int32_t;
using int64   = int64_t;
using uint8   = uint8_t;
using uint16  = uint16_t;
using uint32  = uint32_t;
using uint64  = uint64_t;
using wchar   = wchar_t;


#define STRING(_TEXT_) L##_TEXT_

#if !defined(__cplusplus)
  #error C++ is not defined.
#endif

/*
  OS defines
*/
#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS
#elif defined(__linux__)
    #define OS_LINUX
#endif

/*
  Compiler defines
*/
#if defined(_MSC_VER)
    #define COMPILER_MSVC
#elif defined(__GNUC__)
    #define COMPILER_GNU
#else
  #error No valid compiler found.
#endif

/*
    Compiler specific abstraction
*/
#if defined(OS_WINDOWS) && defined(COMPILER_MSVC)
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
    #define FORCE_INLINE __forceinline
    #define WINDOW_PROC
#elif defined(COMPILER_GNU)     
    #define FORCE_INLINE inline __attribute__((always_inline))
#endif

#ifdef OS_WINDOWS
    #ifndef UNICODE
        #define UNICODE
    #endif // !UNICODE

    #ifndef WIN_32_LEAN_AND_MEAN
        #define WIN_32_LEAN_AND_MEAN
    #endif // !WIN_32_LEAN_AND_MEAN
    
    #include <Windows.h>
    #include <ShObjIdl.h>
    #include <ShlGuid.h>
    #include <objbase.h>
    #include <ShellScalingApi.h>

    #pragma comment(lib, "kernel32")
    #pragma comment(lib, "user32")
    #pragma comment(lib, "gdi32")
    #pragma comment(lib, "Shell32")
    #pragma comment(lib, "Ole32")

#endif // OS_WINDOWS

namespace op::system
{
#define PROC_PTR(T) T*(*)()
#define PROC_PTR_PARAMS(T)

  template <typename T>
  inline T LOAD_LIB_PROC(const char* _path_, const char* _proc_name_)
  {
    // WINDOWS DEFINITION
#if defined(OS_WINDOWS) 
    HMODULE pMod = LoadLibraryA(_path_);
    if (!pMod)
      return 0;

    T proc = (T)GetProcAddress(pMod, _proc_name_);

    if (!proc)
      return nullptr;
    return proc;
#endif // OS_WINDOWS
  }
}