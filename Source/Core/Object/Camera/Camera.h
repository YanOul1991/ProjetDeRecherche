#pragma once

#include "Core/OptimEngine.h"

struct float3
{
	float x;
	float y;
	float z;
};

class CORE_API Camera final
{
public:
	static float posX;
	static float posY;
	static float posZ;

	static float rotX;
	static float rotY;
	static float rotZ;

	static float3 Forward;
	static float3 right;
	static float3 up;
};