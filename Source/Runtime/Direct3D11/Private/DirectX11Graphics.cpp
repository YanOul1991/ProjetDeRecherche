/* ======================================================================================
 *  DirectX11Graphics.cpp:
 * 
 *  By:
 *    Yanis Oulmane
 *
 * --------------------------------------------------------------------------------------
 *  Sources for detailed description of important concepts.
 * 
 *    DXGI_SWAP_CHAIN_DESC            [ https://learn.microsoft.com/en-us/windows/win32/api/dxgi/ns-dxgi-dxgi_swap_chain_desc ]
 *    D3D11CreateDeviceAndSwapChain() [ https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdeviceandswapchain ]
 *    D3D11_BUFFER_DESC               [ https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_buffer_desc ]
 *    Primitive Topologies            [ https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-primitive-topologies ]
 * 
====================================================================================== */

#include "Core/Types/string.h"
#include "Core/Exception/exception.h"
#include "Core/System/Application.h"
#include "DirectX11Graphics.h"
#include <iostream>

#define OPTIM_TRY_DX(_PROC_) if(FAILED( hr = _PROC_)) throw Exception(__LINE__, __FILEW__, hr, TEXT("DirectX Error"), op::sys::windows::translateError(hr))

DirectX11Graphics::DirectX11Graphics() :
  m_pSwapChain        { nullptr },
  m_pDevice           { nullptr },
  m_pDeviceContext    { nullptr },
  m_pRenderTargetView { nullptr },
  m_swapChainDesc     { 0 }
{ }

DirectX11Graphics::~DirectX11Graphics() 
{ }

bool DirectX11Graphics::initialize(HWND _outputWindow)
{
  // Empty Memory
  ZeroMemory(&m_swapChainDesc, sizeof(m_swapChainDesc));

  // Set Swap chain description -> DXGI_MODE_DESC

  m_swapChainDesc.BufferDesc.Width              = 0;
  m_swapChainDesc.BufferDesc.Height             = 0;
  m_swapChainDesc.BufferDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
  m_swapChainDesc.BufferDesc.Scaling            = DXGI_MODE_SCALING_CENTERED;
  m_swapChainDesc.BufferDesc.ScanlineOrdering   = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

  m_swapChainDesc.BufferDesc.RefreshRate.Numerator    = 0;
  m_swapChainDesc.BufferDesc.RefreshRate.Denominator  = 0;

  m_swapChainDesc.SampleDesc.Count = 1;
  m_swapChainDesc.SampleDesc.Quality = 0;

  m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  m_swapChainDesc.BufferCount = 1;

  m_swapChainDesc.OutputWindow = _outputWindow;

  m_swapChainDesc.Windowed = TRUE;

  m_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  m_swapChainDesc.Flags = 0;

  HRESULT hr{ S_OK };

  // D3D_DRIVER_TYPE_HARDWARE
  OPTIM_TRY_DX(D3D11CreateDeviceAndSwapChain(
    nullptr,
    D3D_DRIVER_TYPE_HARDWARE,
    nullptr,
    D3D11_CREATE_DEVICE_DEBUG,
    nullptr,
    0,
    D3D11_SDK_VERSION,
    &m_swapChainDesc,
    &m_pSwapChain,
    &m_pDevice,
    nullptr,
    &m_pDeviceContext
  ));

  // Get pointer to backbuffer
  ComPtr<ID3D11Resource> _pBackbuffer;
  OPTIM_TRY_DX(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &_pBackbuffer));
  OPTIM_TRY_DX(m_pDevice->CreateRenderTargetView(_pBackbuffer.Get(), nullptr, &m_pRenderTargetView));

  return SUCCEEDED(hr) == TRUE;
}

void DirectX11Graphics::clearBuffer(float red, float green, float blue)
{
  const float color[] = { red, green, blue, 1.0f };
  m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
}

void DirectX11Graphics::clearBuffer(const op::color::ColorRgb fillColor)
{
  float color[4]{fillColor.r, fillColor.g, fillColor.b, fillColor.a};
  m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
}

