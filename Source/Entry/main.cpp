#include "Global/OptimEngineGlobal.h"
#include "Editor/MainWindow.h"
#include "DirectX/Direct2D/Direct2D.h"
#include "Object/Object.h"
#include "Object/Component.h"

#pragma warning(disable: 4100)

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
  MainWindow window;

  //WINDOWPLACEMENT wpPrev;
  //DWORD dwStylePrev;

   
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
    MessageBoxA(window.Window(), "Failed to create the Window", "Warning", MB_OK);
    return 1;
  }

  // Initialize Direct 2D
  // And create GPU resources
  OpDirect2D direct2d{};
  direct2d.createGraphicsResources();

  // Show the MainWindow
  //ShowWindow(window.Window(), nCmdShow);
  ShowWindow(window.Window(), SW_MAXIMIZE);
  window.running = true;
  
  //RECT winRect;
  //GetWindowRect(window.Window(), &winRect);
  //ClipCursor(&winRect);

  Component comp{};

  MSG msg{};
  while (window.running)
  {
    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE) > 0)
    {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
    }
    direct2d.draw();
  }

  return 0;
}