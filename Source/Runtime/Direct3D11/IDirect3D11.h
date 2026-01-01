/* ======================================================================================
 *  IDirect3D11.h:
 *    Entry point for D3D11 API implementations.
 * 
 *  By: 
 *    Yanis Oulmane
 * 
 * --------------------------------------------------------------------------------------
 * 
 *  REFERENCES  
 *    References from Microsoft documentation for DIRECTX11 and DXGI.
 *      DXGI    - https://learn.microsoft.com/en-us/windows/win32/api/_direct3ddxgi/
 *      D3D11   - https://learn.microsoft.com/en-us/windows/win32/api/d3d11/
 * 
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Graphics/IGraphicsModule.h"

class DirectX11Graphics;

class IDirect3D11 final : IGraphicsModule
{
public:
  DIRECTX11_API IDirect3D11();
  DIRECTX11_API void Initialize(void* _WindowHandle) override;
  DIRECTX11_API void draw() override;
  DIRECTX11_API void Clean() override;

private:
  void* m_hTargetWindow;      // Target Window.
  DirectX11Graphics* m_pBase;  
};

extern "C" {
  DIRECTX11_API IDirect3D11* CreateDirect3D11Module();
}