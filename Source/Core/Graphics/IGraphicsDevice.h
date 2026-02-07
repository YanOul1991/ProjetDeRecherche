/* ======================================================================================
 *  IGraphicsDevice.h:
 *      This class serves as a central module that managed lifetimes of other Engine 
 *      implemented modules, and manages the life time of the application.
 * 
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Graphics/Vertex.h"
#include "Core/Graphics/Resource/IGraphicResource.h"
#include "Core/Graphics/Resource/GraphicResourceHandle.h"
#include "Core/Graphics/Resource/GraphicPipeline.h"

class CORE_API IGraphicsDevice {
public:

	virtual ~IGraphicsDevice()
	{}
	virtual void initalize() = 0;

	virtual VertexBufferHandle		createVertexBuffer(Vertex* pVertices, uint32 elementCount) = 0;
	virtual IndexBufferHandle			createIndexVertex(uint32* pIndices, uint32 elementCount) = 0;
	virtual FragmentShaderHandle	createFragmentShader(const char* path) = 0;
	virtual PipelineHandle				createPipeline(SPipelineDesc* pPipelineDesc) = 0;
	virtual DepthRTHandle					createDepthRT() = 0;
	virtual ConstantBufferHandle	createConstantBuffer(uint64 objectByteSize) = 0;
};