/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      + IRenderingModule.h :
          Interface exposing abstracted functionalities 
          for rendering modules.

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#pragma once

class IRenderingModule
{
public:
  virtual ~IRenderingModule() {};
  virtual void Initialize(void* _WindowHandle) = 0;
  virtual void draw() = 0;
  virtual void Clean() = 0;
};