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