#pragma once

// Optim Engine
#include <Core/OptimEngineGlobal.h>
#include "Core/System/IWindow.h"
#include <Core/Time/Time.h>
// Standard Library
#include <iostream>

// System Specifics
#if defined(OS_WINDOWS)
  #include <dwmapi.h>
  #pragma comment(lib, "Dwmapi.lib")
#endif
#include <sstream>

void* IWindow::getHandle() const { return pSystemWindow; }

bool IWindow::create(Application* _pApplication_, const std::wstring& _title, int32 _positionX, int32 _positionY, int32 _width, int32 _height, IWindow* _parentWindow)
{
  m_pApplication = _pApplication_;
  xPos = _positionX;
  yPos = _positionY;
  width = _width;
  height = _height;
  windowTitle = _title;
  dpi = 96.0f;

  // WINDOWS OS IMPLEMENTATION
#if defined(OS_WINDOWS)
  /*
    - Register Window cLass
  */

  _width = GetSystemMetrics(SM_CXSCREEN);
  _height = GetSystemMetrics(SM_CYSCREEN);

  WNDCLASS wc{ 0 };
  wc.lpfnWndProc = WindowProcedure;
  wc.hInstance = GetModuleHandleA(0);
  wc.lpszClassName = _title.c_str();
  RegisterClassW(&wc);

  pSystemWindow = CreateWindowExW(
    0,
    _title.c_str(),
    _title.c_str(),
    WS_OVERLAPPEDWINDOW,
    0, 0, _width, _height,
    _parentWindow ? reinterpret_cast<HWND>(_parentWindow->getHandle()) : nullptr,
    0,
    GetModuleHandleW(0),
    this);
  return pSystemWindow != nullptr;
#endif
}

void IWindow::display() const
{
#if defined(OS_WINDOWS)
  ShowWindow(reinterpret_cast<HWND>(pSystemWindow), SW_MAXIMIZE);
#endif
}

void IWindow::windowLoop()
{
  /*
    * Windows Loop
    *   - Windos OS: Listen and translate messages
  */
#if defined(OS_WINDOWS)
  while (PeekMessageW(&m_msg, 0, 0, 0, PM_REMOVE) > 0)
  {
    if (m_msg.message == WM_QUIT)
    {
      m_pApplication->Quit();
    }
    else
    {
      TranslateMessage(&m_msg);
      DispatchMessageW(&m_msg);
    }
  }
#endif
}

/*
  Windows Specific Window Procedure
*/
#if defined(OS_WINDOWS)
LRESULT CALLBACK IWindow::WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  // Pointer to the apporpriate IWindowInstance
  IWindow* pThis{ nullptr };

  if (uMsg == WM_NCCREATE)
  {
    CREATESTRUCT* pCreate{ reinterpret_cast<CREATESTRUCT*>(lParam) };
    pThis = reinterpret_cast<IWindow*>(pCreate->lpCreateParams);
    SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    pThis->pSystemWindow = reinterpret_cast<void*>(hwnd);
  }
  else
  {
    pThis = reinterpret_cast<IWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
  }

  switch (uMsg)
  {
  case WM_CREATE:
    pThis->dpi = static_cast<float>(GetDpiForWindow(hwnd)) / USER_DEFAULT_SCREEN_DPI;
    {
      BOOL useDarkMode = TRUE;
      DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(useDarkMode));
      return 0;
    }
  case WM_ACTIVATE:
  {
    //MARGINS margin{ 0, 0, -20, 0 };
    //DwmExtendFrameIntoClientArea(hwnd, &margin);
    return 0;
  }

  case WM_DPICHANGED:
    pThis->dpi = static_cast<float>(GetDpiForWindow(hwnd)) / USER_DEFAULT_SCREEN_DPI;
    return 0;
  case WM_CLOSE:
    DestroyWindow(hwnd);
    return 0;
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
    /*
    * Window Raw Input message.
    *   
    *   Takes Raw input buffer and send to engine 
    *   managed structure for interpretation.
    * 
    */
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
          //std::stringstream ss;
          //ss << "Mouse dx: " << dx << " | dy: " << dy;
          //SetWindowTextA(hwnd, ss.str().c_str());
        }

        if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
        {
          std::stringstream ss;
          ss << "Timestamp: " << Time::getTime();
          SetWindowTextA(hwnd, ss.str().c_str());
        }
        if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
        {
          //std::cout << "Mouse Left Button Up\n";
        }
        if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
        {
          //std::cout << "Mouse Right Button Down\n";
        }
        if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
        {
          //std::cout << "Mouse Right Button Up\n";
        }
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

        if (iconsInput & dualShockTriangle)
          std::cout << "Dualshock Triangle Press!\n";
        if (iconsInput & dualShockCircle)
          std::cout << "Dualshock Circle Press!\n";
        if (iconsInput & dualShockCross)
          std::cout << "Dualshock Cross Press!\n";
        if (iconsInput & dualShockBox)
          std::cout << "Dualshock Box Press!\n";
      }
    }
    delete[] buffer;
    return 0;
  }
  default:
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
  }
}
#endif
