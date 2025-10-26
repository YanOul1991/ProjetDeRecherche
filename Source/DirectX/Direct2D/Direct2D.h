#pragma once

#ifdef DLL_LIB
  #define DIRECT2D_API __declspec(dllexport)
#else
  #define DIRECT2D_API __declspec(dllimport)
#endif

#include "Global/WindowsSettings.h"

class Direct2DRenderer final
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
  void draw();

private:
  ID2D1Factory*               pFactory;           // Factory interface
  ID2D1HwndRenderTarget*      pRenderTarget;      // Window render target
  ID2D1SolidColorBrush*       pSolidColorBrush;   // Solid brush ressource
};

extern "C" DIRECT2D_API Direct2DRenderer* CreateDirect2DRenderer();