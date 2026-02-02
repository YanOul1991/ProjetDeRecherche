/* ======================================================================================
 *  OptimMathematics.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "math.h"
#include <cstdlib>
#include <cstdio>

namespace mathConst {

constexpr float		PI = 3.1415927f;
constexpr double	PI_double = 3.141592653589793;

}

struct float2 {
	float x;
	float y;
};

struct UVCoord {
	float u;
	float v;
};

struct float3 {
	float x{0};
	float y{0};
	float z{0};
};


struct float4 {
	float x;
	float y;
	float z;
	float w;
};

struct alignas(16) float3x3 {
	float m11; float m12; float m13;
	float m21; float m22; float m23;
	float m31; float m32; float m33;
};

/*
 * @brief
 * A structure defining a 4x4 float matrix.
*/
struct alignas(16) float4x4 {
	float m11, m12, m13, m14;
	float m21, m22, m23, m24;
	float m31, m32, m33, m34;
	float m41, m42, m43, m44;

	inline void printMatrix() const
	{
		printf("| %2.7f, %2.7f, %2.7f, %2.7f |\n", m11, m12, m13, m14);
		printf("| %2.7f, %2.7f, %2.7f, %2.7f |\n", m21, m22, m23, m24);
		printf("| %2.7f, %2.7f, %2.7f, %2.7f |\n", m31, m32, m33, m34);
		printf("| %2.7f, %2.7f, %2.7f, %2.7f |\n", m41, m42, m43, m44);
	}

	inline float4x4 transpose()
	{
		return {
			m11, m21, m31, m41,
			m12, m22, m32, m42,
			m13, m23, m33, m43,
			m14, m24, m34, m44,
		};
	}
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
	return{
		(1.0f / magnitude(v)) * v
	};
}

inline float dotProduct(const float3& a, const float3& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}