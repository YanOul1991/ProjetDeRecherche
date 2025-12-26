/* ======================================================================================
 *  OpDirect3d11.h:
 *    Entry point for D3D11 API implementations.
 * 
 *  By: 
 *    Yanis Oulmane
 * 
 * --------------------------------------------------------------------------------------
 * 
 *  REFERENCES  
 *    References from Microsoft documentation for DIRECTX11 and DXGI.
 *      DXGI - https://learn.microsoft.com/en-us/windows/win32/api/_direct3ddxgi/#interfaces
 * 
====================================================================================== */

#pragma once

#include "Core/OptimEngineGlobal.h"
#include "Core/Rendering/IRenderingModule.h"

class OpDirect3d11Base;

class DIRECTX11_API OpDirect3D11Module final : IRenderingModule
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
  DIRECTX11_API OpDirect3D11Module* CreateDirect3D11Module();
}