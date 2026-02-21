// Dx11DepthStencilView.h

#pragma once

#include "./IDx11Resource.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/OptimEngine.h"
#include "Runtime/Direct3D11/Dx11RHI.h"

/**
 * @brief
 * Wrapper class for ID3D11DepthStencilState resource.
 */
class Dx11DepthStencilViewTexture final : public IDx11Resource
{
 public:
  virtual ~Dx11DepthStencilViewTexture() override final;

  void create(ID3D11Device* pDevice);

  virtual void bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) override final;

  void resize(ID3D11Device* pDevice, uint32 newWidth, uint32 newHeight);

  void clearDepthStencilView(ID3D11DeviceContext* pContext) const;

  ComPtr<ID3D11Texture2D>        pDepthStencil{};
  ComPtr<ID3D11DepthStencilView> pDepthStencilView{};
};