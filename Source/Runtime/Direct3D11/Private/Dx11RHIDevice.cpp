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

#include "Core/Object/Camera/Camera.h"
#include "Core/Object/Image/Image.h"

#include "Core/System/FileStream.h"
#include "Core/System/Application.h"

#include "Core/Types/string.h"

#include "Core/Exception/exception.h"

#include "Core/Input/Input.h"

#include "Private/Resources/Buffer/DirectX11Buffer.h"
#include "Private/Dx11RHIDevice.h"

#include <iostream>
#include <sstream>
#include <random>
#include <iomanip>

#include "Core/_Temporary/InterfaceImGui.h"

ID3D11Device*           Dx11RHIDevice::deviceRef{nullptr};
ID3D11DeviceContext*    Dx11RHIDevice::contextRef{nullptr};
ID3D11RenderTargetView* Dx11RHIDevice::renderTargetView{nullptr};

Dx11RHIDevice::Dx11RHIDevice()
{}

Dx11RHIDevice::~Dx11RHIDevice() 
{ }

bool Dx11RHIDevice::initialize(HWND param_outputWindow, Dx11RHI* param_pDx11RHI)
{
  OPTIM_WIN_COM_CHECK_START();

  DXGI_SWAP_CHAIN_DESC swapChainDesc{};
  ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

  swapChainDesc.BufferDesc.Width              = 1920;
  swapChainDesc.BufferDesc.Height             = 1080;
  swapChainDesc.BufferDesc.Format             = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferDesc.Scaling            = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
  swapChainDesc.BufferDesc.ScanlineOrdering   = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

  swapChainDesc.BufferDesc.RefreshRate.Numerator    = 0;
  swapChainDesc.BufferDesc.RefreshRate.Denominator  = 0;

  swapChainDesc.SampleDesc.Count    = 1;
  swapChainDesc.SampleDesc.Quality  = 0;

  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.BufferCount = 1;

  swapChainDesc.OutputWindow = param_outputWindow;

  swapChainDesc.Windowed = TRUE;

  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;

  swapChainDesc.Flags = 0;

  // D3D_DRIVER_TYPE_HARDWARE
  OPTIM_TRY_DX(D3D11CreateDeviceAndSwapChain(
    nullptr,
    D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
    nullptr,
    D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG,
    nullptr,
    0,
    D3D11_SDK_VERSION,
    &swapChainDesc,
    &m_pSwapChain,
    &m_pDevice,
    nullptr,
    &m_pContext
  ));

  /// ------------------------------------------------------------
  /// TEMPORARY - Set static fields for getting device and context
  /// ------------------------------------------------------------
  deviceRef         = m_pDevice.Get();
  contextRef        = m_pContext.Get();
  renderTargetView  = m_pRenderTargetView.Get();

  /// ------------------------------------------------------------
  /// Transform and view matrices constant buffer initalization
  /// ------------------------------------------------------------
  pDxRHI = param_pDx11RHI;
  constantBufferTransformView = pDxRHI->createConstantBuffer(sizeof(VSInputConstantBuffer));
  
  /// ------------------------------------------------------------
  /// Initalize ImGUI
  /// ------------------------------------------------------------
  InterfaceImGui::initDirectX(m_pDevice.Get(), m_pContext.Get());

  return true;
}

void Dx11RHIDevice::initRenderTargetView(uint32 param_newWidth, uint32 param_newHeight)
{
  OPTIM_CHECK_WIN_COM();
  ComPtr<ID3D11Texture2D> l_pBackbuffer;
  OPTIM_TRY_DX(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &l_pBackbuffer));
  OPTIM_TRY_DX(m_pDevice->CreateRenderTargetView(l_pBackbuffer.Get(), nullptr, &m_pRenderTargetView));

  // Configure Viewport
  D3D11_VIEWPORT vp{};
  vp.Width    = static_cast<float>(param_newWidth);
  vp.Height   = static_cast<float>(param_newHeight);
  vp.MinDepth = 0;
  vp.MaxDepth = 1;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;

  m_pContext->RSSetViewports(1u, &vp);
}

void Dx11RHIDevice::clearRenderTargetView() {
  m_pRenderTargetView.Reset();
}

void Dx11RHIDevice::clearBuffer(float red, float green, float blue, float alpha) const {
  const float color[4] = { red, green, blue, alpha };
  m_pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
}

void Dx11RHIDevice::renderUpdate()
{
  int32 l_windowWidth{};
  int32 l_windowHeight{};

  Application::getMainWindowSize(&l_windowWidth, &l_windowHeight);

  vsInputConstBufferData.transform = float4x4 {
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
  };

  float a = static_cast<float>(l_windowWidth) / static_cast<float>(l_windowHeight);
  constexpr float fov = Optim::Constants::pi / 3.0f;
  constexpr float n   = 0.1f;
  constexpr float f   = 1000.0f;

  float4x4 l_lookAt = {
    Camera::right.x,  Camera::up.x, -Camera::forward.x, 0,
    Camera::right.y,  Camera::up.y, -Camera::forward.y, 0,
    Camera::right.z,  Camera::up.z, -Camera::forward.z, 0,
    -dotProduct(Camera::right, Camera::position), -dotProduct(Camera::up, Camera::position), -dotProduct(-1 * Camera::forward, Camera::position), 1,
  };

  float yScale = 1.0f / (tan(fov / 2.0f));
  float4x4 perspectiveMatrix = float4x4 {
    yScale / a, 0, 0, 0,
    0, yScale, 0, 0,
    0, 0, f / (n - f), -1,
    0, 0, (n * f) / (n - f), 0
  };

  /*
  float a = static_cast<float>(l_windowWidth) / static_cast<float>(l_windowHeight);
  constexpr float fov = Optim::Constants::pi / 3.0f;
  constexpr float n   = 0.1f;
  constexpr float f   = 1000.0f;

  float4x4 l_lookAt = {
    Camera::right.x,  Camera::up.x, Camera::forward.x, 0,
    Camera::right.y,  Camera::up.y, Camera::forward.y, 0,
    Camera::right.z,  Camera::up.z, Camera::forward.z, 0,
    -dotProduct(Camera::right, Camera::position), -dotProduct(Camera::up, Camera::position), -dotProduct(Camera::forward, Camera::position), 1,
  };
  float yScale = 1.0f / (tan(fov / 2.0f));
  float4x4 perspectiveMatrix = float4x4 {
    yScale / a, 0, 0, 0,
    0, yScale, 0, 0,
    0, 0, f / (f - n), 1,
    0, 0, (-n * f) / (f - n), 0
  };

  */

  vsInputConstBufferData.lookAtMatrix       = l_lookAt.transpose();
  vsInputConstBufferData.perspectiveMatrix  = perspectiveMatrix.transpose();

  pDxRHI->updateConstantBuffer(&constantBufferTransformView, &vsInputConstBufferData);
  pDxRHI->cmdBindConstantBuffer(&constantBufferTransformView);
}

void Dx11RHIDevice::presentBuffer() const
{
  OPTIM_WIN_COM_CHECK_START();
  InterfaceImGui::update();

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