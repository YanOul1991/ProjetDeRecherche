#pragma once

#ifdef DLL_LIB
  #define RENDERER_API __declspec(dllexport)
#else
  #define RENDERER_API __declspec(dllimport)
#endif

#include "Global/OptimEngineGlobal.h"

class Graphics;

class RENDERER_API Renderer
{
  friend class Graphics;
public:
  virtual ~Renderer() = default;
  virtual void Draw(ID2D1HwndRenderTarget* renderTarget) = 0;
};
