#pragma once

#include "Core/Graphics/Vertex.h"
#include "Core/Object/Image/Image.h"
#include "Core/OptimEngine.h"

enum class EResourceCPUAccess {
  Read,
  Write
};

/*
class IGraphicResource
{
 public:
  CORE_API virtual ~IGraphicResource(){};
};

class IVertexBuffer : public IGraphicResource
{
 public:
  CORE_API ~IVertexBuffer() override{};
  CORE_API virtual void createResources(Vertex* pVertexBuffer, uint32 elementCount) = 0;
  CORE_API virtual void bindResource()                                              = 0;
};

class IIndexBuffer : public IGraphicResource
{
 public:
  CORE_API ~IIndexBuffer() override{};
  CORE_API virtual void createResources(uint32* pIndices, uint32 elementCount) = 0;
  CORE_API virtual void bindResource()                                         = 0;
};

class IConstantBuffer : public IGraphicResource
{
 public:
  CORE_API virtual ~IConstantBuffer() override {
  }
  CORE_API virtual void createResource(EResourceCPUAccess cpuAccess, void* pInitalData, uint32 dataByteSize) = 0;
  CORE_API virtual void update(void* data)                                                                   = 0;
  CORE_API virtual void bindResource()                                                                       = 0;
};

class IVertexShader : public IGraphicResource
{
 public:
  CORE_API ~IVertexShader() override{};
  CORE_API virtual void createResources(const wchar* path) = 0;
  CORE_API virtual void bindResource()                     = 0;
};

class IPixelShader : public IGraphicResource
{
 public:
  CORE_API ~IPixelShader() override{};
  CORE_API virtual void createResources(const wchar* path) = 0;
  CORE_API virtual void bindResource()                     = 0;
};

class ISampler : public IGraphicResource
{
 public:
  CORE_API ~ISampler() override {
  }
  CORE_API virtual void createResource() = 0;
  CORE_API virtual void bindResource()   = 0;
};

class ITextureResource : public IGraphicResource
{
 public:
  CORE_API ~ITextureResource() override {
  }
  CORE_API virtual void createResource(const Image* pImage) = 0;
  CORE_API virtual void bindResource()                      = 0;
};

using IFragmentShader = IPixelShader;
*/