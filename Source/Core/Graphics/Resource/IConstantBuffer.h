#pragma once

#include "Core/OptimEngine.h"
#include "Core/Graphics/Resource/IGraphicResource.h"

class IConstantBuffer : public IGraphicResource
{
public:
	CORE_API ~IConstantBuffer() override
	{}
};