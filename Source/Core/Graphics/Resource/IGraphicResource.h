#pragma once

#include "Core/OptimEngine.h"

enum class EResourceTypes : uint8 
{
	vertexBuffer,
	indexbuffer,
};

/*
 * @brief
 * Base class interface for all graphic resource types.
*/
class IGraphicResource
{
public:
	CORE_API virtual ~IGraphicResource(){};
};

struct ResourceHandle {
	uint64 data{};
};