/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      ++ IRenderingModule.h :
            Abstraction interface for rendering API: Direct3D11

      ++ By:
            Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#pragma once

class IRenderingModule
{
public:
  virtual ~IRenderingModule() {};
  virtual void Initialize(void* _WindowHandle) = 0;
  virtual void Draw() = 0;
  virtual void Clean() = 0;
};