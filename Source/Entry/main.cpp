#include "Editor/MainWindow.h"
#include "Application/Application.h"

#pragma warning(disable: 4100)

// Application Entry Point
int main() {
  Application application{};
  MainWindow window;

  // Basic main window style 
  constexpr DWORD windowStyle{
    WS_OVERLAPPED   | 
    WS_CAPTION      | 
    WS_SYSMENU      | 
    WS_THICKFRAME   | 
    WS_MINIMIZEBOX  | 
    WS_MAXIMIZEBOX  |
    WS_MAXIMIZE
  };

  // Main Window's extra style
  constexpr DWORD windowExtraStyle{ 
    WS_EX_CONTEXTHELP 
  };

  int screenWidth   { GetSystemMetrics(SM_CXSCREEN) };
  int screenHeight  { GetSystemMetrics(SM_CYSCREEN) };
  int windowWidth   { static_cast<int>(screenWidth * 0.85f) };
  int windowHeight  { static_cast<int>((9.0f * windowWidth) / 16.0f) };
  int windowPosX    { static_cast<int>((screenWidth / 2) - (windowWidth / 2)) };
  int windowPosY    { static_cast<int>((screenHeight / 2) - (windowHeight / 2)) };

  if (!window.Create(L"Optim Engine", windowStyle, windowExtraStyle, windowPosX, windowPosY, windowWidth, windowHeight))
  {
    MessageBoxA(window.GetHandle(), "Failed to create the Window", "Warning", MB_OK);
    return 1;
  }

  ShowWindow(window.GetHandle(), SW_MAXIMIZE);
  window.running = true;

  application.ApplicationStart();

  MSG msg{};
  while (window.running) {
    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
    }
  }

  application.ApplicationQuit();
  return 0;
}