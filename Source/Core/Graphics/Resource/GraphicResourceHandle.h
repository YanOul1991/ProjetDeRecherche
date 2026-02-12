#pragma once

//#include "Core/Graphics/Resource/IGraphicResource.h"

#define OP_RESOURCE_HANDLE(_TYPE_) struct _TYPE_##Handle { uint64 data; };

enum class EResourceTypes : uint8 
{
	Pipeline,
	VertexBuffer,
	IndexBuffer,
	ConstantBuffer,
	VertexShader,
	FragmentShader,
	Texture,
	Sampler,
	DepthRT
};

OP_RESOURCE_HANDLE(Resource)
OP_RESOURCE_HANDLE(Pipeline)
OP_RESOURCE_HANDLE(VertexBuffer)
OP_RESOURCE_HANDLE(IndexBuffer)
OP_RESOURCE_HANDLE(VertexShader)
OP_RESOURCE_HANDLE(FragmentShader)
OP_RESOURCE_HANDLE(TextureResource)
OP_RESOURCE_HANDLE(Sampler)
OP_RESOURCE_HANDLE(DepthRT)
OP_RESOURCE_HANDLE(ConstantBuffer)

#undef OP_RESOURCE_HANDLE