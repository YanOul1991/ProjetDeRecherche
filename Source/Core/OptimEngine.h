/* ======================================================================================
 *  OptimEngine.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

using int8          = signed char;
using int16        = short;
using int32        = int;
using int64        = long long;
using uint8        = unsigned char;
using uint16      = unsigned short;
using uint32      = unsigned int;
using uint64      = unsigned long long;
using wchar       = wchar_t;

#if !defined(__cplusplus)
  #error C++ is not defined.
#endif

// GET PLATFORM
#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS
#elif defined(__linux__)
    #define OS_LINUX
#endif

// GET COMPILER

#if defined(_MSC_VER)
    #define COMPILER_MSVC
#elif defined(__GNUC__)
    #define COMPILER_GNU
#else
  #error No valid compiler found.
#endif

#if defined(OS_WINDOWS) && defined(COMPILER_MSVC)
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#endif

#pragma warning(disable : 4005)
#define TEXT(_VALUE_) L##_VALUE_

#if defined(CORE_EXPORT)
  #define CORE_API EXPORT
#else
  #define CORE_API IMPORT
#endif

#if defined(DIRECTX11_EXPORT)
  #define DIRECTX11_API EXPORT
#else
  #define DIRECTX11_API IMPORT
#endif

#define PROC_PTR(T) T(*)()
#define PROC_PTR_PARAMS(T)

#if defined(UNICODE)
  #define STRINGIFY2(x) L#x
#else
  #define STRINGIFY2(x) #x
#endif

#define STRINGIFY(x) STRINGIFY2(x)

#define CAST(_TYPE_, _VAL_)   static_cast<_TYPE>(_VAL_)
#define RCAST(_TYPE_, _VAL_)  reinterpret_cast<_TYPE_>(_VAL_)

#define THROW_EXCEPTION(_MSG_) throw Exception(__LINE__, __FILEW__, _MSG_)