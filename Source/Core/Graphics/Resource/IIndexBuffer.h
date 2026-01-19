#pragma once

#include "IGraphicResource.h"

class IIndexBuffer : public IGraphicResource
{
public:
	CORE_API ~IIndexBuffer() override {};
	CORE_API virtual void createResources(uint32* pIndices, uint32 elementCount) = 0;
	CORE_API virtual void bindResource() = 0;
};