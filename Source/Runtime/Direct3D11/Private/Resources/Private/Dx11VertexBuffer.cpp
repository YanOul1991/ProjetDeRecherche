#include "../Dx11VertexBuffer.h"

Dx11VertexBuffer::~Dx11VertexBuffer() {
}

void Dx11VertexBuffer::create(ID3D11Device* pDevice, Vertex* pVertexBuffer, uint32 elementCount) {
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
  OPTIM_TRY_DX(pDevice->CreateBuffer(&desc, &subres, &pBuffer));

  printf("[Dx11VertexBuffer] Vertex Buffer resource created\n");
}

inline void Dx11VertexBuffer::bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) {
  pContext->IASetVertexBuffers(0, 1, pBuffer.GetAddressOf(), &stride, &offset);
}