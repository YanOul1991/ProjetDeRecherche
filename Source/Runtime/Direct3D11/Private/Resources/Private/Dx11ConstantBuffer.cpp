// Dx11ConstantBuffer.cpp

#include "../Dx11ConstantBuffer.h"
#include <cstdlib>
#include <cstdio>

void Dx11ConstantBuffer::create(ID3D11Device* pDevice, uint32 param_structByteSize) {
  byteSize = param_structByteSize;
  D3D11_BUFFER_DESC desc{};

  desc.ByteWidth      = byteSize;
  desc.Usage          = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
  desc.BindFlags      = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
  desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
  desc.MiscFlags      = 0;

  pDevice->CreateBuffer(&desc, nullptr, &pBuffer);

  printf("[Dx11ConstantBuffer] Constant buffer resource created\n");
}

void Dx11ConstantBuffer::bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) {
  pContext->VSSetConstantBuffers(0, 1, pBuffer.GetAddressOf());
}

void Dx11ConstantBuffer::update(ID3D11DeviceContext* pContext, void* param_pNewData) const {
  D3D11_MAPPED_SUBRESOURCE mapped{};
  pContext->Map(pBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mapped);
  memcpy(mapped.pData, param_pNewData, byteSize);
  pContext->Unmap(pBuffer.Get(), 0);
}