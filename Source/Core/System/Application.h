/* ======================================================================================
 *  Application.h:
 *      This class serves as a central module that managed lifetimes of other Engine 
 *      implemented modules, and manages the life time of the application.
 * 
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"

class SystemWindow;
class IGraphicsModule;
class IWindow;

namespace op 
{
  struct SInput;
}

class CORE_API Application final
{
public:
  Application();
  ~Application();
  void ApplicationStart();
  void ApplicationLoop();
  void ApplicationQuit();
  void Quit();

  // Should the application continue running and looping.
  bool ShouldRun() const;
  static float getRuntime();

private:
  bool m_shouldRun; // Should the application continue running/looping.
  //SystemWindow* m_pEditorOsWindow; //Pointer to the editor OS managed main window.

  IWindow* m_pWindow;     // Pointer to os managed windows
  IGraphicsModule* m_pRenderModule;
  op::SInput* m_pInput;
  //MSG m_msg;

  static float m_runtime;
};

extern "C" {
  CORE_API Application* CreateApplicationProc();
}