void DirectX11Graphics::drawTriangle()
{
  HRESULT hr = S_OK;
  struct Vertex
  {
    struct
    {
      float x;
      float y;
    } Position;

    struct
    {
      uint8 r;
      uint8 g;
      uint8 b;
      uint8 a;
    } Color;
  };

  Vertex vertices[] =
  {
    //{ -1.0f , -1.0f ,  255, 255, 255, 0 },
    //{ -1.0f , 1.0f ,  255, 255, 255, 0 },
    //{ -0.7f , -1.0f ,  255, 255, 255, 0 },
    //{ -0.7f , 1.0f ,  255, 255, 255, 0 },

    { -0.25f, -0.25f, 128, 128, 000, 000 },
    { -0.25f, 0.25f, 128, 128, 000, 000 },
    { 0.25f, -0.25f, 128, 128, 000, 000 },
    { 0.25f, 0.25f, 128, 128, 000, 000 },
  };

  /// ///////////////////// CREATE VERTEX BUFFER

  ComPtr<ID3D11Buffer>      _pVertexBuffer;
  D3D11_BUFFER_DESC         _bufferDesc{};
  D3D11_SUBRESOURCE_DATA    _subResData{};

  _bufferDesc.ByteWidth           = sizeof(vertices);
  _bufferDesc.StructureByteStride = sizeof(Vertex);
  _bufferDesc.Usage               = D3D11_USAGE_DEFAULT;
  _bufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
  _bufferDesc.CPUAccessFlags      = 0u;
  _bufferDesc.MiscFlags           = 0u;

  _subResData.pSysMem = vertices;

  OPTIM_TRY_DX(m_pDevice->CreateBuffer(&_bufferDesc, &_subResData, &_pVertexBuffer));

  UINT _stride = sizeof(Vertex);
  UINT _offset = 0u;

  m_pDeviceContext->IASetVertexBuffers(0u, 1u, _pVertexBuffer.GetAddressOf(), &_stride, &_offset);

  /// ////////////////////// CREATE INDEX BUFFER

  uint16 indices[] =
  {
    0, 1, 2,
    2, 1, 3,
    //4, 5, 6,
    //6, 5, 7
  };

  ComPtr<ID3D11Buffer>      pIndexBuffer;
  D3D11_BUFFER_DESC         indexBufferDesc{};
  D3D11_SUBRESOURCE_DATA    indexSubResData{};

  indexBufferDesc.ByteWidth           = sizeof(indices);
  indexBufferDesc.StructureByteStride = sizeof(uint16);
  indexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
  indexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
  indexBufferDesc.CPUAccessFlags      = 0u;
  indexBufferDesc.MiscFlags           = 0u;
  
  indexSubResData.pSysMem = indices;

  OPTIM_TRY_DX(m_pDevice->CreateBuffer(&indexBufferDesc, &indexSubResData, &pIndexBuffer));

  m_pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

  /// ////////////////////// CREATE CONSTANT BUFFER

  struct ConstantBuffer
  {
    DirectX::XMMATRIX transform;
  };

  float angle = Application::getRuntime();

  ConstantBuffer cb =
  {
    //{
    //  (9.0f / 16.0f) * cos(angle), sin(angle), 0.0f, 0.0f,
    //  (9.0f / 16.0f) * -sin(angle), cos(angle), 0.0f, 0.0f,
    //  0.0f, 0.0f, 1.0f, 0.0f,
    //  0.0f, 0.0f, 0.0f, 1.0f,
    //}
    {
      DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationZ(angle) * DirectX::XMMatrixScaling(3.0f / 4.0f, 1.0f, 1.0f))
    }
  };

  ComPtr<ID3D11Buffer>    pConstBuffer;
  D3D11_BUFFER_DESC       constBufferDesc{};
  D3D11_SUBRESOURCE_DATA  constBufferSubResData{};

  constBufferDesc.ByteWidth = sizeof(cb);
  constBufferDesc.StructureByteStride = 0;
  constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  constBufferDesc.MiscFlags = 0u;
  
  constBufferSubResData.pSysMem = &cb;
  OPTIM_TRY_DX(m_pDevice->CreateBuffer(&constBufferDesc, &constBufferSubResData, &pConstBuffer));

  m_pDeviceContext->VSSetConstantBuffers(0, 1, pConstBuffer.GetAddressOf());

  /* ===================================
      SHADERS LOADING
  =================================== */
  ComPtr<ID3DBlob> pBlob;

  //////////////////// PIXEL SHADER START
  ComPtr<ID3D11PixelShader> pPixelShader;

  OPTIM_TRY_DX(D3DReadFileToBlob(TEXT("bin/PixelShader.cso"), &pBlob));

  OPTIM_TRY_DX(m_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

  m_pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0);
  //////////////////// PIXEL SHADER END


  //////////////////// VERTEX SHADER START
  ComPtr<ID3D11VertexShader> pVertexShader;

  OPTIM_TRY_DX(D3DReadFileToBlob(TEXT("bin/VertexShader.cso"), &pBlob));

  OPTIM_TRY_DX(m_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
  //////////////////// VERTEX SHADER END

  // Input (vertx) layout
  ComPtr<ID3D11InputLayout> pInputLayout;
  const D3D11_INPUT_ELEMENT_DESC ied[] =
  {
    {"Position" , 0,  DXGI_FORMAT_R32G32_FLOAT,   0, 0,   D3D11_INPUT_PER_VERTEX_DATA,  0 },
    {"Color"    , 0,  DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u,  D3D11_INPUT_PER_VERTEX_DATA,  0 }
  };

  OPTIM_TRY_DX(m_pDevice->CreateInputLayout(ied, std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));

  /* ===================================
      INPUT BINDING
  =================================== */
  // Set Primitive topology to triangle list;
  m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  // Bind Input Layout
  m_pDeviceContext->IASetInputLayout(pInputLayout.Get());

  // Bind Vertex shader
  m_pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0);

  // Bind Render Target
  m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), nullptr);

  
  // Configure Viewport
  D3D11_VIEWPORT vp{};
  vp.Width    = 800;
  vp.Height   = 450;
  vp.MinDepth = 0;
  vp.MaxDepth = 1;
  vp.TopLeftX = (1920 / 2) - 400;
  vp.TopLeftY = (1080 / 2) - 275;

  m_pDeviceContext->RSSetViewports(1u, &vp);

  //m_pDeviceContext->Draw(std::size(vertices), 0u);
  m_pDeviceContext->DrawIndexed(std::size(indices), 0u, 0u);
}

void DirectX11Graphics::presentBuffer()
{
  HRESULT hr{ S_OK };
  hr = m_pSwapChain->Present(1u, 0u);

  if (FAILED(hr)) {
    if (hr == DXGI_ERROR_DEVICE_REMOVED) {
      THROW_EXCEPTION(op::sys::windows::translateError(m_pDevice->GetDeviceRemovedReason()));
    }
    else {
      THROW_EXCEPTION(op::sys::windows::translateError(hr));
    }
  }
}