// Dx11ConstantBuffer.h

#pragma once

#include "./IDx11Resource.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Graphics/Resource/GraphicResourceHandle.h"
#include "Core/OptimEngine.h"

#include <vector>

class Dx11ConstantBuffer final : public IDx11Resource
{
 public:
  virtual ~Dx11ConstantBuffer() override final;

  /**
   * \deprecated
   * Swiching to Create(ComPtr<ID3D11Device>& pDevice, const SCBufferDesc& bufferDesc);
   */
  void create(ID3D11Device* pDevice, uint32 param_structByteSize);

  void Create(ComPtr<ID3D11Device>& pDevice, const SCBufferDesc& bufferDesc);

  virtual void bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) override final;

  void update(ID3D11DeviceContext* pContext, void* param_pNewData) const;

  ComPtr<ID3D11Buffer> pBuffer{};

  uint32 byteSize{};
  uint32 bindSlot{};
  uint32 bindStage{};
};
