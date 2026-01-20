#pragma once

#include "Core/OptimEngine.h"

typedef struct SGraphicResourceHandle {
	uint32 data;
	uint32 generation;
} SGraphicResourceHandle;

/*
 * @brief
 * Base class interface for all graphic resource types.
*/
class IGraphicResource
{
public:
	CORE_API virtual ~IGraphicResource(){};
};