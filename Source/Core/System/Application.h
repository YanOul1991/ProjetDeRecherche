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
#include "Core/Types/String.h"

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
  static float getDeltaTime();

private:
  static float m_runtime;
  static float m_deltaTime;

  SystemWindow* m_pSysWindow;
  IGraphicsModule* m_pRenderModule;
  op::SInput* m_pInput;

  bool m_shouldRun; 
};

extern "C" {
  CORE_API Application* CreateApplicationProc();
}
