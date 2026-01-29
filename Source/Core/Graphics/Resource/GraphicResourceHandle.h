#pragma once

#include "Core/Graphics/Resource/IGraphicResource.h"

enum class EResourceTypes : uint8 
{
	Pipeline,
	vertexBuffer,
	indexbuffer,
	VertexShader,
	FragmentShader,
};

template <typename T> 
struct GraphicResourceHandle 
{
	uint64 data;
};

using ResourceHandle				= GraphicResourceHandle<IGraphicResource>;
using VertexBufferHandle		= GraphicResourceHandle<IVertexBuffer>;
using IndexBufferHandle			= GraphicResourceHandle<IIndexBuffer>;
using VertexShaderHandle		= GraphicResourceHandle<IVertexShader>;
using FragmentShaderHandle	= GraphicResourceHandle<IFragmentShader>;
using TextureResourceHandle	= GraphicResourceHandle<ITextureResource>;
using SamplerHandle					= GraphicResourceHandle<ISampler>;
