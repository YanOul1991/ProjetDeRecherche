#pragma once

#include "Core/Graphics/Resource/IGraphicResource.h"

template <typename T>
struct GraphicResourceHandle {
	uint64 data;
};

using ResourceHandle				= GraphicResourceHandle<IGraphicResource>;
using VertexBufferHandle		= GraphicResourceHandle<IVertexBuffer>;
using IndexBufferHandle			= GraphicResourceHandle<IIndexBuffer>;
using VertexShaderHandle		= GraphicResourceHandle<IVertexShader>;
using FragmentShaderHandle	= GraphicResourceHandle<IFragmentShader>;
using TextureResourceHandle	= GraphicResourceHandle<ITextureResource>;
using SamplerHandle					= GraphicResourceHandle<ISampler>;