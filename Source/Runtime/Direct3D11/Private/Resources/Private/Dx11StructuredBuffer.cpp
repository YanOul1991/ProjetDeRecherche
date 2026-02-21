#include "../Dx11StructuredBuffer.h"

#include "Core/Math/OptimMathematics.h"

Dx11StructuredBuffer::~Dx11StructuredBuffer() {
}

void Dx11StructuredBuffer::bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) {
  pContext->VSGetShaderResources(1, 1, &srv);
}

void Dx11StructuredBuffer::create(ID3D11Device* pDevice) {
  OPTIM_WIN_COM_CHECK_START();

  D3D11_BUFFER_DESC desc{};

  desc.Usage               = D3D11_USAGE_DYNAMIC;
  desc.ByteWidth           = sizeof(float4) * (3);
  desc.BindFlags           = D3D11_BIND_SHADER_RESOURCE;
  desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
  desc.MiscFlags           = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
  desc.StructureByteStride = sizeof(float4);

  OPTIM_TRY_DX(pDevice->CreateBuffer(&desc, nullptr, &buffer));

  // Shader resource view

  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};

  srvDesc.Format              = DXGI_FORMAT_UNKNOWN;
  srvDesc.ViewDimension       = D3D11_SRV_DIMENSION_BUFFER;
  srvDesc.Buffer.FirstElement = 0;
  srvDesc.Buffer.NumElements  = 3;

  pDevice->CreateShaderResourceView(buffer.Get(), &srvDesc, &srv);

  printf("[Dx11StructuredBuffer] Structured buffer created\n");
}

