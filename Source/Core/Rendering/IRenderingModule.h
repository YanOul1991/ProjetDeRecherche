/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      + IRenderingModule.h :
          Interface exposing abstracted functionalities 
          for rendering modules.

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#pragma once

#ifdef DLL_LIB
#define OPTIMENGINE_API __declspec(dllexport)
#else
#define OPTIMENGINE_API __declspec(dllimport)
#endif // DLL_LIB


class OPTIMENGINE_API IRenderingModule
{
public:
  virtual ~IRenderingModule() {};
  virtual void Initialize(void* _WindowHandle) = 0;
  virtual void draw() = 0;
  virtual void Clean() = 0;
};