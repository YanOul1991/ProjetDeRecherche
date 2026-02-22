/* ======================================================================================
 *  Dx11ConstantBuffer.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "./IDx11Resource.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/OptimEngine.h"

#include <vector>

class Dx11ConstantBuffer final : public IDx11Resource
{
 public:
  virtual ~Dx11ConstantBuffer() override final{}

  void create(ID3D11Device* pDevice, uint32 param_structByteSize);

  virtual void bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) override final;

  void update(ID3D11DeviceContext* pContext, void* param_pNewData) const;

  ComPtr<ID3D11Buffer> pBuffer{};
  uint32               byteSize{};
};
