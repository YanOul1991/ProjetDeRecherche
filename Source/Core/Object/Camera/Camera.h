#pragma once

#include "Core/OptimEngine.h"
#include "Core/Math/OptimMathematics.h"

class CORE_API Camera final
{
public:
	static float posX;
	static float posY;
	static float posZ;

	static float pitch; // The X rotation of the camera.
	static float yaw;		// The Y rotation of the camera.
	static float roll;	// The Z rotation of the camera.

	static float3 Forward;
	static float3 right;
	static float3 up;
};