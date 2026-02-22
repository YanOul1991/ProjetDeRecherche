// Dx11VertexBuffer.h

#pragma once

#include "./IDx11Resource.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Graphics/Vertex.h"
#include "Core/OptimEngine.h"

struct VertexStream {
  ComPtr<ID3D11Buffer> buffer;
  uint32               stride;
  uint32               offset;
};

class Dx11VertexBuffer final : public IDx11Resource
{
 public:
  virtual ~Dx11VertexBuffer() override final;

  void create(ID3D11Device* pDevice, Vertex* pVertexBuffer, uint32 elementCount);

  virtual void bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) override final;

  uint32 bufferElementCount{};

  VertexStream vertexStreams[8];

  //ComPtr<ID3D11Buffer> pBuffer{};
  //ComPtr<ID3D11Buffer> uvStream{};
  //ComPtr<ID3D11Buffer> normalStream{};

  //uint32 stride{};
  //uint32 uvStreamStride{};
  //uint32 normalStreamStride{};


  //uint32 offset{};
  //uint32 inputSlot{};
};