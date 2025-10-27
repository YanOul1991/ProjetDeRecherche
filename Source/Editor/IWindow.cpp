#pragma once

#include "IWindow.h"
#include "Global/OptimEngineGlobal.h"
#include <sstream>

void* IWindow::GetHandle() const { return m_pHandle; }

bool IWindow::Create(const wstring& _title, int32 _positionX, int32 _positionY, int32 _width, int32 _height, IWindow* _parentWindow)
  {
    xPos = _positionX;
    yPos = _positionY;
    width = _width;
    height = _height;
    windowTitle = _title;
    dpi = 96.0f;

    /*
      Windows OS specific window creation
    */
#if defined(WINDOWS_PLATFORM)
    int32 screenWidth{ GetSystemMetrics(SM_CXSCREEN) };
    int32 screenHeight{ GetSystemMetrics(SM_CYSCREEN) };

    /*
      Make the Window 85% of the main monitor's width
      and adjust the height to make the window 16/9 format
    */
    width = static_cast<int32>(screenWidth * 0.85f);
    height = static_cast<int32>((9.0f * screenWidth) / 16.0f);

    /*
      Center the window on the screen
    */
    xPos = static_cast<int32>((screenWidth / 2) - (width / 2));
    yPos = static_cast<int32>((screenHeight / 2) - (height / 2));

    WNDCLASS wc{ 0 };
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = GetModuleHandleA(0);
    wc.lpszClassName = _title.c_str();
    RegisterClassW(&wc);

    m_pHandle = CreateWindowExW(
      0,
      _title.c_str(),
      _title.c_str(),
      WS_OVERLAPPEDWINDOW,
      xPos, yPos, width, height,
      _parentWindow ? reinterpret_cast<HWND>(_parentWindow->GetHandle()) : nullptr,
      0,
      GetModuleHandleW(0),
      this);
    return m_pHandle != nullptr;
#endif
}

/*
  Windows Specific Window Procedure
*/
#if defined(WINDOWS_PLATFORM)
LRESULT CALLBACK IWindow::WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  // Pointer to the apporpriate IWindowInstance
  IWindow* pThis{ nullptr };

  if (uMsg == WM_NCCREATE)
  {
    CREATESTRUCT* pCreate{ reinterpret_cast<CREATESTRUCT*>(lParam) };
    pThis = reinterpret_cast<IWindow*>(pCreate->lpCreateParams);
    SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    pThis->m_pHandle = reinterpret_cast<void*>(hwnd);
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

      MARGINS margins = { 0, 0, 200, 0 };
      DwmExtendFrameIntoClientArea(hwnd, &margins);
    }
    return 0;
  case WM_DPICHANGED:
    pThis->dpi = static_cast<float>(GetDpiForWindow(hwnd)) / USER_DEFAULT_SCREEN_DPI;
    return 0;
  case WM_NCCALCSIZE:
    if (wParam == TRUE)
    {
      NCCALCSIZE_PARAMS* param{ reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam) };
      param->rgrc[0].top += 200;
    }
    return 0;
  case WM_LBUTTONDOWN:
    return 0;
  default:
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
  }
}
#endif
