/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      ++ Input.h :
            Optim Engine Input API

      ++ By: 
            Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#pragma once

class Application;

namespace op
{
  struct SInput
  {
    bool (initialize)(void* _pApplication_, void* _pWindowHandle_);
    //void (translate)();
  };
}
