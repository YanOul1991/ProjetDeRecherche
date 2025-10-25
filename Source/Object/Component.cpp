#include "Component.h"
#include "Object.h"
#include "DirectX/Direct2D/Direct2D.h"
#include <sstream>

Component::Component() : 
  positionX { 200 },
  positionY { 200 },
  radius    { 50 },
  color     { D2D1::ColorF(0.0f, 1.0f, 1.0f)  }
{ 
  OpDirect2D* pDirect2D = OpDirect2D::Handle();
  if (pDirect2D) pDirect2D->comp = this;
}