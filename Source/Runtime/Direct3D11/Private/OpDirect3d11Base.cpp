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
 * 
====================================================================================== */

#include "OpDirect3d11Base.h"
#include "Core/Types/string.h"
#include "Core/Exception/exception.h"

OpDirect3d11Base::OpDirect3d11Base() :
  m_pSwapChain{ nullptr },
  m_pDevice{ nullptr },
  m_pDeviceContext{ nullptr },
  m_pRenderTargetView{ nullptr },
  m_swapChainDesc{ 0 }
{}

OpDirect3d11Base::~OpDirect3d11Base() {
  if (m_pDeviceContext)     m_pDeviceContext->Release();
  if (m_pSwapChain)         m_pSwapChain->Release();
  if (m_pRenderTargetView)  m_pRenderTargetView->Release();
  if (m_pDevice)            m_pDevice->Release();
}

/*
  @brief Initializes Direct3D11. create a Device and a swap chain.
  @param HWND _outputWindow - Window handle to the draw area.
  @return bool - Did D3D11 initialized successfully.
*/
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

  hr = D3D11CreateDeviceAndSwapChain(
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
  );

  if (FAILED(hr))
    throw op::Exception(__LINE__, __FILEW__, hr, TEXT("DirectX Error"), op::sys::windows::translateError(hr));
    //THROW_EXCEPTION(op::sys::windows::translateError(hr));

  ID3D11Resource* pBackBuffer{ 0 };

  hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), RCAST(void**, &pBackBuffer));

  if (FAILED(hr)) 
    THROW_EXCEPTION(op::sys::windows::translateError(hr));

  if (pBackBuffer == nullptr) throw op::Exception(__LINE__, __FILEW__, TEXT("Pointer to back buffer is nullptr"));

  hr = m_pDevice->CreateRenderTargetView(
    pBackBuffer,
    nullptr,
    &m_pRenderTargetView
  );

  if (FAILED(hr)) THROW_EXCEPTION(op::sys::windows::translateError(hr));

  pBackBuffer->Release();

  return SUCCEEDED(hr) == TRUE;
}

void OpDirect3d11Base::clearBuffer(float red, float green, float blue)
{
  const float color[] = { red, green, blue, 1.0f };
  m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
}

//void OpDirect3d11Base::clearBuffer(const op::color::ColorHex fillColor)
//{
//  float color[4]{};
//  op::color::setHexArray(color, fillColor);
//  m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
//}

void OpDirect3d11Base::clearBuffer(const op::color::ColorRgb fillColor)
{
  float color[4]{fillColor.r, fillColor.g, fillColor.b, fillColor.a};
  m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
}

void OpDirect3d11Base::presentBuffer()
{
  HRESULT hr{ S_OK };
  hr = m_pSwapChain->Present(1u, 0u);

  if (FAILED(hr))
  {
    if (hr == DXGI_ERROR_DEVICE_REMOVED)
      THROW_EXCEPTION(op::sys::windows::translateError(m_pDevice->GetDeviceRemovedReason()));
    else
      THROW_EXCEPTION(op::sys::windows::translateError(hr));
  }
}