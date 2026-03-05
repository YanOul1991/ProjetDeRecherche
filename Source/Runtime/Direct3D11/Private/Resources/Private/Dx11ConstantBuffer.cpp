// Dx11ConstantBuffer.cpp

#include "../Dx11ConstantBuffer.h"

#include <cstdio>
#include <cstdlib>

Dx11ConstantBuffer::~Dx11ConstantBuffer() {
}

void Dx11ConstantBuffer::create(ID3D11Device* pDevice, uint32 param_structByteSize) {
  this->byteSize = param_structByteSize;
  this->bindSlot = 0;
  this->bindStage = 1 << (uint32)EShaderStage::Vertex;

  D3D11_BUFFER_DESC desc{};

  desc.ByteWidth      = byteSize;
  desc.Usage          = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
  desc.BindFlags      = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
  desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
  desc.MiscFlags      = 0;

  pDevice->CreateBuffer(&desc, nullptr, &pBuffer);

  printf("[Dx11ConstantBuffer] Constant buffer resource created\n");
}

void Dx11ConstantBuffer::Create(ComPtr<ID3D11Device>& pDevice, const SCBufferDesc& bufferDesc) {
  if ((bufferDesc.byteSize % 16) != 0) {
    printf("[Dx11ConstantBuffer::Create] The constant buffer data must be 16 bytes aliged\n");
    return;
  }

  this->byteSize  = (uint32)bufferDesc.byteSize;
  this->bindSlot  = bufferDesc.bufferSlot;
  this->bindStage = bufferDesc.shaderStageBind;

  D3D11_BUFFER_DESC desc{};

  desc.ByteWidth      = this->byteSize;
  desc.Usage          = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
  desc.BindFlags      = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
  desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
  desc.MiscFlags      = 0;

  pDevice->CreateBuffer(&desc, nullptr, &pBuffer);

  printf("[Dx11ConstantBuffer::Create] Constant buffer resource created through new function!\n");

  if (bindStage & (1U << static_cast<uint32>(EShaderStage::Vertex))) {
    printf("[Dx11ConstantBuffer] Resource will be binding to vertex shader stage.\n");
  }
  if (bindStage & (1U << static_cast<uint32>(EShaderStage::Fragment))) {
    printf("[Dx11ConstantBuffer] Resource will be binding to pixel shader stage.\n");
  }
}

void Dx11ConstantBuffer::bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) {
  if (bindStage & (1U << static_cast<uint32>(EShaderStage::Vertex))) {
    pContext->VSSetConstantBuffers(bindSlot, 1, pBuffer.GetAddressOf());
  }
  if (bindStage & (1U << static_cast<uint32>(EShaderStage::Fragment))) {
    pContext->PSSetConstantBuffers(bindSlot, 1, pBuffer.GetAddressOf());
  }
}

void Dx11ConstantBuffer::update(ID3D11DeviceContext* pContext, void* param_pNewData) const {
  D3D11_MAPPED_SUBRESOURCE mapped{};
  pContext->Map(pBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mapped);
  memcpy(mapped.pData, param_pNewData, byteSize);
  pContext->Unmap(pBuffer.Get(), 0);
}