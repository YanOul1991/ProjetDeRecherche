#pragma once

#include "Core/OptimEngineGlobal.h"
#include "Core/Rendering/IRenderingModule.h"

class OpDirect3d11Base;

class EXPORT OpDirect3D11Module final : IRenderingModule
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
  EXPORT OpDirect3D11Module* CreateDirect3D11Module();
}