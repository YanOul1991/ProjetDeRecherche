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

Dx11RHIDevice::Dx11RHIDevice() :
  m_pSwapChain        { nullptr },
  m_pDevice           { nullptr },
  m_pContext          { nullptr },
  m_pRenderTargetView { nullptr }
{}

Dx11RHIDevice::~Dx11RHIDevice() 
{ }

bool Dx11RHIDevice::initialize(HWND _outputWindow, Dx11RHI* param_pDx11RHI)
{

  DXGI_SWAP_CHAIN_DESC swapChainDesc{};
  ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

  swapChainDesc.BufferDesc.Width              = 1920;
  swapChainDesc.BufferDesc.Height             = 1080;
  swapChainDesc.BufferDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferDesc.Scaling            = DXGI_MODE_SCALING_UNSPECIFIED;
  swapChainDesc.BufferDesc.ScanlineOrdering   = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

  swapChainDesc.BufferDesc.RefreshRate.Numerator    = 0;
  swapChainDesc.BufferDesc.RefreshRate.Denominator  = 0;

  swapChainDesc.SampleDesc.Count = 1;
  swapChainDesc.SampleDesc.Quality = 0;

  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.BufferCount = 1;

  swapChainDesc.OutputWindow = _outputWindow;

  swapChainDesc.Windowed = TRUE;

  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  swapChainDesc.Flags = 0;

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
    &swapChainDesc,
    &m_pSwapChain,
    &m_pDevice,
    nullptr,
    &m_pContext
  ));

  // Get pointer to backbuffer
  ComPtr<ID3D11Resource> _pBackbuffer;
  OPTIM_TRY_DX(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &_pBackbuffer));
  OPTIM_TRY_DX(m_pDevice->CreateRenderTargetView(_pBackbuffer.Get(), nullptr, &m_pRenderTargetView));

  // TEMPORARY - Set static fields for getting device and context
  deviceRef         = m_pDevice.Get();
  contextRef        = m_pContext.Get();
  renderTargetView  = m_pRenderTargetView.Get();

  /// ---------------------------------
  /// Transform and view matrices constant buffer initalization
  /// ---------------------------------
  pDxRHI = param_pDx11RHI;
  constantBufferTransformView = pDxRHI->createConstantBuffer(sizeof(VSInputConstantBuffer));
  
  //__t_constBuffer.init(m_pDevice.Get());

  InterfaceImGui::initDirectX(m_pDevice.Get(), m_pContext.Get());

  //matrix_perspective =  DirectX::XMMatrixPerspectiveRH(1.0f, 1080.0f / 1920.0f, 1.0f, 1000.0f);
  matrix_perspective =  DirectX::XMMatrixPerspectiveFovRH(mathConst::PI / 3.0f, 16.0f / 9.0f, 0.1f, 1000.0f);

  return true;
}

void Dx11RHIDevice::clearBuffer(float red, float green, float blue, float alpha) const {
  const float color[4] = { 
    red, 
    green, 
    blue, 
    alpha 
  };

  m_pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
}

