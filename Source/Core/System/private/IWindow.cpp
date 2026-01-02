/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      + IWindow.cpp :
          Base system window class definition

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Defines/Windows/WindowsProcs.h"
#include "Core/Time/Time.h"
#include "Core/Exception/exception.h"
#include "Core/System/Application.h"
#include "Core/System/IWindow.h"
#include <iostream>
#include <sstream>

//static LPARAM __stdcall defWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void* IWindow::getHandle() const { return pSystemWindow; }

bool IWindow::create(Application* _pApplication_, String&& name, int32 _positionX, int32 _positionY, int32 _width, int32 _height, IWindow* _parentWindow)
{
  m_pApplication  = _pApplication_;
  xPos            = _positionX;
  yPos            = _positionY;
  width           = _width;
  height          = _height;
  windowTitle     = name;
  dpi             = 96.0f;

  // WINDOWS OS IMPLEMENTATION
#if defined(OS_WINDOWS)
  WNDCLASSEXW wc{ 0 };
  wc.cbSize         = sizeof(WNDCLASSEXW);
  wc.lpfnWndProc    = op::sys::windows::windowProcedure;
  wc.hInstance      = GetModuleHandleW(0);
  wc.lpszClassName  = name.value();
  wc.hIcon          = nullptr,
  wc.hCursor        = nullptr,

  //RegisterClassW(&wc);
  RegisterClassEx(&wc);

  pSystemWindow = CreateWindowExW(
    0,
    name.value(),
    name.value(),
    WS_OVERLAPPEDWINDOW,
    0, 0, 800, 450,
    _parentWindow ? RCAST(HWND, _parentWindow->getHandle()) : nullptr,
    0,
    GetModuleHandleW(0),
    this);


  ShowWindow(reinterpret_cast<HWND>(pSystemWindow), SW_MAXIMIZE);

  op::sys::windows::initRawInput(pSystemWindow);

  return pSystemWindow != nullptr;
#endif
}

bool IWindow::windowLoop()
{
  return op::sys::windows::messageLoop();
}
