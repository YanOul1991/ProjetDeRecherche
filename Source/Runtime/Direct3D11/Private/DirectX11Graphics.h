/* ======================================================================================
 *  DirectX11Graphics.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/Types/Color.h"
#include "Core/Object/Image/Image.h"
#include "Direct3D11/Dx11RHI.h"
#include "Private/Resources/Buffer/DirectX11Buffer.h"

#include <vector>

struct VSInputConstantBuffer 
{
  DirectX::XMFLOAT4X4 transform;  
  DirectX::XMFLOAT4X4 worldView;  
};

struct VSInputLightBuffer
{
  float3 lightPosition;
  float pad1;
  float3 lightColor;
  float pad2;
};

class DirectX11Graphics final
{
public:
  friend class Dx11RHI;

  static ID3D11Device* deviceRef;
  static ID3D11DeviceContext* contextRef;

  DirectX11Graphics();
  ~DirectX11Graphics();

  // No move or copy
  DirectX11Graphics(const DirectX11Graphics&)             = delete;
  DirectX11Graphics(const DirectX11Graphics&&)            = delete;
  DirectX11Graphics& operator=(const DirectX11Graphics&)  = delete;
  DirectX11Graphics& operator=(const DirectX11Graphics&&) = delete;

  bool initialize(HWND _outputWindow);
  void presentBuffer();
  void clearBuffer(float red, float green, float blue, float alpha);
  void renderUpdate();

private:
  ComPtr<ID3D11Device>            m_pDevice;
  ComPtr<IDXGISwapChain>          m_pSwapChain;
  ComPtr<ID3D11DeviceContext>     m_pContext;
  ComPtr<ID3D11RenderTargetView>  m_pRenderTargetView;
  ComPtr<ID3D11DepthStencilView>  m_pDepthStencilView;

  /// TEST FIELDS
  DirectX::XMMATRIX matrix_perspective{};
  DirectX::XMMATRIX matrix_camera{};

  ConstantBuffer<VSInputConstantBuffer> __t_constBuffer{};

  uint32 drawCallIndexCount{0};

  friend Dx11RHI;
};