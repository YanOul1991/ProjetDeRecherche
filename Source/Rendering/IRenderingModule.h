#pragma once

#include "Global/OptimEngineGlobal.h"

class IRenderingModule
{
public:
  virtual ~IRenderingModule() {};
  virtual void Initialize(void* _WindowHandle) = 0;
  virtual void Draw() = 0;
};