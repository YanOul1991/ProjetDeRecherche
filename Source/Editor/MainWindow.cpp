#include <sstream>
#include "MainWindow.h"
#include "windowsx.h"
#include "Devices/Mouse.h"

MainWindow* MainWindow::Handle() 
{ 
  return pSelfMainWindow;
}

MainWindow* MainWindow::pSelfMainWindow = nullptr;

MainWindow::MainWindow() :
  m_dpiScale{ USER_DEFAULT_SCREEN_DPI },
  running{ false }
{ }

MainWindow::~MainWindow()
{
 pSelfMainWindow = nullptr;
}

// Windows messages handling procedure.
LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_CREATE:
    pSelfMainWindow = this;
    return 0;
  case WM_DPICHANGED:
    m_dpiScale = static_cast<float>(GetDpiForWindow(hWindowHandle)) / USER_DEFAULT_SCREEN_DPI;
    return 0;
  case WM_LBUTTONDOWN:
    //const int posX{ GET_X_LPARAM(lParam) };
    //const int posY{ GET_Y_LPARAM(lParam) };
    //Mouse::m_posX = posX;
    //Mouse::m_posY = posY;
    return 0;
  case WM_MOUSEMOVE:
  {
    int xPos{ GET_X_LPARAM(lParam) };
    int yPos{ GET_Y_LPARAM(lParam) };
    Mouse::m_posX = xPos;
    Mouse::m_posY = yPos;
    return 0;
  }
  case WM_SIZE:
    m_OnWindowResize.broadcast();
    return 0;
  case WM_CLOSE:
    running = false;
    DestroyWindow(hWindowHandle);
    return 0;
  default:
    return DefWindowProcW(hWindowHandle, uMsg, wParam, lParam);
  }
}

// Change DPI scale on window resize.
float MainWindow::GetDpiScale() const { return m_dpiScale; }

// Window resizing event.
MainWindow::OnWindowResizeEvent& MainWindow::OnWindowResize()     { return m_OnWindowResize;  }
MainWindow::OnMouseLeftClickEvent& MainWindow::OnMouseLeftClick() { return m_evtOnMouseLeftClick; }