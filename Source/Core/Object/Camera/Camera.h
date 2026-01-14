#pragma once

#include "Core/OptimEngine.h"
#include "Core/Math/OptimMathematics.h"
#include "Core/Math/OptimMathematics.h"
#include "Core/Math/Quaternion.h"

class Camera final
{
public:
	CORE_API static float posX;
	CORE_API static float posY;
	CORE_API static float posZ;

	CORE_API static float3 position;
	CORE_API static Quaternion rotation;

	CORE_API static float pitch; // The X rotation of the camera.
	CORE_API static float yaw;		// The Y rotation of the camera.
	CORE_API static float roll;	// The Z rotation of the camera.

	CORE_API static float3 Forward;
	CORE_API static float3 right;
	CORE_API static float3 up;
};