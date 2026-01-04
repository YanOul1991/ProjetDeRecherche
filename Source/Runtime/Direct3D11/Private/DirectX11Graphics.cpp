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

#include "Core/System/Application.h"
#include "Core/Types/string.h"
#include "Core/Exception/exception.h"
#include "Core/Input/Input.h"

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
{ 
  delete[] __t_vertexData;
  delete[] __t_indexData;
}

bool DirectX11Graphics::initialize(HWND _outputWindow)
{
  // Empty Memory
  ZeroMemory(&m_swapChainDesc, sizeof(m_swapChainDesc));

  // Set Swap chain description -> DXGI_MODE_DESC

  m_swapChainDesc.BufferDesc.Width              = 1920;
  m_swapChainDesc.BufferDesc.Height             = 1080;
  m_swapChainDesc.BufferDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
  m_swapChainDesc.BufferDesc.Scaling            = DXGI_MODE_SCALING_UNSPECIFIED;
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

  /* +++++++++++++++++++++++++++++++++++++++++++++++++++++++
      DEPTH BUFFER SETTING
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

  // Create Depth stencil state
  D3D11_DEPTH_STENCIL_DESC dsDesc = {};
  dsDesc.DepthEnable    = TRUE;
  dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  dsDesc.DepthFunc      = D3D11_COMPARISON_LESS;
  ComPtr<ID3D11DepthStencilState> pDepthStencilState;
  OPTIM_TRY_DX(m_pDevice->CreateDepthStencilState(&dsDesc, &pDepthStencilState));

  // Bind depth state | OM -> Output Merger 
  m_pDeviceContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1);

  // Create depth stencil texture
  ComPtr<ID3D11Texture2D> pDepthStencil;
  D3D11_TEXTURE2D_DESC depthDesc = {};
  depthDesc.Width       = 1920;
  depthDesc.Height      = 1080;
  depthDesc.MipLevels   = 1u;
  depthDesc.ArraySize   = 1u;
  depthDesc.Format      = DXGI_FORMAT_D32_FLOAT; // D32_FLOAT -> Depth 32bit floating point value
  depthDesc.Usage       = D3D11_USAGE_DEFAULT;
  depthDesc.BindFlags   = D3D11_BIND_DEPTH_STENCIL;

  depthDesc.SampleDesc.Count    = 1;
  depthDesc.SampleDesc.Quality  = 0;

  OPTIM_TRY_DX(m_pDevice->CreateTexture2D(&depthDesc, nullptr, &pDepthStencil));

  // Create view of depth stencil texture
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};

  descDSV.Format              = DXGI_FORMAT_D32_FLOAT;
  descDSV.ViewDimension       = D3D11_DSV_DIMENSION_TEXTURE2D;
  descDSV.Texture2D.MipSlice  = 0u;

  OPTIM_TRY_DX(m_pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &m_pDepthStencilView));

  // Bind depth stencil view
  m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

  /// ////////////////////// TESTING

  // Initializtion of vertex and index buffer testing values.
  __t_vertexData = new SGFXVertex[8] 
  {
    { -0.5f, -0.5f, -0.5f }, // 0  
    {  0.5f, -0.5f, -0.5f }, // 1  
    { -0.5f,  0.5f, -0.5f }, // 2  
    {  0.5f,  0.5f, -0.5f }, // 3  
    { -0.5f, -0.5f,  0.5f }, // 4  
    {  0.5f, -0.5f,  0.5f }, // 5  
    { -0.5f,  0.5f,  0.5f }, // 6  
    {  0.5f,  0.5f,  0.5f }  // 7
  };

  __t_VertexBuffer = VertexBuffer(__t_vertexData, sizeof(SGFXVertex) * 8);
  createBuffer(__t_VertexBuffer);

  // Index buffer initialization

  __t_indexData = new uint16[]
  {
    0, 2, 1,  2, 3, 1,
    1, 3, 5,  3, 7, 5,
    2, 6, 3,  3, 6, 7,
    4, 5, 7,  4, 7, 6,
    0, 4, 2,  2, 4, 6,
    0, 1, 4,  1, 5, 4
  };

  __t_IndexBuffer = IndexBuffer(__t_indexData, sizeof(uint16) * 36);
  createBuffer(__t_IndexBuffer);


  // Initialize constant Buffers
  __t_constBuffer = ConstantBuffer<DirectX::XMMATRIX>(DirectX::XMMatrixIdentity());
  createBuffer(__t_constBuffer);

  __t_constBufferColor = ConstantBuffer<ConstColors>();
  __t_constBufferColor = ConstColors
  {
    {
      { 1.0f, 0.0f, 1.0f }, // face 1
      { 1.0f, 0.0f, 0.0f }, // face 2
      { 0.0f, 1.0f, 0.0f }, // face 3
      { 0.0f, 0.0f, 1.0f }, // face 4
      { 1.0f, 1.0f, 0.0f }, // face 5
      { 0.0f, 1.0f, 1.0f }  // face 6
    }
  };

  createBuffer(__t_constBufferColor);

  return true;
}

void DirectX11Graphics::clearBuffer(float red, float green, float blue, float alpha)
{
  const float color[] = { red, green, blue, alpha };
  m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
  m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

/**************************
***************************
**************************/

void DirectX11Graphics::drawTriangle(float __offset, float __angle, float __posX, float __posY)
{
  HRESULT hr = S_OK;

  m_pDeviceContext->IASetVertexBuffers(0u, 1u, __t_VertexBuffer.m_comptr.GetAddressOf(), &__t_VertexBuffer.m_stride, &__t_VertexBuffer.m_offset);

  m_pDeviceContext->IASetIndexBuffer(__t_IndexBuffer.m_comptr.Get(), DXGI_FORMAT_R16_UINT, 0);

  float runtime = Application::getRuntime();

  __t_constBuffer.data =
  {
    DirectX::XMMatrixTranspose(
      DirectX::XMMatrixRotationY(runtime) *
      DirectX::XMMatrixRotationX(-runtime) *
      DirectX::XMMatrixTranslation(0, -sin(runtime * 2), 3.0f + sin(runtime * 2))*
      DirectX::XMMatrixPerspectiveLH(1.0f, 1080.0f / 1920.0f, 0.5f, 10.0f)
    )
  };

  m_pDeviceContext->UpdateSubresource(__t_constBuffer.m_comptr.Get(), 0, nullptr, &__t_constBuffer.data, 0, 0);
  m_pDeviceContext->VSSetConstantBuffers(0, 1, __t_constBuffer.m_comptr.GetAddressOf());

  m_pDeviceContext->PSSetConstantBuffers(0, 1, __t_constBufferColor.m_comptr.GetAddressOf());

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
    {"Position" , 0,  DXGI_FORMAT_R32G32B32_FLOAT,  0, 0,   D3D11_INPUT_PER_VERTEX_DATA,  0 }
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

  // Configure Viewport
  D3D11_VIEWPORT vp{};
  vp.Width      = 1920;
  vp.Height     = 1080;
  vp.MinDepth   = 0;

  vp.MaxDepth   = 1;
  vp.TopLeftX   = 0;
  vp.TopLeftY   = 0;

  m_pDeviceContext->RSSetViewports(1u, &vp);

  //m_pDeviceContext->Draw(std::size(vertices), 0u);
  m_pDeviceContext->DrawIndexed(__t_IndexBuffer.m_elementCount, 0u, 0u);
}

void DirectX11Graphics::renderUpdate()
{
  m_pDeviceContext->IASetVertexBuffers(0u, 1u, __t_VertexBuffer.m_comptr.GetAddressOf(), &__t_VertexBuffer.m_stride, &__t_VertexBuffer.m_offset);

  m_pDeviceContext->IASetIndexBuffer(__t_IndexBuffer.m_comptr.Get(), DXGI_FORMAT_R16_UINT, 0);

  float runtime = Application::getRuntime();

  __t_constBuffer.data =
  {
    DirectX::XMMatrixTranspose(
      DirectX::XMMatrixRotationY(runtime) *
      DirectX::XMMatrixRotationX(-runtime) *
      DirectX::XMMatrixTranslation(0, -sin(runtime * 2), 3.0f + sin(runtime * 2)) *
      DirectX::XMMatrixPerspectiveLH(1.0f, 1080.0f / 1920.0f, 0.5f, 10.0f)
    )
  };

  m_pDeviceContext->UpdateSubresource(__t_constBuffer.m_comptr.Get(), 0, nullptr, &__t_constBuffer.data, 0, 0);

  m_pDeviceContext->VSSetConstantBuffers(0, 1, __t_constBuffer.m_comptr.GetAddressOf());

  m_pDeviceContext->PSSetConstantBuffers(0, 1, __t_constBufferColor.m_comptr.GetAddressOf());
}

void DirectX11Graphics::createBuffer(DirectX11Buffer& buffer)
{
  HRESULT hr{ S_OK };
  OPTIM_TRY_DX(m_pDevice->CreateBuffer(&buffer.m_bufferDesc, &buffer.m_bufferSubres, &buffer.m_comptr));
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