
#pragma once

#include "Core/OptimEngine.h"
#include "math.h"

struct float2
{
	float x;
	float y;
};

struct float3
{
	float x;
	float y;
	float z;
};


struct float4
{
	float x;
	float y;
	float z;
	float w;
};

struct float3x3
{
	float m11; float m12; float m13;
	float m21; float m22; float m23;
	float m31; float m32; float m33;
};

/*
 *	A structure defining a 4x4 float matrix
*/
struct float4x4
{
	float m11; float m12; float m13; float m14;
	float m21; float m22; float m23; float m24;
	float m31; float m32; float m33; float m34;
	float m41; float m42; float m43; float m44;
};


/**
 * ************************************************
 *			Operator overload
 * ************************************************
*/

inline float3 operator+(float3 left, float3 right)
{
	return { 
		left.x + right.x,
		left.y + right.y,
		left.z + right.z
	};
}

inline float3 operator-(float3 left, float3 right)
{
	return { 
		left.x - right.x,
		left.y - right.y,
		left.z - right.z
	};
}

inline float3 operator*(float scalar, float3 vector)
{
	return{
		vector.x * scalar,
		vector.y * scalar,
		vector.z * scalar
	};
}

inline float3 operator/(float scalar, float3 vector)
{
	return{
		vector.x / scalar,
		vector.y / scalar,
		vector.z / scalar
	};
}

/**
 * ************************************************
 *			Utility functions
 * ************************************************
*/

inline float3 cross(const float3& a, const float3& b)
{
	return{
			(a.y * b.z) - (a.z * b.y),
		-((a.x * b.z) - (a.z * b.x)),
			(a.x * b.y) - (a.y * b.x)
	};
}

inline float magnitude(const float3& v)
{
	return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

inline float3 normalize(const float3& v)
{
	return {
		(1.0f / magnitude(v)) * v
	};
}