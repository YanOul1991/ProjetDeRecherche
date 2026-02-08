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

namespace Optim::Constants {

constexpr float		pi				= 3.1415927f;
constexpr double	pi_d			= 3.141592653589793;
constexpr uint64	maxUint64 = 0xFFFFFFFFFFFFFFFFull;
constexpr uint64	maxUint32 = 0xFFFFFFFFu;
constexpr uint64	maxUint16 = 0xFFFFull;
constexpr uint64	maxUint8	= 0xFFull;
constexpr int64		maxInt64	= 0x7FFFFFFFFFFFFFFFull;
constexpr int64		minInt64	= 0x8000000000000000;
constexpr int64		maxInt32	= 0x7FFFFFFF;
constexpr int64		minInt32	= 0x80000000;
constexpr int64		maxInt16	= 0x7FFF;
constexpr int64		minInt16	= 0x8000;
constexpr int64		maxInt8		= 0x7F;
constexpr int64		minInt8		= 0x80;

constexpr float		EPSILONF	= 1E-6F;

/*
constexpr float		pi				= 3.1415927f;
constexpr double	pi_d			= 3.141592653589793;
constexpr uint64	maxUint64 = 0xFFFFFFFFFFFFFFFFull;
constexpr uint32	maxUint32 = 0xFFFFFFFFu;
constexpr uint16	maxUint16 = 0xFFFFull;
constexpr uint8		maxUint8	= 0xFFull;

constexpr int64		maxInt64	= 0x7FFFFFFFFFFFFFFFull;
constexpr int64		minInt64	= 0x8000000000000000;
constexpr int32		maxInt32	= 0x7FFFFFFF;
constexpr int32		minInt32	= 0x80000000;
constexpr int16		maxInt16	= 0x7FFF;
constexpr int16		minInt16	= 0x8000;
constexpr int8		maxInt8		= 0x7F;
constexpr int8		minInt8		= 0x80;
*/

} // Namespace - END 

struct UVCoord 
{
	float u{0};
	float v{0};
};

struct float2 
{
	float x{0};
	float y{0};
};

struct float3 {
	float x{0};
	float y{0};
	float z{0};

	inline void print()
	{
		printf("(%f, %f, %f)\n", x, y, z);
	}
};

struct alignas(16) float4 {
	float x{0};
	float y{0};
	float z{0};
	float w{0};
};

struct alignas(16) float2x2 
{
	float m11{1}, m12{0};
	float m21{0}, m22{1};
};

struct alignas(16) float3x3 
{
	float m11{1}, m12{0}, m13{0};
	float m21{0}, m22{1}, m23{0};
	float m31{0}, m32{0}, m33{1};
};

