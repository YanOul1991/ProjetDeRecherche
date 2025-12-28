/* ======================================================================================
 *  OpDirect3d11Base.cpp:
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

#include "OpDirect3d11Base.h"
#include "Core/Types/string.h"
#include "Core/Exception/exception.h"
#include <iostream>

#define OPTIM_TRY_DX(_PROC_) if(FAILED( hr = _PROC_)) throw op::Exception(__LINE__, __FILEW__, hr, TEXT("DirectX Error"), op::sys::windows::translateError(hr))


OpDirect3d11Base::OpDirect3d11Base() :
  m_pSwapChain{ nullptr },
  m_pDevice{ nullptr },
  m_pDeviceContext{ nullptr },
  m_pRenderTargetView{ nullptr },
  m_swapChainDesc{ 0 }
{}

OpDirect3d11Base::~OpDirect3d11Base() {
  //if (m_pDeviceContext)     m_pDeviceContext->Release();
  //if (m_pSwapChain)         m_pSwapChain->Release();
  //if (m_pRenderTargetView)  m_pRenderTargetView->Release();
  //if (m_pDevice)            m_pDevice->Release();
}

bool OpDirect3d11Base::Initialize(HWND _outputWindow)
{
  // Empty Memory
  ZeroMemory(&m_swapChainDesc, sizeof(m_swapChainDesc));

  // Set Swap chain description -> DXGI_MODE_DESC

  m_swapChainDesc.BufferDesc.Width              = 0;
  m_swapChainDesc.BufferDesc.Height             = 0;
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

  return SUCCEEDED(hr) == TRUE;
}

void OpDirect3d11Base::clearBuffer(float red, float green, float blue)
{
  const float color[] = { red, green, blue, 1.0f };
  m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
}

void OpDirect3d11Base::clearBuffer(const op::color::ColorRgb fillColor)
{
  float color[4]{fillColor.r, fillColor.g, fillColor.b, fillColor.a};
  m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
}

void OpDirect3d11Base::__testDrawTriangle()
{
  struct Vertex
  {
    float x;
    float y;
  };

  const Vertex vertices[] =
  {
    { 0.0f  , 0.5f    },
    { 0.5f  , -0.5f   },
    { -0.5f , -0.5f   },
    //{ 0.0f  , 0.5f    },
    //{ 0.5   , 1.0f    },
    //{ 1.0f  , 0.5f    },
    //{ 0.5f  , 0.5f    }
  };

  ComPtr<ID3D11Buffer>      _pVertexBuffer;
  D3D11_BUFFER_DESC         _bufferDesc{};
  D3D11_SUBRESOURCE_DATA    _subResData{};

  _bufferDesc.ByteWidth = sizeof(vertices);
  _bufferDesc.StructureByteStride = sizeof(Vertex);
  _bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  _bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  _bufferDesc.CPUAccessFlags = 0u;
  _bufferDesc.MiscFlags = 0u;

  _subResData.pSysMem = vertices;

  HRESULT hr = S_OK;

  hr = m_pDevice->CreateBuffer(&_bufferDesc, &_subResData, &_pVertexBuffer);
  if (FAILED(hr)) THROW_EXCEPTION(op::sys::windows::translateError(hr));
  UINT _stride = sizeof(Vertex);
  UINT _offset = 0u;
  m_pDeviceContext->IASetVertexBuffers(0u, 1u, _pVertexBuffer.GetAddressOf(), &_stride, &_offset);

  /* ===================================
      SHADERS LOADING
  =================================== */
  ComPtr<ID3DBlob> pBlob;

  //////////////////// PIXEL SHADER START
  ComPtr<ID3D11PixelShader> pPixelShader;
  hr = D3DReadFileToBlob(TEXT("bin/PixelShader.cso"), &pBlob);

  if (FAILED(hr))
    THROW_EXCEPTION(op::sys::windows::translateError(hr));

  hr = m_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

  if (FAILED(hr))
    THROW_EXCEPTION(op::sys::windows::translateError(hr));

  m_pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0);
  //////////////////// PIXEL SHADER END


  //////////////////// VERTEX SHADER START
  ComPtr<ID3D11VertexShader> pVertexShader;

  hr = D3DReadFileToBlob(TEXT("bin/VertexShader.cso"), &pBlob);
  if (FAILED(hr)) THROW_EXCEPTION(op::sys::windows::translateError(hr));

  hr = m_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
  if (FAILED(hr)) THROW_EXCEPTION(op::sys::windows::translateError(hr));
  //////////////////// VERTEX SHADER END

  // Input (vertx) layout
  ComPtr<ID3D11InputLayout> pInputLayout;
  const D3D11_INPUT_ELEMENT_DESC ied[] =
  {
    {"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
  };

  hr = m_pDevice->CreateInputLayout(ied, 1, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);
  if (FAILED(hr)) THROW_EXCEPTION(op::sys::windows::translateError(hr));


  /* ===================================
      INPUT BINDING
  =================================== */

  // Bind Input Layout
  m_pDeviceContext->IASetInputLayout(pInputLayout.Get());

  // Bind Vertex shader
  m_pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0);

  // Bind Render Target
  m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), nullptr);

  // Set Primitive topology to triangle list;
  m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  
  // Configure Viewport
  D3D11_VIEWPORT vp{};
  vp.Width    = 800;
  vp.Height   = 450;
  vp.MinDepth = 0;
  vp.MaxDepth = 1;
  vp.TopLeftX = (1920 / 2) - 400;
  vp.TopLeftY = (1080 / 2) - 275;

  m_pDeviceContext->RSSetViewports(1u, &vp);

  m_pDeviceContext->Draw(std::size(vertices), 0u);
}

void OpDirect3d11Base::presentBuffer()
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