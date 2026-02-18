// Time.h

#pragma once

#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/OptimEngine.h"

#include <chrono>

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

/*
namespace op::time {
enum class ETimeUnits : uint64 {
  sec          = static_cast<uint64>(1 * 1e0),
  milliseconds = static_cast<uint64>(1 * 1e3),
  microseconds = static_cast<uint64>(1 * 1e6),
  nanoseconds  = static_cast<uint64>(1 * 1e9)
};

inline uint64 getMachineFrequency() {
#ifdef OS_WINDOWS
  static LARGE_INTEGER freq;
  static bool          init = false;

  if (!init) {
    QueryPerformanceFrequency(&freq);
    init = true;
  }
  return freq.QuadPart;
#endif // OS_WINDOWS
}

inline uint64 nowHighFreq() {
#ifdef OS_WINDOWS
  static LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  return now.QuadPart;
#endif // OS_WINDOWS
}

inline uint64 getEpoch() {
  using _sys_clock = std::chrono::system_clock;

  _sys_clock::time_point now     = _sys_clock::now();
  _sys_clock::duration   _time   = now.time_since_epoch();
  uint64                 _msTime = std::chrono::duration_cast<std::chrono::milliseconds>(_time).count();
  return _msTime;
}

inline double getDeltaTime(ETimeUnits timeUnit = ETimeUnits::milliseconds) {
#ifdef OS_WINDOWS
  static bool          init = false;
  static LARGE_INTEGER freq;
  static LARGE_INTEGER now{0};
  static LARGE_INTEGER last{0};

  if (!init) {
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&last);
    init = true;
    return 0.0;
  }

  QueryPerformanceCounter(&now);

  // return delta in seconds
  double delta  = (double(now.QuadPart - last.QuadPart) / freq.QuadPart) * static_cast<uint64>(timeUnit);
  last.QuadPart = now.QuadPart;
  return delta;
#endif
}
} // namespace op::time
*/