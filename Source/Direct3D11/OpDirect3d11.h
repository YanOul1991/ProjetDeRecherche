#pragma once

#include "Global/OptimEngineGlobal.h"
#include "Rendering/IRenderingModule.h"

#if defined(OP_DIRECT3D11_MODULE)
#define OP_D3D11_API __declspec(dllexport)
#else
#define OP_D3D11_API  __declspec(dllimport)
#endif

class OpDirect3d11Base;

/*
  Direct3D11 Class Interface
*/
class OP_D3D11_API OpDirect3D11Module final : IRenderingModule
{
public:
  OpDirect3D11Module();
  void Initialize(void* _WindowHandle) override;
  void Draw() override;
  void Clean() override;

private:
  void* m_hTargetWindow;      // Target Window.
  OpDirect3d11Base* m_pBase;  
};

extern "C" {
  OP_D3D11_API OpDirect3D11Module* CreateDirect3D11Module();
}