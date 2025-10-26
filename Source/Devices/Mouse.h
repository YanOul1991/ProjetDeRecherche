#pragma once

#ifdef DLL_LIB
  #define MOUSE_API __declspec(dllexport)
#else
  #define MOUSE_API __declspec(dllimport)
#endif

#include "Global/OptimEngineGlobal.h"

class MainWindow;

class MOUSE_API Mouse final
{
  friend class MainWindow;

private:
  static int32 m_posX;
  static int32 m_posY;

public:
  static int32 GetX();
  static int32 GetY();
};