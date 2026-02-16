// Dx11GraphicsDevice.h

#pragma once

#include "Core/Graphics/IGraphicsDevice.h"

class Dx11GraphicsDevice override final : public IGraphicsDevice
{
 public:
  Dx11GraphicsDevice() = default;

  virtual ~Dx11GraphicsDevice() override final {
  }

  virtual void initalize() override final;

  virtual VertexBufferHandle createVertexBuffer(Vertex* pVertices, uint32 elementCount) override final;

  virtual IndexBufferHandle createIndexVertex(uint32* pIndices, uint32 elementCount) override final;

  virtual PipelineHandle createPipeline(SPipelineDesc* pPipelineDescription) override final;

  virtual DepthRTHandle createDepthRT() override final;

  virtual ConstantBufferHandle createConstantBuffer(uint64 objectByteSize) override final;
};

extern "C" inline DIRECTX11_API Dx11GraphicsDevice* createGraphicsDevice() {
  return new Dx11GraphicsDevice;
}