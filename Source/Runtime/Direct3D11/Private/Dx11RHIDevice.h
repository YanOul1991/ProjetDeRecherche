/* ======================================================================================
 *  Dx11RHIDevice.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Math/OptimMathematics.h"
#include "Direct3D11/Dx11RHI.h"

struct alignas(16) VSInputConstantBuffer {
  float4x4 transform;
  float4x4 lookAtMatrix;
  float4x4 perspectiveMatrix;
};

class Dx11RHIDevice final
{
 public:
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

  ComPtr<ID3D11Device>           m_pDevice{nullptr};
  ComPtr<IDXGISwapChain>         m_pSwapChain{nullptr};
  ComPtr<ID3D11DeviceContext>    m_pContext{nullptr};
  ComPtr<ID3D11RenderTargetView> m_pRenderTargetView{nullptr};

 private:
  ConstantBufferHandle constantBufferTransformView{};

  VSInputConstantBuffer vsInputConstBufferData{};

  Dx11RHI* pDxRHI{};
  friend Dx11RHI;
};