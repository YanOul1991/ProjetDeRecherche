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

#include "Core/Object/Image/Image.h"
#include "Core/System/FileStream.h"
#include "Core/System/Application.h"
#include "Core/Types/string.h"
#include "Core/Exception/exception.h"
#include "Core/Input/Input.h"

#include "DirectX11Graphics.h"
#include "Private/Resources/Buffer/DirectX11Buffer.h"

#include "Core/Object/Camera/Camera.h"

#include <iostream>
#include <sstream>
#include <random>
#include <iomanip>

#include "Core/_Temporary/InterfaceImGui.h"

ID3D11Device*         DirectX11Graphics::deviceRef{nullptr};
ID3D11DeviceContext*  DirectX11Graphics::contextRef{nullptr};

DirectX11Graphics::DirectX11Graphics() :
  m_pSwapChain        { nullptr },
  m_pDevice           { nullptr },
  m_pContext          { nullptr },
  m_pRenderTargetView { nullptr }
{ }

DirectX11Graphics::~DirectX11Graphics() 
{ }

bool DirectX11Graphics::initialize(HWND _outputWindow)
{
  DXGI_SWAP_CHAIN_DESC swapChainDesc{};
  // Empty Memory
  ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

  // Set Swap chain description -> DXGI_MODE_DESC

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

  // TEMPORARY - Set static fields for getting device and context
  deviceRef = m_pDevice.Get();
  contextRef = m_pContext.Get();
  // 

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
  m_pContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1);

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
  m_pContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

  /// ////////////////////// TESTING


  //printf("Loading allocating vertex and pixel buffer resources...\n");
  /*
  _cubeMesh = createCubeMesh();
  _cubeMesh.vertexBuffer.init(m_pDevice.Get());
  _cubeMesh.indexBuffer.init(m_pDevice.Get());
  */

  /// ---------------------------------
  /// CONSTANT BUFFER INITIALIZATION
  /// ---------------------------------
  //__t_constBuffer = ConstantBuffer<VSInputConstantBuffer>({
  //  DirectX::XMMatrixIdentity(),
  //  DirectX::XMMatrixIdentity(),
  //});
  //printf("Loading allocating constant buffer resources...\n");
  __t_constBuffer.init(m_pDevice.Get());

  /// ---------------------------------
  /// SHADERS INITIALIZATION
  /// ---------------------------------

  //printf("Loading allocating shader resources...\n");
  //_TEST_material.initializeMaterial(m_pDevice.Get(), TEXT("bin/VertexShader.cso"), TEXT("bin/PixelShader.cso"));

  //Image img = Image();
  //FileStream::readPngImage("images/jeff.png", img);

  //printf("Loading allocating image resources...\n");
  //_test_texture.allocResource(m_pDevice.Get(), &img);
  //_test_texture.allocResource(m_pDevice.Get(), &img);

  //printf("Loading allocating sampler resources...\n");
  //_test_sampler.init(m_pDevice.Get());

  InterfaceImGui::initDirectX(m_pDevice.Get(), m_pContext.Get());

  matrix_perspective =  DirectX::XMMatrixPerspectiveLH(1.0f, 1080.0f / 1920.0f, 0.5f, 1000.0f);

  return true;
}

void DirectX11Graphics::clearBuffer(float red, float green, float blue, float alpha) {
  const float color[] = { red, green, blue, alpha };
  m_pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
  m_pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DirectX11Graphics::renderUpdate()
{
  /* RASTERIZER MINI CODE
  D3D11_RASTERIZER_DESC rsDesc{};
  rsDesc.FillMode = D3D11_FILL_SOLID;
  rsDesc.CullMode = D3D11_CULL_NONE;
  ComPtr<ID3D11RasterizerState> pRsState;
  m_pDevice->CreateRasterizerState(&rsDesc, &pRsState);
  m_pContext->RSSetState(pRsState.Get());
  */

  float runtime = Application::getRuntime();

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

  matrix_camera = DirectX::XMMatrixLookToLH(
    DirectX::XMLoadFloat3(&position),
    DirectX::XMLoadFloat3(&forward),
    DirectX::XMLoadFloat3(&up)
  );

  //_test_texture.bind(m_pContext.Get());
  //_test_sampler.bind(m_pContext.Get());

  m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  // Update subresource for Pixel shader to make cube move 
  // and rotate in 3D space based on current runtime
  //__t_constBuffer.data = {
  //  DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0, 0, 0)),
  //  DirectX::XMMatrixTranspose(matrix_camera * matrix_perspective)
  //};

  DirectX::XMStoreFloat4x4(&__t_constBuffer.data.transform, DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0, 0, 0)));
  DirectX::XMStoreFloat4x4(&__t_constBuffer.data.worldView, DirectX::XMMatrixTranspose(matrix_camera * matrix_perspective));

  //__t_constBuffer.data.transform = DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0, 0, 0));
  //__t_constBuffer.data.worldView = DirectX::XMMatrixTranspose(matrix_camera * matrix_perspective);

  // Update constant buffer and bind
  __t_constBuffer.update(m_pContext.Get());
  __t_constBuffer.bind(m_pContext.Get());

  //_TEST_material.bindShaders(m_pContext.Get());

  // Configure Viewport
  D3D11_VIEWPORT vp{};
  vp.Width    = 1920;
  vp.Height   = 1080;
  vp.MinDepth = 0;

  vp.MaxDepth = 1;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;

  m_pContext->RSSetViewports(1u, &vp);

  // TEMP - hard code index count
  m_pContext->DrawIndexed(36, 0u, 0u);
}

void DirectX11Graphics::presentBuffer()
{
  InterfaceImGui::update();

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