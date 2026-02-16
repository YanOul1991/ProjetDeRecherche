// IDx11Resource.h

#pragma once

#include "Core/Defines/DirectX/msDx11.h"
#include "Core/OptimEngine.h"

class IDx11Resource
{
 public:
  virtual ~IDx11Resource() {};
  virtual void bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) = 0;
};