#pragma once

#ifdef DLL_LIB
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif

#include "BaseWindow.h"
#include "Event/Event.h"

class CORE_API MainWindow final : public Window<MainWindow>
{
public:
  static MainWindow* Handle();

  MainWindow();
  ~MainWindow();
  PCWSTR ClassName() const override { return L"MainWindow"; }               // Get the class name of the main window.
  float GetDpiScale() const;                                                // Get the main window's DPI scale
  bool running{ false };  
  // Running state of the window
  LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;  // Function to handle Windows messages

  // Window Resizing event.
  DECLARE_EVENT(MainWindow, OnWindowResizeEvent);
  OnWindowResizeEvent& OnWindowResize();

  DECLARE_EVENT(MainWindow, OnMouseLeftClickEvent);
  OnMouseLeftClickEvent& OnMouseLeftClick();

private:
  static MainWindow* pSelfMainWindow;

  // No copies
  MainWindow(const MainWindow&) = delete;
  MainWindow(const MainWindow&&) = delete;
  MainWindow& operator=(const MainWindow&) = delete;
  MainWindow& operator=(const MainWindow&&) = delete;

  // Events
  OnWindowResizeEvent m_OnWindowResize;
  OnMouseLeftClickEvent m_evtOnMouseLeftClick;

  // Window DPI
  float m_dpiScale;
};