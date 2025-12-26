/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 
      + Input.h:
          Interface for system input management.

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#pragma once

class Application;

namespace op
{
  struct SInput
  {
    bool (initialize)(void* _pApplication_, void* _pWindowHandle_);
  };
}
