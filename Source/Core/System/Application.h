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
  static float  getRuntime();
  static float  getDeltaTime();
  static void   getMainWindowSize(int32* pWidth, int32* pHeight);

private:
  static float m_runtime;
  static float m_deltaTime;
  bool m_shouldRun{false};

  // Event management functions

  void mangeWindowClickEvent(float posX, float posY, int32 buttonID);
  void manageSysWinMouseUp(float posX, float posY, int32 buttonID);
  void manageWindowResizeEvent(uint32 width, uint32 height);

};

extern "C" {
  CORE_API Application* CreateApplicationProc();
}

namespace OptimEditor {

  CORE_API void processFile(const char* cstrFilePath);

} // END - namespace OptimEditor