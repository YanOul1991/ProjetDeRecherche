// Dx11 VertexBuffer.h

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Graphics/Resource/IVertexBuffer.h"
#include "Direct3D11/IDirect3D11.h"

class Dx11VertexBuffer final : public IVertexBuffer 
{
public:
  ~Dx11VertexBuffer() override {}

  inline void createResources(Vertex* pVertexBuffer, uint32 elementCount) override final {
    bufferElementCount = elementCount;
    stride = sizeof(Vertex);
    offset = 0;

    D3D11_BUFFER_DESC       desc{};
    D3D11_SUBRESOURCE_DATA  subres{};

    desc.ByteWidth            = sizeof(Vertex) * elementCount;
    desc.StructureByteStride  = sizeof(Vertex);
    desc.Usage                = D3D11_USAGE_DEFAULT;
    desc.BindFlags            = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags       = 0;
    desc.MiscFlags            = 0;

    subres.pSysMem            = pVertexBuffer;

    HRESULT hr = S_OK;
    OPTIM_TRY_DX(IDirect3D11::getDevicePtr()->CreateBuffer(&desc, &subres, &pBuffer));

    printf("Vertex Buffer was initalized!\n");
  }

  inline void bindResource() override final {
    IDirect3D11::getContextPtr()->IASetVertexBuffers(0, 1, pBuffer.GetAddressOf(), &stride, &offset);
  }

  ComPtr<ID3D11Buffer> pBuffer{};
  uint32 bufferElementCount{};
  uint32 stride{};
  uint32 offset{};
};