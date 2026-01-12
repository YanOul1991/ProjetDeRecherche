/* ======================================================================================
 *  WindowsProc.cpp:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

//#include "Core/System/IWindow.h"

#include "Core/Defines/Windows/WindowsProcs.h"
#include "Core/Exception/exception.h"
#include "Core/Input/Input.h"

#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")

/* +++++++++++++++++++++++++++
    Windows - Message Loop
+++++++++++++++++++++++++++ */

static MSG m_msg;

bool op::sys::windows::messageLoop()
{
  while (PeekMessageW(&m_msg, 0, 0, 0, PM_REMOVE) > 0)
  {
    if (m_msg.message == WM_QUIT)
      return true;

    TranslateMessage(&m_msg);
    DispatchMessageW(&m_msg);
  }
  return false;
}

/* +++++++++++++++++++++++++++
    Windows - Window Proc
+++++++++++++++++++++++++++ */

LPARAM __stdcall op::sys::windows::windowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	IWindow* pWindow{ nullptr };

	if (uMsg == WM_NCCREATE)
	{
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		pWindow = reinterpret_cast<IWindow*>(pCreate->lpCreateParams);
		SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		pWindow->pSystemWindow = reinterpret_cast<void*>(hwnd);
	}
	else
	{
		pWindow = reinterpret_cast<IWindow*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
	}

	/////////////////////////////////////////////// SWITCH CASE START

	switch (uMsg)
	{
	case WM_CREATE:
		pWindow->dpi = static_cast<float>(GetDpiForWindow(hwnd)) / USER_DEFAULT_SCREEN_DPI;
		{
			BOOL useDarkMode = TRUE;
			DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(useDarkMode));
			return 0;
		}

	case WM_DPICHANGED:
		pWindow->dpi = static_cast<float>(GetDpiForWindow(hwnd)) / USER_DEFAULT_SCREEN_DPI;
		return 0;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

  case WM_INPUT:
  {
    UINT size = 0;
    GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
    BYTE* buffer = new BYTE[size];

    if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER)) == size)
    {
      RAWINPUT* raw = (RAWINPUT*)buffer;
      /*
      *   Keyboard raw inputs
      */
      if (raw->header.dwType == RIM_TYPEKEYBOARD)
      {
        RAWKEYBOARD& keyboard = raw->data.keyboard;
      }

      /*
      *   Mouse Raw Inputs
      */
      if (raw->header.dwType == RIM_TYPEMOUSE)
      {
        RAWMOUSE& mouse = raw->data.mouse;

        if (mouse.usFlags == MOUSE_MOVE_RELATIVE)
        {
          int dx = mouse.lLastX;
          int dy = mouse.lLastY;

          POINT pt;
          GetCursorPos(&pt);

          Mouse::posX = (int)pt.x;
          Mouse::posY = (int)pt.y;
        }

        if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)  { }
        if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)    { }
        if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) { }
        if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)   { }
      }

      /*
      *   HID Raw inputs
      */
      if (raw->header.dwType == RIM_TYPEHID)
      {
        RAWHID& controller = raw->data.hid;

        char dualShockTriangle = 0b1000;
        char dualShockCircle = 0b0100;
        char dualShockCross = 0b0010;
        char dualShockBox = 0b0001;

        char iconsInput = (controller.bRawData[8] & 0xF0) >> 4;

        //if (iconsInput & dualShockTriangle)
        //  std::cout << "Dualshock Triangle Press!\n";
        //if (iconsInput & dualShockCircle)
        //  std::cout << "Dualshock Circle Press!\n";
        //if (iconsInput & dualShockCross)
        //  std::cout << "Dualshock Cross Press!\n";
        //if (iconsInput & dualShockBox)
        //  std::cout << "Dualshock Box Press!\n";
      }
    }
    delete[] buffer;
    return 0;
  }
	default:
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}
}

void op::sys::windows::initRawInput(void* pWindow)
{
  if (pWindow == nullptr) return;

  HWND windowHandle = reinterpret_cast<HWND>(pWindow);

  RAWINPUTDEVICE rid[3]{};

  // Mouse device
  rid[0].usUsagePage = 0x01;
  rid[0].usUsage = 0x02;
  rid[0].dwFlags = RIDEV_INPUTSINK;
  rid[0].hwndTarget = windowHandle;

  // Keyboard device
  rid[1].usUsagePage = 0x01;
  rid[1].usUsage = 0x06;
  rid[1].dwFlags = RIDEV_INPUTSINK;
  rid[1].hwndTarget = windowHandle;

  // Gamepad
  rid[2].usUsagePage = 0x01;
  rid[2].usUsage = 0x05;
  rid[2].dwFlags = RIDEV_INPUTSINK;
  rid[2].hwndTarget = windowHandle;

  if (!RegisterRawInputDevices(rid, 3, sizeof(RAWINPUTDEVICE)))
  {
    MessageBox(0, TEXT("Failed to register raw inputs"), TEXT("Error"), MB_OK);
  }
}