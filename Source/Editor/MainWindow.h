#pragma once

#ifdef DLL_LIB
  #define CORE_API __declspec(dllexport)
#else
  #define CORE_API __declspec(dllimport)
#endif

#include "BaseWindow.h"
#include "Event/Event.h"

class CORE_API MainWindow final : public BaseWindow<MainWindow>
{
public:
  MainWindow();
  PCWSTR ClassName() const override { return L"MainWindow"; }               // Get the class name of the main window.
  float GetDpiScale() const;                                                // Get the main window's DPI scale
  bool running{ false };                                                    // Running state of the window
  LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;  // Function to handle Windows messages

  // Event Declarations

  DECLARE_EVENT(MainWindow, OnWindowResizeEvent);
  OnWindowResizeEvent& OnWindowResize();

private:
  // No copies
  MainWindow(const MainWindow&) = delete;
  MainWindow(const MainWindow&&) = delete;
  MainWindow& operator=(const MainWindow&) = delete;
  MainWindow& operator=(const MainWindow&&) = delete;

  // Events
  OnWindowResizeEvent m_OnWindowResize;

  // Window DPI
  float m_dpiScale;
};