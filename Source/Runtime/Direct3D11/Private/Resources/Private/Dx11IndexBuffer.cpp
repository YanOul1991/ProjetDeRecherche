// Dx11IndexBuffer.cpp

#include "../Dx11IndexBuffer.h"

Dx11IndexBuffer::~Dx11IndexBuffer() {

}

void Dx11IndexBuffer::create(ID3D11Device* pDevice, uint32* pIndices, uint32 elementCount) {
  OPTIM_WIN_COM_CHECK_START();

  bufferElementCount = elementCount;
  stride             = 0;
  offset             = 0;

  D3D11_BUFFER_DESC      desc{};
  D3D11_SUBRESOURCE_DATA subres{};

  desc.ByteWidth           = sizeof(uint32) * bufferElementCount;
  desc.StructureByteStride = sizeof(uint32);
  desc.Usage               = D3D11_USAGE_DEFAULT;
  desc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
  desc.CPUAccessFlags      = 0;
  desc.MiscFlags           = 0;

  subres.pSysMem = pIndices;

  OPTIM_TRY_DX(pDevice->CreateBuffer(&desc, &subres, &pBuffer));
  // printf("[Dx11IndexBuffer] Index Buffer resource created\n");
}

void Dx11IndexBuffer::bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) {
  pContext->IASetIndexBuffer(pBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}
