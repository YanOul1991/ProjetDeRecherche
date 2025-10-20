#pragma once

#ifdef DLL_LIB
  #define DIRECT2D_API __declspec(dllexport)
#else
  #define DIRECT2D_API __declspec(dllimport)
#endif

#include "Global/OptimEngineGlobal.h"
#include <vector>

class Renderer;
class MainWindow;

class DIRECT2D_API Graphics final
{
public:
  Graphics(MainWindow& hMainWindow);
  ~Graphics();

  HRESULT createGraphicsResources();
  void discardGraphicsResources();
  void resize();
  void draw();

private:
  const MainWindow& hMainWindow;

  ID2D1Factory*           pFactory;           // Factory interface
  ID2D1HwndRenderTarget*  pRenderTarget;      // Window render target
  ID2D1SolidColorBrush*   pSolidColorBrush;    // Solid brush ressource

  D2D1_ELLIPSE m_ellipse;
  D2D1_RECT_F m_rect;
};
