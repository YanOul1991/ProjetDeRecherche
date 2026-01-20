#pragma once

#include "Core/OptimEngine.h"
#include "IGraphicResource.h"

class ISampler : public IGraphicResource
{
public:
	CORE_API ~ISampler() override{}
	CORE_API virtual void createResource() = 0;
	CORE_API virtual void bindResource() = 0;
};