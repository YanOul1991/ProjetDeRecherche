// Dx11 VertexBuffer.h

#pragma once

#include "Core/Defines/DirectX/msDx11.h"
#include "Core/OptimEngine.h"
#include "./IDx11Resource.h"
#include "Direct3D11/Dx11RHI.h"

class Dx11VertexBuffer final : public IDx11Resource
{
 public:
  virtual ~Dx11VertexBuffer() override final {
  }
  virtual void bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) override final {
  }

  inline void createResources(Vertex* pVertexBuffer, uint32 elementCount) {
    bufferElementCount = elementCount;
    stride             = sizeof(Vertex);
    offset             = 0;

    D3D11_BUFFER_DESC      desc{};
    D3D11_SUBRESOURCE_DATA subres{};

    desc.ByteWidth           = sizeof(Vertex) * elementCount;
    desc.StructureByteStride = sizeof(Vertex);
    desc.Usage               = D3D11_USAGE_DEFAULT;
    desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags      = 0;
    desc.MiscFlags           = 0;

    subres.pSysMem = pVertexBuffer;

    HRESULT hr = S_OK;
    OPTIM_TRY_DX(Dx11RHI::getDevicePtr()->CreateBuffer(&desc, &subres, &pBuffer));

    // printf("Vertex Buffer was initalized!\n");
  }

  inline void bindResource() {
    Dx11RHI::getContextPtr()->IASetVertexBuffers(0, 1, pBuffer.GetAddressOf(), &stride, &offset);
  }

  ComPtr<ID3D11Buffer> pBuffer{};
  uint32               bufferElementCount{};
  uint32               stride{};
  uint32               offset{};
};