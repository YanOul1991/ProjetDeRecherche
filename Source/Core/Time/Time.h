/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      ++ Time.h :
            Optim Engine Time API.

      ++ By:
            Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#pragma once

enum ETimeUnits
{
  sec           = (unsigned long)(1 * 1e0),
  milliseconds  = (unsigned long)(1 * 1e3),
  microseconds  = (unsigned long)(1 * 1e6),
  nanoseconds   = (unsigned long)(1 * 1e9)
};

class Time
{
public:
  static double getTime();
  static double getDeltaTime(ETimeUnits timeUnit);
private:
  static void tick();
};

namespace op::time
{
  struct STime
  {

  };

  inline void getDeltaTime()
  {
  }
}