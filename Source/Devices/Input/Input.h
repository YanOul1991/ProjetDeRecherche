#pragma once

#include "Global/OptimEngineGlobal.h"
#include "Global/PlatformSystem.h"

class Application;
class Mouse;
class GameController;

class Input final
{
public:
  static Input* Initalize(Application* _pApplication, void* _hWindowHandle);
  static void Translate();

private:
  static Input* sm_pSingleton;
};
