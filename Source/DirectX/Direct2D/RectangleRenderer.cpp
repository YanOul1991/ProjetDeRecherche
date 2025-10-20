#include "DirectX/Direct2D/RectangleRenderer.h"
#include "DirectX/Direct2D/Renderer.h"

RectangleRenderer::RectangleRenderer(float _posX, float _posY, float _width, float _height) :
  positionX   { _posX }, 
  positionY   { _posY }, 
  width       { _width }, 
  height      { _height } 
{ }

void RectangleRenderer::Draw(ID2D1HwndRenderTarget* renderTarget)
{ }

