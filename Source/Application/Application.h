#pragma once

#ifdef DLL_LIB
#define APPLICATION_API __declspec(dllexport)
#else
#define APPLICATION_API __declspec(dllimport)
#endif

#include "Global/OptimEngineGlobal.h"
#include "Global/PlatformSystem.h"
#include "Rendering/IRenderingModule.h"
#include "Devices/Input/Input.h"

class EditorSystemWindow;
class Input;

class APPLICATION_API Application final
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
/*
  OS specifics
*/
#if defined(WINDOWS_OS)
  MSG m_windowsMsg{};
#endif

  bool m_shouldRun; // Should the application continue running/looping.
  EditorSystemWindow* m_pEditorOsWindow; //Pointer to the editor OS managed main window.
  IRenderingModule* m_pDirect2dModule;  // Pointer to the direct 2D module
  IRenderingModule* m_pGraphicsRenderingModule;
  Input* m_pInput;
};
