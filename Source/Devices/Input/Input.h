#pragma once

#include "Global/OptimEngineGlobal.h"
#include "Global/PlatformSystem.h"

class Application;

class Input final
{
public:
  static Input* Initalize(Application* _pApplication, void* _hWindowHandle);

private:
  static Input* sm_pSingleton;
};
