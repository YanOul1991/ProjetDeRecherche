/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      ++ Time.cc :
            Optim Engine Time API definitions

      ++ By:
            Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#pragma once

// Optim Engine API
#include "Core/OptimEngineGlobal.h"
#include "Time.h"

// System Library
#include <Windows.h>
#include <ctime>

double Time::getTime()
{
#ifdef OS_WINDOWS
  static LARGE_INTEGER freq;
  static bool init = QueryPerformanceFrequency(&freq);
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  return static_cast<double>(now.QuadPart) / freq.QuadPart;
#endif // OS_WINDOWS
}

double Time::getDeltaTime(ETimeUnits timeUnit = ETimeUnits::milliseconds)
{
  static LARGE_INTEGER freq;
  static LARGE_INTEGER now{ 0 };
  static LARGE_INTEGER last{ 0 };

  static bool init = QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&now);

  double delta = (double(now.QuadPart - last.QuadPart) / freq.QuadPart) * static_cast<uint32>(timeUnit);
  last.QuadPart = now.QuadPart;
  return delta;
}

void Time::tick()
{
  static LARGE_INTEGER freq;
  static LARGE_INTEGER now;
  static LARGE_INTEGER last;
  static bool init
}