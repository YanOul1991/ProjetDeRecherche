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
class Component;

class DIRECT2D_API OpDirect2D final
{
public:
  static OpDirect2D* Handle();

  OpDirect2D();
  ~OpDirect2D();
  OpDirect2D(const OpDirect2D&) = delete;
  OpDirect2D(const OpDirect2D&&) = delete;
  OpDirect2D& operator=(const OpDirect2D&) = delete;
  OpDirect2D& operator=(const OpDirect2D&&) = delete;

  HRESULT createGraphicsResources();
  void discardGraphicsResources();
  void resize();
  void draw();

  Component* comp;

private:
  static OpDirect2D* pSelf;

  ID2D1Factory*           pFactory;           // Factory interface
  ID2D1HwndRenderTarget*  pRenderTarget;      // Window render target
  ID2D1SolidColorBrush*   pSolidColorBrush;   // Solid brush ressource

  D2D1_ELLIPSE m_ellipse;
  D2D1_RECT_F m_rect;

  
};
