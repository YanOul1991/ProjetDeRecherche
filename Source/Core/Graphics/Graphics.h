/* ======================================================================================
 *  Graphics.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/System/Application.h"
#include "Core/Graphics/Resource/IGraphicResource.h"

/*
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
*/

/*
class CORE_API Graphics final
{
public:
	static inline IGraphicsRHI* RHI() 
	{
		return s_pActiveGraphicsRHI;
	}
private:
	static inline void initalize(IGraphicsRHI* param_activeRenderModule) 
	{
		s_pActiveGraphicsRHI = param_activeRenderModule;
	}
	static IGraphicsRHI* s_pActiveGraphicsRHI;

	friend Application;
};
*/