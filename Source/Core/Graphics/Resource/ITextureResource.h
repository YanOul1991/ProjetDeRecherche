#pragma once

#include "Core/OptimEngine.h"
#include "Core/Graphics/Resource/IGraphicResource.h"
#include "Core/Object/Image/Image.h"

class ITextureResource : public IGraphicResource
{
public:
	CORE_API ~ITextureResource() override {}
	CORE_API virtual void createResource(const Image* pImage) = 0;
	CORE_API virtual void bindResource() = 0;
};