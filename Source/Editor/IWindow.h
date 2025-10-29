#pragma once

#include "Global/OptimEngineGlobal.h"
#include "Global/PlatformSystem.h"

#ifdef DLL_LIB
#define IWINDOW_API __declspec(dllexport)
#else
#define IWINDOW_API __declspec(dllimport)
#endif

class Application;

class IWINDOW_API IWindow
{
public:
  int32     xPos;         // The window's top left corner's x position.
  int32     yPos;         // The window's top left corner's y position.
  int32     width;        // The window's horizontal size.
  int32     height;       // The window's vertical size.
  wstring   windowTitle;  // The window's Title | Application Name
  float     dpi;          // The window's DPI
   
  // A handle to the window, must recast to OS specific types.
  void* m_pHandle;
  void* GetHandle() const;  // Returns the window's handle as a void pointer

  // Create a window, returns wether the creation was successfull for not.
  bool Create(const wstring& _title, int32 _positionX, int32 _positionY, int32 _width, int32 _height, IWindow* _parentWindow);

  // Tries to display the window, and returns if the operation has succeded.
  void Display() const;
protected:
  Application* m_pApplication;

private:
  /*
    Windows Specific Implementation.
  */
#if defined(WINDOWS_OS)
  static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
};