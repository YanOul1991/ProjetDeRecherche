// Time.h

#pragma once

#include "Core/OptimEngine.h"

class CORE_API Time final
{
 public:
  static double getMachineFrequency();
  static uint64 getNow();
  static uint64 getEpoch();
  static double getDeltaTime();

  static constexpr uint64 UnitSecond   = static_cast<uint64>(1 * 1e0);
  static constexpr uint64 UnitMillisec = static_cast<uint64>(1 * 1e3);
  static constexpr uint64 UnitMicroSec = static_cast<uint64>(1 * 1e6);
  static constexpr uint64 UnitNanoSec  = static_cast<uint64>(1 * 1e9);

  static void onNewFrame();
  static void onFrameEnd();
};