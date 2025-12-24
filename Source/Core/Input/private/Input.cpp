/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      + Input.cpp:
          Definitions for Input.h header.

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#include "Core/Input/Input.h"
#include "Core/OptimEngineGlobal.h"
#include <Windows.h>

bool op::SInput::initialize(void* _pApplication, void* p_WindowHandle)
{
#ifdef OS_WINDOWS
  RAWINPUTDEVICE rid[3]{};

  // Mouse device
  rid[0].usUsagePage = 0x01;
  rid[0].usUsage = 0x02;
  rid[0].dwFlags = RIDEV_INPUTSINK;
  rid[0].hwndTarget = reinterpret_cast<HWND>(p_WindowHandle);

  // Keyboard device
  rid[1].usUsagePage = 0x01;
  rid[1].usUsage = 0x06;
  rid[1].dwFlags = RIDEV_INPUTSINK;
  rid[1].hwndTarget = reinterpret_cast<HWND>(p_WindowHandle);

  // Gamepad
  rid[2].usUsagePage = 0x01;
  rid[2].usUsage = 0x05;
  rid[2].dwFlags = RIDEV_INPUTSINK;
  rid[2].hwndTarget = reinterpret_cast<HWND>(p_WindowHandle);

  if (!RegisterRawInputDevices(rid, 1, sizeof(RAWINPUTDEVICE)))
  {
    MessageBoxA(0, "Failed to register raw inputs", "Error", MB_OK + MB_ICONERROR);
    return false;
  }
  else
  {
    return true;
  }
#endif // OS_WINDOWS
}