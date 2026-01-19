/* ======================================================================================
 *  IDirect3D11.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#include "Core/System/Application.h"
#include "Core/Time/Time.h"
#include "Core/Types/string.h"
#include "Core/Exception/exception.h"
#include "Core/Input/Input.h"

#include "DirectX11Graphics.h"
#include "Runtime/Direct3D11/IDirect3D11.h"
#include <cmath>

#include "Private/Resources/Dx11VertexBuffer.h"
#include "Private/Resources/Dx11IndexBuffer.h"
#include "Private/Resources/Dx11VertexShader.h"
#include "Private/Resources/Dx11PixelShader.h"

extern "C" DIRECTX11_API IDirect3D11* CreateDirect3D11Module() {
  return new IDirect3D11;
}

IDirect3D11::IDirect3D11() :
  m_hTargetWindow{ nullptr },
  m_pBase{ nullptr }
{}

IDirect3D11::~IDirect3D11() {}

void IDirect3D11::Initialize(void* _WindowHandle)
{
  m_hTargetWindow =_WindowHandle;

  m_pBase = new DirectX11Graphics{};

  if (m_pBase) { 
    m_pBase->initialize(reinterpret_cast<HWND>(m_hTargetWindow));
  }
}

/*
  Excecute Drawing Instructions
*/
void IDirect3D11::draw() {
  m_pBase->clearBuffer((34.0f / 255.0f), (38.0f / 255.0f), (92.0f / 255.0f), 1.0f);
  m_pBase->renderUpdate();
  m_pBase->presentBuffer();
}

void IDirect3D11::Clean() {}

void IDirect3D11::clearGraphicsResource(IGraphicsResource** ppResource) {
  delete *ppResource;
  *ppResource = nullptr;
}

/*
 * CREATE FUNCTIONS
*/

IVertexBuffer* IDirect3D11::createVertexBuffer(Vertex* pVertices, const uint32& bufferElementCount) {
  Dx11VertexBuffer* pResource = new Dx11VertexBuffer;
  pResource->createResources(pVertices, bufferElementCount);
  return pResource;
}

IIndexBuffer* IDirect3D11::createIndexBuffer(uint32* pIndices, const uint32& bufferElementCount) {
  Dx11IndexBuffer* pResource = new Dx11IndexBuffer;
  pResource->createResources(pIndices, bufferElementCount);
  return pResource;
}

IVertexShader* IDirect3D11::createVertexShader(const wchar* path) {
  Dx11VertexShader* pResource = new Dx11VertexShader;
  pResource->createResources(path);
  return pResource;
}

IPixelShader* IDirect3D11::createPixelShader(const wchar* path) {
  Dx11PixelShader* pResource = new Dx11PixelShader;
  pResource->createResources(path);
  return pResource;
}

/*
 * BINDING FUNCTIONS
*/

void IDirect3D11::bindVertexBuffer(IVertexBuffer* pVertexBuffer) {
  pVertexBuffer->bindResource();
}

void IDirect3D11::bindIndexBuffer(IIndexBuffer* pIndexBuffer) {
  pIndexBuffer->bindResource();
}

void IDirect3D11::bindVertexShader(IVertexShader* pVertexShader) {
  pVertexShader->bindResource();
}

void IDirect3D11::bindPixelShader(IPixelShader* pPixelShader) {
  pPixelShader->bindResource();
}

ID3D11Device* IDirect3D11::getDevicePtr() {
  return DirectX11Graphics::deviceRef;
}

ID3D11DeviceContext* IDirect3D11::getContextPtr() {
  return DirectX11Graphics::contextRef;
}