void Dx11RHIDevice::renderUpdate()
{
  DirectX::XMFLOAT3 position = {
    Camera::position.x,
    Camera::position.y,
    Camera::position.z
  };
  DirectX::XMFLOAT3 forward = {
    Camera::forward.x,
    Camera::forward.y,
    Camera::forward.z
  };
  DirectX::XMFLOAT3 up = {
    Camera::up.x,
    Camera::up.y,
    Camera::up.z
  };

  matrix_camera = DirectX::XMMatrixLookToRH(
    DirectX::XMLoadFloat3(&position),
    DirectX::XMLoadFloat3(&forward),
    DirectX::XMLoadFloat3(&up)
  );

  DirectX::XMFLOAT4X4 mat{};
  DirectX::XMStoreFloat4x4(&mat, matrix_perspective);

  //printf("DX camera view matrix: \n");
  //for (int i = 0; i < 4; i++) {
  //  printf("|%2.7f, %2.7f, %2.7f, %2.7f|\n", mat.m[i][0],  mat.m[i][1],  mat.m[i][2],  mat.m[i][3]);
  //}

  //printf("LookAt Matrix: \n");
  float4x4 l_lookAt = {
    Camera::right.x,  Camera::up.x, -Camera::forward.x, 0,
    Camera::right.y,  Camera::up.y, -Camera::forward.y, 0,
    Camera::right.z,  Camera::up.z, -Camera::forward.z, 0,
    -dotProduct(Camera::right, Camera::position), -dotProduct(Camera::up, Camera::position), -dotProduct(-1 * Camera::forward, Camera::position), 1,
  };

  //l_lookAt.printMatrix();

  //m_pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);s

  //DirectX::XMStoreFloat4x4(&vsInputConstBufferData.transform, DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0, 0, 0)));
  constexpr float pX{0};
  constexpr float pY{0};
  constexpr float pZ{0};

  vsInputConstBufferData.transform = float4x4 {
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
  };

  //float4x4 l_lookAt = {
  //  Camera::right.x,    Camera::right.y,    Camera::right.z,   -dotProduct(Camera::right,   Camera::position),
  //  Camera::up.x,       Camera::up.y,       Camera::up.z,      -dotProduct(Camera::up,      Camera::position),
  //  -Camera::forward.x,  -Camera::forward.y,  -Camera::forward.z, dotProduct(Camera::forward, Camera::position),
  //  0, 0, 0, 1,
  //};

  vsInputConstBufferData.lookAtMatrix = l_lookAt.transpose();

  constexpr float a   = 1920.0f / 1080.0f;
  constexpr float fov = mathConst::PI / 3.0f;
  constexpr float n   = 0.1f;
  constexpr float f   = 1000.0f;

  float yScale = 1.0f / (tan(fov / 2.0f));

  float4x4 perspectiveMatrix = float4x4 {
    yScale / a, 0, 0, 0,
    0, yScale, 0, 0,
    0, 0, f / (n - f), -1,
    0, 0, (n * f) / (n - f), 0
  };

  vsInputConstBufferData.perspectiveMatrix = perspectiveMatrix.transpose();

  //printf("Transform Matrix: \n");
  //vsInputConstBufferData.transform.printMatrix();
  //vsInputConstBufferData.lookAtMatrix.printMatrix();
  //printf("Perspective Matrix: \n");
  //vsInputConstBufferData.perspectiveMatrix.printMatrix();

  // Update constant buffer for camera view and for now also the transform for the mesh object as its always assumed to be 0
  //DirectX::XMStoreFloat4x4(&vsInputConstBufferData.lookAtMatrix, DirectX::XMMatrixTranspose(matrix_camera));
  //DirectX::XMStoreFloat4x4(&vsInputConstBufferData.perspectiveMatrix, DirectX::XMMatrixTranspose(matrix_perspective));
  pDxRHI->updateConstantBuffer(&constantBufferTransformView, &vsInputConstBufferData);
  pDxRHI->cmdBindConstantBuffer(&constantBufferTransformView);

  // Configure Viewport
  D3D11_VIEWPORT vp{};
  vp.Width    = 1920;
  vp.Height   = 1080;
  vp.MinDepth = 0;

  vp.MaxDepth = 1;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;

  m_pContext->RSSetViewports(1u, &vp);
}

void Dx11RHIDevice::presentBuffer() const
{
  InterfaceImGui::update();

  HRESULT hr{ S_OK };
  hr = m_pSwapChain->Present(0u, 0u);

  if (FAILED(hr)) {
    if (hr == DXGI_ERROR_DEVICE_REMOVED) {
      THROW_EXCEPTION(op::sys::windows::translateError(m_pDevice->GetDeviceRemovedReason()));
    }
    else {
      THROW_EXCEPTION(op::sys::windows::translateError(hr));
    }
  }
}