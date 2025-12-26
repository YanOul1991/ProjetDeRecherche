/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      + Application.h :
          This class serves as a central module that 
          managed lifetimes of other Engine implemented 
          modules, and manages the life time of the 
          application.

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#pragma once

#include "Core/OptimEngineGlobal.h"

class SystemWindow;
class IRenderingModule;
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

private:
  bool m_shouldRun; // Should the application continue running/looping.
  //SystemWindow* m_pEditorOsWindow; //Pointer to the editor OS managed main window.

  IWindow* m_pWindow;     // Pointer to os managed windows
  IRenderingModule* m_pDirect2dModule;  // Pointer to the direct 2D module
  IRenderingModule* m_pGraphicsRenderingModule;
  op::SInput* m_pInput;
  //MSG m_msg;

  float m_runtime{ 0 };
};

extern "C" {
  CORE_API Application* CreateApplicationProc();
}
