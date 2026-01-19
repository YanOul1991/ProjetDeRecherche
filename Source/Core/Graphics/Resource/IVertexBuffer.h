#pragma once

#include "IGraphicResource.h"
#include "Core/OptimEngine.h"
#include "Core/Graphics/Vertex.h"

class IVertexBuffer : public IGraphicResource 
{
public:
	CORE_API ~IVertexBuffer() override {};
	CORE_API virtual void createResources(Vertex* pVertexBuffer, uint32 elementCount) = 0;
	CORE_API virtual void bindResource() = 0;
};