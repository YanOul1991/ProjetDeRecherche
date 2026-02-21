/* ======================================================================================
 *  OptimEngineGlobal.h:
 *    This file contains definitions and helper functions generic to all Microsoft API
 *    such as WindowsAPI and COM.
 *
 *    This file does not contain any definitions or functions
 *    related to DirectX APIs, those will be implemented in their own separate
 *    modules.
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/Types/string.h"

#if defined(_WIN32) || defined(_WIN64)
  #define OS_WINDOWS
#endif

#if defined(OS_WINDOWS) && !defined(WIN_32_LEAN_AND_MEAN)
  #define WIN_32_LEAN_AND_MEAN
#endif // !WIN_32_LEAN_AND_MEAN

#if defined(OS_WINDOWS) && !defined(UNICODE)
  #define UNICODE
#endif

#if defined(OS_WINDOWS)
  #include "wrl.h"

  #include <Windows.h>
  #pragma comment(lib, "kernel32")
  #pragma comment(lib, "user32")
  #pragma comment(lib, "gdi32")

// Shortcut for ComPtr

template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

namespace op::sys::windows {
// Help function to translate HRESULT codes into
// descriptive text.
inline String translateError(HRESULT hr) {
  char* msgBuffer = nullptr;
  DWORD msgLength = FormatMessageA(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    nullptr,
    hr,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    reinterpret_cast<LPSTR>(&msgBuffer),
    0,
    nullptr);

  if (msgLength == 0) {
    return String("Unknown error code");
  }

  String errMsg = String(msgBuffer);
  LocalFree(msgBuffer);
  return errMsg;
}
} // namespace op::sys::windows
#endif