#pragma once

#ifdef DLL_LIB
  #define ELLIPSE_RENDERER_API __declspec(dllexport)
#else
  #define ELLIPSE_RENDERER_API __declspec(dllimport)
#endif

#include "Global/OptimEngineGlobal.h"
#include "Renderer.h"

class MainWindow;
class Renderer;

class ELLIPSE_RENDERER_API EllipseRenderer : private Renderer {
  friend MainWindow;
public:
  EllipseRenderer();
  EllipseRenderer(float _position_x, float _position_y, float _radius);
  float position_x;
  float position_y;
  float radius;

private:
  void update();
};
