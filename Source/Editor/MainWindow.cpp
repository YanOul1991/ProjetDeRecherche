#include "MainWindow.h"
#include <sstream>

MainWindow::MainWindow() :
  m_dpiScale{ USER_DEFAULT_SCREEN_DPI },
  running{ false }
{ }

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_CREATE:
    MessageBoxA(hWindowHandle, "A new Window was Created!!!", "Window Creation", MB_OK + MB_ICONASTERISK);
    return 0;
  case WM_DPICHANGED:
    m_dpiScale = static_cast<float>(GetDpiForWindow(hWindowHandle)) / USER_DEFAULT_SCREEN_DPI;
    //{
    //  std::stringstream ss;
    //  ss << "DPI Changed! Current Window's DPI scale is " << m_dpiScale << "\nThe DPI is currently : " << GetDpiForWindow(hWindowHandle);
    //  MessageBoxA(hWindowHandle, ss.str().c_str(), "DPI", MB_OK + MB_ICONASTERISK);
    //}
    return 0;
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

float MainWindow::GetDpiScale() const { return m_dpiScale; }

MainWindow::OnWindowResizeEvent& MainWindow::OnWindowResize() { return m_OnWindowResize;  }