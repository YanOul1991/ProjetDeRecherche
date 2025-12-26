/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      + IRenderingModule.h :
          Interface exposing abstracted functionalities 
          for rendering modules.

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#pragma once

#include "Core/OptimEngineGlobal.h"

class IRenderingModule
{
public:
  CORE_API virtual ~IRenderingModule() {};
  CORE_API virtual void Initialize(void* _WindowHandle) = 0;
  CORE_API virtual void draw() = 0;
  CORE_API virtual void Clean() = 0;
};