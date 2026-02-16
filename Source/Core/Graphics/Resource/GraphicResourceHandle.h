#pragma once

// #include "Core/Graphics/Resource/IGraphicResource.h"

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
  //VertexShader,
  //FragmentShader,
  //Sampler,
};

OPTIM_MAKE_HANDLE(Resource)

OPTIM_MAKE_HANDLE(Pipeline)
OPTIM_MAKE_HANDLE(VertexBuffer)
OPTIM_MAKE_HANDLE(IndexBuffer)
OPTIM_MAKE_HANDLE(ConstantBuffer)
OPTIM_MAKE_HANDLE(TextureResource)
OPTIM_MAKE_HANDLE(DepthRT)

// TO BE DEPRECATED

//OPTIM_MAKE_HANDLE(Sampler)
//OPTIM_MAKE_HANDLE(FragmentShader)
//OPTIM_MAKE_HANDLE(VertexShader)

#undef OP_RESOURCE_HANDLE