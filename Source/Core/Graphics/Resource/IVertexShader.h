#pragma once

#include "Core/OptimEngine.h"
#include "IGraphicResource.h"

class IVertexShader : public IGraphicResource 
{
public:
	CORE_API ~IVertexShader() override {};
	CORE_API virtual void createResources(const wchar* path) = 0;
	CORE_API virtual void bindResource() = 0;
};