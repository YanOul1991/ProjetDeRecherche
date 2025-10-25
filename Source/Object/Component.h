#pragma once

#ifdef DLL_LIB
#define COMPONENT_API __declspec(dllexport)
#else
#define COMPONENT_API __declspec(dllimport) 
#endif

#include "Global/OptimEngineGlobal.h"
#include "Object/Object.h"

class OpDirect2D;

class COMPONENT_API Component : public Object
{
public:
  Component();
  float positionX;
  float positionY;
  float radius;

  D2D1_COLOR_F color;
  friend class OpDirect2D;
};