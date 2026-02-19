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

class Dx11StructuredBuffer : public IDx11Resource
{
  virtual ~Dx11StructuredBuffer() override final;

  virtual void bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView);

  void create(ID3D11Device* pDevice);

  ComPtr<ID3D11Buffer> buffer;
  ComPtr<ID3D11ShaderResourceView> srv;
};