// Dx11DescriptorSet.h

#pragma once

#include "./IDx11Resource.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/OptimEngine.h"
#include <vector>

class Dx11DescriptorSet final : public IDx11Resource
{
  inline virtual ~Dx11DescriptorSet() override final {
  }

  inline virtual void bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) override final {
  }

  inline void create(ID3D11Device* pDevice) {
  }

  std::vector<ComPtr<ID3D11ShaderResourceView>> m_srv;
  std::vector<ComPtr<ID3D11SamplerState>>       m_samplers;
  std::vector<ComPtr<ID3D11Buffer>>             m_constantBuffers;
};