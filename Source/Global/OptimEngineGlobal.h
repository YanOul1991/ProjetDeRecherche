#pragma once

#define OPTIMENGINE_EDITOR_CONSOLE_DEBUG

/*
  OS defines
*/
#if defined(_WIN32) || defined(_WIN64)
#define OP_OS_WINDOWS
#endif

/*
  Compiler defines
*/
#if defined(_MSC_VER)
#define OP_COMPILER_MSVC
#elif defined(__GNUC__)
#define OP_COMPILER_GNU
#else
#error "No valid compiler found."
#endif

#if defined(OP_COMPILER_MSVC)
#define EXTERNAL_MODULE_EXPORT __declspec(dllexport)
#define EXTERNAL_MODULE_IMPORT __declspec(dllimport)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#pragma warning(disable: 4100)
#endif

#if defined(OP_OS_WINDOWS) && defined(OP_COMPILER_MSVC) && defined(DLL_LIB) && defined(USING_GRAPHICS_API)
#define GRAPHICS_API __declspec(dllexport)
#endif

#if defined(OP_OS_WINDOWS) && defined(OP_COMPILER_MSVC) && defined(DLL_LIB) && defined(USING_APPLICATION_API)
#define APPLICATION_API __declspec(dllexport)
#endif

#if defined(OP_OS_WINDOWS) && defined(OP_COMPILER_MSVC) && defined(DLL_LIB) && defined(USING_DIRECT3D11_OPTIM_API)
#define DIRECT3D11_OPTIM_API __declspec(dllexport)
#endif

#include <iostream>
#include "Types/OptimEngineTypeLayers.h"
#include "Types/Color.h"