struct alignas(16) float4x4 
{
	float m11{1}, m12{0}, m13{0}, m14{0};
	float m21{0}, m22{1}, m23{0}, m24{0};
	float m31{0}, m32{0}, m33{1}, m34{0};
	float m41{0}, m42{0}, m43{0}, m44{1};

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
inline void matrixPrint(const float2x2& mat)
{
	printf("| %2.7f, %2.7f |\n", mat.m11, mat.m12);
	printf("| %2.7f, %2.7f |\n", mat.m21, mat.m22);
}
inline void matrixPrint(const float3x3& mat)
{
	printf("| %02.7f, %02.7f, %02.7f |\n", mat.m11, mat.m12, mat.m13);
	printf("| %02.7f, %02.7f, %02.7f |\n", mat.m21, mat.m22, mat.m23);
	printf("| %02.7f, %02.7f, %02.7f |\n", mat.m31, mat.m32, mat.m33);
}

/* ////////////////////////////////////////////////////
 * 
 * MATRIX	OPERATOR OVERLOADS
 * 
//////////////////////////////////////////////////// */

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

/* +++++++++++++++++++++++++++++++++++++++++++++++
		MATRIX SCALAR MULTIPLICATION
+++++++++++++++++++++++++++++++++++++++++++++++ */

inline float2x2 operator*(float s, const float2x2& mat) 
{
	return {
		s * mat.m11, s * mat.m12,
		s * mat.m21, s * mat.m22,
	};
}

inline float3x3 operator*(float s, const float3x3& m)
{
	return{
		m.m11 * s, m.m12 * s, m.m13 * s,
		m.m21 * s, m.m22 * s, m.m23 * s,
		m.m31 * s, m.m32 * s, m.m33 * s
	};
}

inline float4x4 operator*(float s, const float4x4& m)
{
	return{
		m.m11 * s, m.m12 * s, m.m13 * s, m.m14 * s,
		m.m21 * s, m.m22 * s, m.m23 * s, m.m24 * s,
		m.m31 * s, m.m32 * s, m.m33 * s, m.m34 * s,
		m.m41 * s, m.m42 * s, m.m43 * s, m.m44 * s
	};
}

inline float4x4 operator*(const float4x4& m, float s)
{
	return s * m;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++
		MATRIX x VECTOR MULTIPLICATION
+++++++++++++++++++++++++++++++++++++++++++++++ */

//inline float4 operator*(const float4x4& mat, const float4& vec)
//{
//	return {
//		vec.x * mat.m11 + vec.y * mat.m12 + vec.z * mat.m13 + vec.w * mat.m14,
//		vec.x * mat.m21 + vec.y * mat.m22 + vec.z * mat.m23 + vec.w * mat.m24,
//		vec.x * mat.m31 + vec.y * mat.m32 + vec.z * mat.m33 + vec.w * mat.m34,
//		vec.x * mat.m41 + vec.y * mat.m42 + vec.z * mat.m43 + vec.w * mat.m44,
//	};
//}
inline float4 operator*(const float4x4& m, const float4& v) 
{ 
	return { 
		m.m11 * v.x + m.m12 * v.y + m.m13 * v.z + m.m14 * v.w, 
		m.m21 * v.x + m.m22 * v.y + m.m23 * v.z + m.m24 * v.w, 
		m.m31 * v.x + m.m32 * v.y + m.m33 * v.z + m.m34 * v.w, 
		m.m41 * v.x + m.m42 * v.y + m.m43 * v.z + m.m44 * v.w 
	}; 
}

/* +++++++++++++++++++++++++++++++++++++++++++++++
		MATRIX x MATRIX MULTIPLICATION
+++++++++++++++++++++++++++++++++++++++++++++++ */

inline float4x4 operator*(const float4x4& a, const float4x4& b) 
{ 
	float4x4 r{};

	r.m11 = a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31 + a.m14 * b.m41;
	r.m12 = a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32 + a.m14 * b.m42;
	r.m13 = a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33 + a.m14 * b.m43;
	r.m14 = a.m11 * b.m14 + a.m12 * b.m24 + a.m13 * b.m34 + a.m14 * b.m44;

	r.m21 = a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31 + a.m24 * b.m41;
	r.m22 = a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32 + a.m24 * b.m42;
	r.m23 = a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33 + a.m24 * b.m43;
	r.m24 = a.m21 * b.m14 + a.m22 * b.m24 + a.m23 * b.m34 + a.m24 * b.m44;

	r.m31 = a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31 + a.m34 * b.m41;
	r.m32 = a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32 + a.m34 * b.m42;
	r.m33 = a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33 + a.m34 * b.m43;
	r.m34 = a.m31 * b.m14 + a.m32 * b.m24 + a.m33 * b.m34 + a.m34 * b.m44;

	r.m41 = a.m41 * b.m11 + a.m42 * b.m21 + a.m43 * b.m31 + a.m44 * b.m41;
	r.m42 = a.m41 * b.m12 + a.m42 * b.m22 + a.m43 * b.m32 + a.m44 * b.m42;
	r.m43 = a.m41 * b.m13 + a.m42 * b.m23 + a.m43 * b.m33 + a.m44 * b.m43;
	r.m44 = a.m41 * b.m14 + a.m42 * b.m24 + a.m43 * b.m34 + a.m44 * b.m44;

	return r; 
}

/* ////////////////////////////////////////////////////
 * 
 * MATRIX	UTILITY FUNCTIONS
 * 
//////////////////////////////////////////////////// */

namespace Optim::Mathematics
{

inline float4 getFloat4FromFloat3(const float3& f) 
{
	return float4{
		f.x,
		f.y,
		f.z,
		1
	};
}

inline float3 getFloat3Part(const float4& f)
{
	return float3 {
		f.x,
		f.y,
		f.z
	};
}

inline float4x4 getMatrixTranslation(float x, float y, float z)
{
	return float4x4 {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
			 x,    y,    z, 1.0f
	};
}

inline float4x4 getMatrixTranslation(const float3& f)
{
	return float4x4 {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		 f.x,   f.y,   f.z, 1.0f
	};
}

/* +++++++++++++++++++++++++++++++++++++++++++++++
		MATRIX DETERMINANT
+++++++++++++++++++++++++++++++++++++++++++++++ */

inline float getMatrixDeterminant(const float2x2& param_matrix)
{
	return (param_matrix.m11 * param_matrix.m22) - (param_matrix.m12 * param_matrix.m21);
}

inline float getMatrixDeterminant(const float3x3& param_matrix)
{	
	float2x2 l_matrix1 = {
		param_matrix.m22, param_matrix.m23,
		param_matrix.m32, param_matrix.m33
	};

	float2x2 l_matrix2 = {
		param_matrix.m21, param_matrix.m23,
		param_matrix.m31, param_matrix.m33
	};

	float2x2 l_matrix3 = {
		param_matrix.m21, param_matrix.m22,
		param_matrix.m31, param_matrix.m32
	};

	return {
		(param_matrix.m11 * getMatrixDeterminant(l_matrix1)) - 
		(param_matrix.m12 * getMatrixDeterminant(l_matrix2)) + 
		(param_matrix.m13 * getMatrixDeterminant(l_matrix3))
	};
}

inline float getMatrixDeterminant(const float4x4& param_matrix)
{
	float3x3 l_matrix1 = {
		param_matrix.m22, param_matrix.m23, param_matrix.m24,
		param_matrix.m32, param_matrix.m33, param_matrix.m34,
		param_matrix.m42, param_matrix.m43, param_matrix.m44,
	};

	float3x3 l_matrix2 = {
		param_matrix.m21, param_matrix.m23, param_matrix.m24,
		param_matrix.m31, param_matrix.m33, param_matrix.m34,
		param_matrix.m41, param_matrix.m43, param_matrix.m44,
	};

	float3x3 l_matrix3 = {
		param_matrix.m21, param_matrix.m22, param_matrix.m24,
		param_matrix.m31, param_matrix.m32, param_matrix.m34,
		param_matrix.m41, param_matrix.m42, param_matrix.m44,
	};

	float3x3 l_matrix4 = {
		param_matrix.m21, param_matrix.m22, param_matrix.m23,
		param_matrix.m31, param_matrix.m32, param_matrix.m33,
		param_matrix.m41, param_matrix.m42, param_matrix.m43,
	};

	return {
		(param_matrix.m11 * getMatrixDeterminant(l_matrix1)) -
		(param_matrix.m12 * getMatrixDeterminant(l_matrix2)) +
		(param_matrix.m13 * getMatrixDeterminant(l_matrix3)) -
		(param_matrix.m14 * getMatrixDeterminant(l_matrix4))
	};
}

/* +++++++++++++++++++++++++++++++++++++++++++++++
		MATRIX MINOR
+++++++++++++++++++++++++++++++++++++++++++++++ */

inline float2x2 getMatrixMinor(const float2x2& matrix)
{
	return {
		matrix.m22, matrix.m21,
		matrix.m12, matrix.m11
	};
}

inline float3x3 getMatrixMinor(const float3x3& matrix)
{
	float2x2 M11 = {
		matrix.m22, matrix.m23,
		matrix.m32, matrix.m33
	};
	float2x2 M12 = {
		matrix.m21, matrix.m23,
		matrix.m31, matrix.m33
	};
	float2x2 M13 = {
		matrix.m21, matrix.m22,
		matrix.m31, matrix.m32
	};

	float2x2 M21 = {
		matrix.m12, matrix.m13,
		matrix.m32, matrix.m33
	};
	float2x2 M22 = {
		matrix.m11, matrix.m13,
		matrix.m31, matrix.m33
	};
	float2x2 M23 = {
		matrix.m11, matrix.m12,
		matrix.m31, matrix.m32
	};

	float2x2 M31 = {
		matrix.m12, matrix.m13,
		matrix.m22, matrix.m23
	};
	float2x2 M32 = {
		matrix.m11, matrix.m13,
		matrix.m21, matrix.m23
	};
	float2x2 M33 = {
		matrix.m11, matrix.m12,
		matrix.m21, matrix.m22
	};

	return {
		getMatrixDeterminant(M11), getMatrixDeterminant(M12), getMatrixDeterminant(M13),
		getMatrixDeterminant(M21), getMatrixDeterminant(M22), getMatrixDeterminant(M23),
		getMatrixDeterminant(M31), getMatrixDeterminant(M32), getMatrixDeterminant(M33)
	};
}

inline float4x4 getMatrixMinor(const float4x4& matrix)
{
	// ROW 1

	float3x3 M11 = {
		matrix.m22, matrix.m23, matrix.m24,
		matrix.m32, matrix.m33, matrix.m34,
		matrix.m42, matrix.m43, matrix.m44,
	};
	float3x3 M12 = {
		matrix.m21, matrix.m23, matrix.m24,
		matrix.m31, matrix.m33, matrix.m34,
		matrix.m41, matrix.m43, matrix.m44,
	};
	float3x3 M13 = {
		matrix.m21, matrix.m22, matrix.m24,
		matrix.m31, matrix.m32, matrix.m34,
		matrix.m41, matrix.m42, matrix.m44,
	};
	float3x3 M14 = {
		matrix.m21, matrix.m22, matrix.m23,
		matrix.m31, matrix.m32, matrix.m33,
		matrix.m41, matrix.m42, matrix.m43,
	};

	// ROW 2

	float3x3 M21 = {
		matrix.m12, matrix.m13, matrix.m14,
		matrix.m32, matrix.m33, matrix.m34,
		matrix.m42, matrix.m43, matrix.m44,
	};
	float3x3 M22 = {
		matrix.m11, matrix.m13, matrix.m14,
		matrix.m31, matrix.m33, matrix.m34,
		matrix.m41, matrix.m43, matrix.m44,
	};
	float3x3 M23 = {
		matrix.m11, matrix.m12, matrix.m14,
		matrix.m31, matrix.m32, matrix.m34,
		matrix.m41, matrix.m42, matrix.m44,
	};
	float3x3 M24 = {
		matrix.m11, matrix.m12, matrix.m13,
		matrix.m31, matrix.m32, matrix.m33,
		matrix.m41, matrix.m42, matrix.m43,
	};

	// ROW 3

	float3x3 M31 = {
		matrix.m12, matrix.m13, matrix.m14,
		matrix.m22, matrix.m23, matrix.m24,
		matrix.m42, matrix.m43, matrix.m44,
	};
	float3x3 M32 = {
		matrix.m11, matrix.m13, matrix.m14,
		matrix.m21, matrix.m23, matrix.m24,
		matrix.m41, matrix.m43, matrix.m44,
	};
	float3x3 M33 = {
		matrix.m11, matrix.m12, matrix.m14,
		matrix.m21, matrix.m22, matrix.m24,
		matrix.m41, matrix.m42, matrix.m44,
	};
	float3x3 M34 = {
		matrix.m11, matrix.m12, matrix.m13,
		matrix.m21, matrix.m22, matrix.m23,
		matrix.m41, matrix.m42, matrix.m43,
	};

	// ROW 3

	float3x3 M41 = {
		matrix.m12, matrix.m13, matrix.m14,
		matrix.m22, matrix.m23, matrix.m24,
		matrix.m32, matrix.m33, matrix.m34,
	};
	float3x3 M42 = {
		matrix.m11, matrix.m13, matrix.m14,
		matrix.m21, matrix.m23, matrix.m24,
		matrix.m31, matrix.m33, matrix.m34,
	};
	float3x3 M43 = {
		matrix.m11, matrix.m12, matrix.m14,
		matrix.m21, matrix.m22, matrix.m24,
		matrix.m31, matrix.m32, matrix.m34,
	};
	float3x3 M44 = {
		matrix.m11, matrix.m12, matrix.m13,
		matrix.m21, matrix.m22, matrix.m23,
		matrix.m31, matrix.m32, matrix.m33,
	};

	return {
		getMatrixDeterminant(M11), getMatrixDeterminant(M12), getMatrixDeterminant(M13), getMatrixDeterminant(M14),
		getMatrixDeterminant(M21), getMatrixDeterminant(M22), getMatrixDeterminant(M23), getMatrixDeterminant(M24),
		getMatrixDeterminant(M31), getMatrixDeterminant(M32), getMatrixDeterminant(M33), getMatrixDeterminant(M34),
		getMatrixDeterminant(M41), getMatrixDeterminant(M42), getMatrixDeterminant(M43), getMatrixDeterminant(M44)
	};
}

/* +++++++++++++++++++++++++++++++++++++++++++++++
		MATRIX COFACTOR
+++++++++++++++++++++++++++++++++++++++++++++++ */

inline float2x2 getMatrixCofactor(const float2x2& matrix)
{
	float2x2 l_minor = getMatrixMinor(matrix);
	return {
		l_minor.m11, -l_minor.m12,
		-l_minor.m21, l_minor.m22
	};
}

inline float3x3 getMatrixCofactor(const float3x3& matrix)
{
	float3x3 l_minor = getMatrixMinor(matrix);

	return {
		l_minor.m11, -l_minor.m12, l_minor.m13,
		-l_minor.m21, l_minor.m22, -l_minor.m23,
		l_minor.m31, -l_minor.m32, l_minor.m33
	};
}

inline float4x4 getMatrixCofactor(const float4x4& matrix)
{
	float4x4 l_minor = getMatrixMinor(matrix);
	return {
		l_minor.m11,	-l_minor.m12,	l_minor.m13, -l_minor.m14,
		-l_minor.m21,	 l_minor.m22, -l_minor.m23,	l_minor.m24,
		l_minor.m31,	-l_minor.m32,	l_minor.m33,	-l_minor.m34,
		-l_minor.m41,	l_minor.m42,	-l_minor.m43,	l_minor.m44,
	};
}

/* +++++++++++++++++++++++++++++++++++++++++++++++
		MATRIX TRANSPOSE
+++++++++++++++++++++++++++++++++++++++++++++++ */

inline float2x2 getMatrixTranspose(const float2x2& matrix)
{
	return {
		matrix.m11, matrix.m21,
		matrix.m12, matrix.m22
	};
}

inline float3x3 getMatrixTranspose(const float3x3& matrix)
{
	return {
		matrix.m11, matrix.m21, matrix.m31,
		matrix.m12, matrix.m22, matrix.m32,
		matrix.m13, matrix.m23, matrix.m33
	};
}

inline float4x4 getMatrixTranspose(const float4x4& matrix)
{
	return {
		matrix.m11, matrix.m21, matrix.m31, matrix.m41,
		matrix.m12, matrix.m22, matrix.m32, matrix.m42,
		matrix.m13, matrix.m23, matrix.m33, matrix.m43,
		matrix.m14, matrix.m24, matrix.m34, matrix.m44,
	};
}

/* +++++++++++++++++++++++++++++++++++++++++++++++
		MATRIX ADJOINT
+++++++++++++++++++++++++++++++++++++++++++++++ */

inline float2x2 getMatrixAdjoint(const float2x2& matrix)
{
	return getMatrixTranspose(getMatrixCofactor(matrix));
}

inline float3x3 getMatrixAdjoint(const float3x3& matrix)
{
	return getMatrixTranspose(getMatrixCofactor(matrix));
}

inline float4x4 getMatrixAdjoint(const float4x4& matrix)
{
	return getMatrixTranspose(getMatrixCofactor(matrix));
}

/* +++++++++++++++++++++++++++++++++++++++++++++++
		MATRIX INVERSE
+++++++++++++++++++++++++++++++++++++++++++++++ */

inline float2x2 getMatrixInverse(const float2x2& matrix)
{
	float det			= getMatrixDeterminant(matrix);
	float2x2 adj	= getMatrixAdjoint(matrix);
	return (1 / det) * adj;
}

inline float3x3 getMatrixInverse(const float3x3& matrix)
{
	float det			= getMatrixDeterminant(matrix);
	float3x3 adj	= getMatrixAdjoint(matrix);
	return (1 / det) * adj;
}

inline float4x4 getMatrixInverse(const float4x4& matrix)
{
	float			det	= getMatrixDeterminant(matrix);
	float4x4 adj	= getMatrixAdjoint(matrix);
	return (1 / det) * adj;
}

} // namepsace Optim::Mathematics - END


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

inline float3 cross(const float3& a, const float3& b)
{
	return{
			(a.y * b.z) - (a.z * b.y),
		-((a.x * b.z) - (a.z * b.x)),
			(a.x * b.y) - (a.y * b.x)
	};
}

inline float dotProduct(const float3& a, const float3& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}