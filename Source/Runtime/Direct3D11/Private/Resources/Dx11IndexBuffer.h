// Dx11IndexBuffer.h

#pragma once

#include "./IDx11Resource.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/OptimEngine.h"

class Dx11IndexBuffer final : public IDx11Resource
{
 public:
  virtual ~Dx11IndexBuffer() override final;

  void create(ID3D11Device* pDevice, uint32* pIndices, uint32 elementCount);

  virtual void bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) override final;

  ComPtr<ID3D11Buffer> pBuffer{};
  uint32               bufferElementCount{};
  uint32               stride{};
  uint32               offset{};
};