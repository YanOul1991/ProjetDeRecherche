#pragma once

#ifdef DLL_LIB
  #define DIRECT2D_API __declspec(dllexport)
#else
  #define DIRECT2D_API __declspec(dllimport)
#endif

/*
  Direct2D is Windows only interface, so include
  Windows os settings by default.
*/
#include "Global/OptimEngineGlobal.h"
#include "Rendering/IRenderingModule.h"
#include "d2d1.h"
#pragma comment(lib, "d2d1.lib")

class Direct2DRenderer : public IRenderingModule
{
public:
  Direct2DRenderer();
  ~Direct2DRenderer();
  Direct2DRenderer(const Direct2DRenderer&) = delete;
  Direct2DRenderer(const Direct2DRenderer&&) = delete;
  Direct2DRenderer& operator=(const Direct2DRenderer&) = delete;
  Direct2DRenderer& operator=(const Direct2DRenderer&&) = delete;

  HRESULT createGraphicsResources();
  void discardGraphicsResources();
  void resize();

  /*
    Overrides 
  */

  void Initialize(void* _targetWindow) override;
  void Draw() override;
  void Clean() override;

private:
  HWND m_hTargetWindow;
  ID2D1HwndRenderTarget*      pRenderTarget;      // Window render target
  ID2D1Factory*               pFactory;           // Factory interface
  ID2D1SolidColorBrush*       pSolidColorBrush;   // Solid brush ressource
};

// Factory Function.
extern "C" DIRECT2D_API IRenderingModule* CreateDirect2DRenderer();