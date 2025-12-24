/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      + OpDirect3d11.h:
          Entry point for D3D11 API implementations.

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#pragma once

#include "Core/OptimEngineGlobal.h"
#include "Core/Rendering/IRenderingModule.h"

#ifdef DLL_LIB  
  #define DIRECTX11API EXPORT
#else 
  #define DIRECTX11API IMPORT
#endif

class OpDirect3d11Base;

class DIRECTX11API OpDirect3D11Module final : IRenderingModule
{
public:
  OpDirect3D11Module();
  void Initialize(void* _WindowHandle) override;
  void draw() override;
  void Clean() override;

private:
  void* m_hTargetWindow;      // Target Window.
  OpDirect3d11Base* m_pBase;  
};

extern "C" {
  DIRECTX11API OpDirect3D11Module* CreateDirect3D11Module();
}