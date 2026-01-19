/* ======================================================================================
 *  DirectX11Buffer.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Defines/DirectX/msDx11.h"

class IDirectX11Buffer
{
public:
  virtual ~IDirectX11Buffer() {}

  virtual void init(ID3D11Device* pDevice)         = 0;
  virtual void bind(ID3D11DeviceContext* pContext) = 0;

  ComPtr<ID3D11Buffer> pBuffer;
  int32   bufferByteSize  { 0 };
  int32   elementCount    { 0 };
  uint32  stride          { 0 };
  uint32  offset          { 0 };

};

template <typename T> 
class ConstantBuffer final : public IDirectX11Buffer
{
public:
  inline ConstantBuffer() {
  }
  inline ~ConstantBuffer() override
  {}

  inline ConstantBuffer(T&& initial_data)
  {
    data            = initial_data;
    bufferByteSize  = sizeof(T);
    elementCount    = bufferByteSize / sizeof(T);
    stride          = 0;
    offset          = 0;
    data            = initial_data;
  }

  inline void init(ID3D11Device* pDevice) override 
  { 
    D3D11_BUFFER_DESC       desc{};
    D3D11_SUBRESOURCE_DATA  subres{};

    desc.ByteWidth            = sizeof(T);
    desc.StructureByteStride  = 0;
    desc.Usage                = D3D11_USAGE_DYNAMIC;
    desc.BindFlags            = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags       = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags            = 0;

    subres.pSysMem            = &data;

    pDevice->CreateBuffer(&desc, &subres, &pBuffer);
    //printf("Element at 0x%p is a Constant buffer resource\n", this);
  }

  inline void bind(ID3D11DeviceContext* pContext) override { 
    pContext->VSSetConstantBuffers(0, 1, pBuffer.GetAddressOf());
  }

  inline void update(ID3D11DeviceContext* pContext) {
    D3D11_MAPPED_SUBRESOURCE mapped;
    pContext->Map(pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    memcpy(mapped.pData, &data, sizeof(T));
    pContext->Unmap(pBuffer.Get(), 0);
  }

  T data{};
};