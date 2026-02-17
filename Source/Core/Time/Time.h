// Time.h

#pragma once

#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/OptimEngine.h"

#include <chrono>

class Time final
{
};

namespace op::time {
/*
  Representation of different time values and their ratio
  relative to 1 second.
*/
enum class ETimeUnits : uint64 {
  sec          = (unsigned long)(1 * 1e0),
  milliseconds = (unsigned long)(1 * 1e3),
  microseconds = (unsigned long)(1 * 1e6),
  nanoseconds  = (unsigned long)(1 * 1e9)
};

/*
  Get the machine's frequency rate ticks per seconds. A frequency of 1,000Hz indiquates that
  the lowest time unit the hardware can measure is 1 / 1000 seconds, which is 1ms.
*/
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

/*
  Get the interval of time since the last time this function was called.
  The first time it it called it returns 0.

  @param timeUnit Takes as argument a unit of time, representing the desired scale.
  By default value is set to milliseconds.

  @return Time interval since last time the function was called.
*/
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