/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      + IDirect3D11.cpp
          Definitions for IDirect3D11.h file.

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */


#include "Core/Time/Time.h"
#include "Core/Types/string.h"
#include "Core/Exception/exception.h"
#include "DirectX11Graphics.h"
#include "Core/System/Application.h"
#include "Core/Input/Input.h"

#include "Runtime/Direct3D11/IDirect3D11.h"

#include <cmath>

extern "C" {
  DIRECTX11_API IDirect3D11* CreateDirect3D11Module()
  {
    return new IDirect3D11;
  }
}

IDirect3D11::IDirect3D11() :
  m_hTargetWindow{ nullptr },
  m_pBase{ nullptr }
{}

void IDirect3D11::Initialize(void* _WindowHandle)
{
  m_hTargetWindow =_WindowHandle;

  m_pBase = new DirectX11Graphics{};

  if (m_pBase) 
    m_pBase->initialize(reinterpret_cast<HWND>(m_hTargetWindow));
}

/*
  Excecute Drawing Instructions
*/
void IDirect3D11::draw()
{
  m_pBase->clearBuffer((34.0f / 255.0f), (38.0f / 255.0f), (92.0f / 255.0f), 1.0f);

  float angle = Application::getRuntime();
  float ndcX =  ((float)Mouse::posX / 1920) * 2 - 1.0f;
  float ndcY = -((float)Mouse::posY / 1080) * 2 + 1.0f;

  m_pBase->renderUpdate();
  m_pBase->drawTriangle(0, angle, 0, 0);
  m_pBase->presentBuffer();
}

void IDirect3D11::Clean() { }
