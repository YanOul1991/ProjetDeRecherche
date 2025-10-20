#pragma once

#include "Global/OptimEngineGlobal.h"

// Base window class template.
template<class DERIVED>
class BaseWindow
{
protected:
  virtual PCWSTR ClassName() const = 0;
  virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

public:
  HWND hWindowHandle;
  static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    DERIVED* pThis{ nullptr };

    if (uMsg == WM_NCCREATE)
    {
      CREATESTRUCT* pCreate{ reinterpret_cast<CREATESTRUCT*>(lParam) };
      pThis = reinterpret_cast<DERIVED*>(pCreate->lpCreateParams);
      SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
      pThis->hWindowHandle = hwnd;
    }
    else
    {
      pThis = reinterpret_cast<DERIVED*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis)
    {
      return pThis->HandleMessage(uMsg, wParam, lParam);
    }
    else
    {
      return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
  }

  // Class constructor
  BaseWindow() : hWindowHandle{ NULL } {};

  // Create Window function
  BOOL Create(
    PCWSTR lpWindowName,
    DWORD dwStyle,
    DWORD dwExStyle = 0,
    int x = CW_USEDEFAULT,
    int y = CW_USEDEFAULT,
    int nWidth = CW_USEDEFAULT,
    int nHeight = CW_USEDEFAULT,
    HWND hParentWindow = 0,
    HMENU hMenu = 0)
  {
    // Create and register a new window class
    WNDCLASS wc{ 0 };
    wc.lpfnWndProc = DERIVED::WindowProcedure;
    wc.hInstance = GetModuleHandleW(0);
    wc.lpszClassName = ClassName();
    RegisterClassW(&wc);

    // Create the actual window
    hWindowHandle = CreateWindowExW(
      dwExStyle,
      ClassName(),
      lpWindowName,
      dwStyle,
      x, y, nWidth, nHeight,
      hParentWindow,
      hMenu,
      GetModuleHandleW(NULL),
      this);

    return (hWindowHandle ? TRUE : FALSE);
  }

  // Returns a handle to the Window.
  HWND Window() const { return hWindowHandle; }
};
