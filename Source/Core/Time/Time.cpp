#include "./Time.h"

#include <Core/Defines/Windows/windowsAPI.h>
#include <chrono>

static uint64 tick      = 0;
static double deltaTime = 0;
static double runtime   = 0;

double Time::getMachineFrequency() {
#ifdef OS_WINDOWS
  static double freq = [] {
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return static_cast<double>(freq.QuadPart);
  }();
  return freq;
#elif OS_LINUX
  // TODO
  // implement clock_gettime
#else
  // std c++
  return std::chrono::steady_clock::now().time_since_epoch().count();
#endif
}

uint64 Time::getNow() {
#ifdef OS_WINDOWS
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  return now.QuadPart;
#endif
}

uint64 Time::getEpoch() {
  // TODO
  //
  // For Windows implement GetSystemTimeAsFileTime or GetSystemTimePreciseAsFileTime
  //
  // For Linux / Mac implement clock_gettime(CLOCK_REALTIME)
  //

  using _sysclock = std::chrono::system_clock;

  _sysclock::time_point now    = _sysclock::now();
  _sysclock::duration   time   = now.time_since_epoch();
  uint64                msTime = std::chrono::duration_cast<std::chrono::microseconds>(time).count();
  return msTime;
}

double Time::getDeltaTime() {
  return deltaTime;
}

float Time::getRuntime() {
  return static_cast<float>(runtime);
}

void Time::onNewFrame() {
  tick = getNow();
}

void Time::onFrameEnd() {
  deltaTime = static_cast<double>(getNow() - tick) / getMachineFrequency();
  runtime  += deltaTime;
}