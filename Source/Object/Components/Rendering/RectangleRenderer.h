#pragma once

#ifdef DLL_LIB
  #define RENDERING_API __declspec(dllexport)
#else
  #define RENDERING_API __declspec(dllimport)
#endif

#include "Global/OptimEngineGlobal.h"
#include "Renderer.h"

class Graphics;
class Renderer;

class RectangleRenderer : private Renderer
{
public:
  friend class Graphics;

  RectangleRenderer(float _posX, float _posY, float _width, float _height);
  float positionX{ 0 };
  float positionY{ 0 };
  float width{ 0 };
  float height{ 0 };
};
