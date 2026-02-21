// Dx11Texture.h

#pragma once

#include "./IDx11Resource.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Object/Image/Image.h"
#include "Core/OptimEngine.h"

class Dx11TextureResource final : public IDx11Resource
{
 public:
  virtual ~Dx11TextureResource() override final;

  void create(ID3D11Device* pDevice, const Image* pImage);

  virtual void bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) override final;

  ComPtr<ID3D11Texture2D>          pTexture{};
  ComPtr<ID3D11ShaderResourceView> pResourceView{};
};