#include "Direct3D11/OpDirect3d11.h"
#include "OpDirect3d11Base.h"

extern "C" {
  OP_D3D11_API OpDirect3D11Module* CreateDirect3D11Module() 
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

  if (m_hTargetWindow != nullptr)
    std::cout << "\n ------- Direct3D11 Was Successfully initalized! ------- \n";

  m_pBase = new OpDirect3d11Base{};

  if (m_pBase)
  {
    std::cout << "Direct3d11 | OpDirect3d11BaseClass has been initalized.\n";
    m_pBase->Initialize(reinterpret_cast<HWND>(m_hTargetWindow));
  }
}

/*
  Excecute Drawing Instructions
*/
void OpDirect3D11Module::Draw()
{
  op::color::ColorHex color1{ 0x00ffffff };
  op::color::ColorHSV colorHsv{ op::color::RgbToHsv(op::color::ConvertColorHexToRgba(color1)) };

  // Clear the Buffer witg
  m_pBase->ClearBuffer(color1);
  m_pBase->EndFrame();
}

void OpDirect3D11Module::Clean()
{
  std::cout << "Direct3D11 module cleanning...\n";
}
