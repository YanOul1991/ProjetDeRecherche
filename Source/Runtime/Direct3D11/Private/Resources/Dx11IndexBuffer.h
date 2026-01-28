
// Dx11IndexBuffer.h

#pragma once 

#include "Core/OptimEngine.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Direct3D11/Dx11RHI.h"

class Dx11IndexBuffer final : public IIndexBuffer
{
public:
  void createResources(uint32* pIndices, uint32 elementCount) override final {
    bufferElementCount = elementCount;
    stride = 0;
    offset = 0;

    D3D11_BUFFER_DESC       desc{};
    D3D11_SUBRESOURCE_DATA  subres{};

    desc.ByteWidth            = sizeof(uint32) * bufferElementCount;
    desc.StructureByteStride  = sizeof(uint32);
    desc.Usage                = D3D11_USAGE_DEFAULT;
    desc.BindFlags            = D3D11_BIND_INDEX_BUFFER;
    desc.CPUAccessFlags       = 0;
    desc.MiscFlags            = 0;

    subres.pSysMem            = pIndices;

    HRESULT hr{ S_OK };
    OPTIM_TRY_DX(Dx11RHI::getDevicePtr()->CreateBuffer(&desc, &subres, &pBuffer));
    //printf("Index Buffer was initalized!\n");
  }

  void bindResource() override final {
     Dx11RHI::getContextPtr()->IASetIndexBuffer(pBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
  }

  ComPtr<ID3D11Buffer> pBuffer{};
  uint32 bufferElementCount{};
  uint32 stride{};
  uint32 offset{};
};