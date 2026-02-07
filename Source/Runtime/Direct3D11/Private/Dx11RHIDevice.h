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

//struct VSInputConstantBuffer 
//{
//  float4x4 transform;             // Transform of mesh object
//  DirectX::XMFLOAT4X4 worldView;  // Camera view matrix
//  float3 cameraPosition;
//  float padding;
//};

struct alignas(16) VSInputConstantBuffer 
{
  float4x4 transform;
  float4x4 lookAtMatrix;
  float4x4 perspectiveMatrix;
};

class Dx11RHIDevice final
{
public:
  static ID3D11Device*            deviceRef;
  static ID3D11DeviceContext*     contextRef;
  static ID3D11RenderTargetView*  renderTargetView;

  Dx11RHIDevice();
  ~Dx11RHIDevice();

  // No move or copy
  Dx11RHIDevice(const Dx11RHIDevice&)             = delete;
  Dx11RHIDevice(const Dx11RHIDevice&&)            = delete;
  Dx11RHIDevice& operator=(const Dx11RHIDevice&)  = delete;
  Dx11RHIDevice& operator=(const Dx11RHIDevice&&) = delete;

  bool initialize(HWND outputWindow, Dx11RHI* pDx11RHI);

  /*
   * @brief
   * Initalized an ID3D11RenderTargetView object to get reference to
   * backbuffer.
  */
  void initRenderTargetView(uint32 newWidth, uint32 newHeight);

  /**
   * @brief
   * Clears the ID3D11RenderTargetView object references and sets
   * the render target values of the the render target view and depth stencil 
   * to null;
   */
  void clearRenderTargetView();

  void presentBuffer() const;
  void clearBuffer(float red, float green, float blue, float alpha) const;
  void renderUpdate();

  ComPtr<ID3D11Device>            m_pDevice           {nullptr};
  ComPtr<IDXGISwapChain>          m_pSwapChain        {nullptr};
  ComPtr<ID3D11DeviceContext>     m_pContext          {nullptr};
  ComPtr<ID3D11RenderTargetView>  m_pRenderTargetView {nullptr};

private:

  ConstantBufferHandle  constantBufferTransformView{};
  VSInputConstantBuffer vsInputConstBufferData{};
  Dx11RHI* pDxRHI{};
  friend Dx11RHI;
};