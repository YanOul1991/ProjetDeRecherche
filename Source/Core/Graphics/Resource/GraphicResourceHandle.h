#pragma once

#include "Core/OptimEngine.h"

enum class EResourceCPUAccess {
  Read,
  Write
};

#define OPTIM_MAKE_HANDLE(_TYPE_) \
  struct _TYPE_##Handle {         \
    uint64 data;                  \
  };

enum class EResourceTypes {
  Pipeline,
  VertexBuffer,
  IndexBuffer,
  ConstantBuffer,
  Texture,
  DepthRT
};

OPTIM_MAKE_HANDLE(Resource)

OPTIM_MAKE_HANDLE(Pipeline)
OPTIM_MAKE_HANDLE(VertexBuffer)
OPTIM_MAKE_HANDLE(IndexBuffer)
OPTIM_MAKE_HANDLE(ConstantBuffer)
OPTIM_MAKE_HANDLE(TextureResource)
OPTIM_MAKE_HANDLE(DepthRT)

#undef OP_RESOURCE_HANDLE

enum class EShaderStage {
  Vertex,
  Fragment
};

struct SCBufferDesc {
  uint64 byteSize;   // Byte size of the structure
  uint32 bufferSlot; // The input slot in the shader of the constant buffer
  // void*  initalData;  // [Optional] Inital data of the buffer.

  uint32 shaderStageBind{ 1 << static_cast<uint32>(EShaderStage::Vertex) };
};