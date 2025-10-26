#pragma once

#ifdef DLL_LIB
  #define COMPONENT_API __declspec(dllexport)
#else
  #define COMPONENT_API __declspec(dllimport) 
#endif

#include "Global/OptimEngineGlobal.h"
#include "Object.h"

class COMPONENT_API Component : public Object
{
public:
  Component();
  float positionX;
  float positionY;
  float radius;
};