/* ======================================================================================
 *  IWindow.h:
 *      Base Class Interface for system managed windows. This class defines global 
 *      system window functionalities.
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Types/string.h"

#include <string>

class Application;

class IWindow final
{
public:
  int32     xPos;               // The window's top left corner's x position.
  int32     yPos;               // The window's top left corner's y position.
  int32     width;              // The window's horizontal size.
  int32     height;             // The window's vertical size.
  String    windowTitle;   // The window's Title | Application Name
  float     dpi;                // The window's DPI
  
  void* pSystemWindow;          // A handle to the window, must recast to OS specific types.
  void* getHandle() const;      // Returns the window's handle as a void pointer

  bool create(
    Application* _pApplication_,
    String&& name, 
    int32 _positionX, 
    int32 _positionY, 
    int32 _width, 
    int32 _height, 
    IWindow* _parentWindow
  );

  bool windowLoop();

protected:
  Application* m_pApplication;
};