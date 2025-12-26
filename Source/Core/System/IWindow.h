/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      + application.h :
          Base Class Interface for system managed windows. This class defines global
          system window functionalities.

      + By:
          Yanis Oulmane
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#pragma once

#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/OptimEngineGlobal.h"

#include <string>

class Application;

class IWindow final
{
public:
  int32     xPos;               // The window's top left corner's x position.
  int32     yPos;               // The window's top left corner's y position.
  int32     width;              // The window's horizontal size.
  int32     height;             // The window's vertical size.
  std::wstring   windowTitle;   // The window's Title | Application Name
  float     dpi;                // The window's DPI
  
  void* pSystemWindow;          // A handle to the window, must recast to OS specific types.
  void* getHandle() const;      // Returns the window's handle as a void pointer

  bool create(
    Application* _pApplication_, 
    const std::wstring& _title, 
    int32 _positionX, 
    int32 _positionY, 
    int32 _width, 
    int32 _height, 
    IWindow* _parentWindow
  );

  void display() const;
  void windowLoop();
  MSG m_msg;

protected:
  Application* m_pApplication;

private:
#if defined(OS_WINDOWS)
  static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
};