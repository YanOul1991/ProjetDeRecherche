#include "Global/OptimEngineGlobal.h"
#include "Editor/MainWindow.h"
#include "DirectX/Direct2D/Direct2D.h"

#pragma warning(disable: 4100)

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
  MainWindow window;
   
  if (!window.Create(L"Optim Engine", WS_OVERLAPPEDWINDOW))
  {
    MessageBoxA(window.Window(), "Failed to create the Window", "Warning", MB_OK);
    return 1;
  }

  Graphics direct2d(window);
  direct2d.createGraphicsResources();

  ShowWindow(window.Window(), nCmdShow);

  window.running = true;

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