/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      + OpDirect3d11.cpp
          Definitions for OpDirect3d11.h file.

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */


#include "Core/Time/Time.h"
#include "Core/Types/string.h"
#include "Core/Exception/exception.h"

#include "Runtime/Direct3D11/OpDirect3d11.h"
#include "OpDirect3d11Base.h"

#include <cmath>

extern "C" {
  DIRECTX11_API OpDirect3D11Module* CreateDirect3D11Module()
  {
    return new OpDirect3D11Module;
  }
}

OpDirect3D11Module::OpDirect3D11Module() :
  m_hTargetWindow{ nullptr },
  m_pBase{ nullptr }
{}

void OpDirect3D11Module::Initialize(void* _WindowHandle)
{
  m_hTargetWindow =_WindowHandle;

  m_pBase = new OpDirect3d11Base{};

  if (m_pBase) 
    m_pBase->Initialize(reinterpret_cast<HWND>(m_hTargetWindow));
}

/*
  Excecute Drawing Instructions
*/
void OpDirect3D11Module::draw()
{
  m_pBase->clearBuffer({ (34.0f / 255.0f), (38.0f / 255.0f), (92.0f / 255.0f) , 1.0f });
  m_pBase->presentBuffer();
}

void OpDirect3D11Module::Clean() { }
