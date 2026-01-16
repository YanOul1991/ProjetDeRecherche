/* ======================================================================================
 *  Quaternion.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Math/OptimMathematics.h"

//#include <iostream>
//#include <cmath>
#include <math.h>
#include <algorithm>

/*
 * @brief 
 * A representation of a rotation on an axis.
*/
struct Quaternion {
	float w{1.0f};
	float x{1.0f};
	float y{1.0f};
	float z{1.0f};

	static Quaternion fromAxisAngle(const float3& axis, float rad) {
		float half = 0.5f * rad;
		float s = sinf(half);
		float3 n = normalize(axis);

		return {
			cosf(half),
			n.x * s,
			n.y * s,
			n.z * s
		};
	}

	/*
	 * @brief
	 * Applies the quaternion's rotation to a vector.
	 * 
	 * @param v
	 * A float3 to rotate.
	 * 
	 * @returns
	 * The value of the rotated vector.
	*/
	inline float3 rotate(const float3& v) const {
		// Get Vector part from quaternion
		float3 qv = { x, y, z };

		float3 t = 2.0f * cross(qv, v);

		return v + w * t + cross(qv, t);
	}

	/*
	 * @brief
	 * Returns the identify quaternion: <w, x, y, z>(1, 0, 0, 0)
	*/
	static Quaternion identity() {
		return {
			1.0f, 
			0.0f, 
			0.0f, 
			0.0f
		};
	}

	/*
	 * @brief
	 * Transforms a quaternion into euler angles in radians
	*/
	inline void toEuler(float& pitch, float& yaw, float& roll) const {
		// Pitch (x-axis)
		float sinp = 2.0f * (w * x + y *z);
		float cosp = 1.0f - 2.0f * (x * x + y * y);
		pitch = atan2f(sinp, cosp);

		// Yaw (y-axis)
		float siny = 2.0f * (w * y - z * x);
		siny = std::clamp(siny, -1.0f, 1.0f);
		yaw = asinf(siny);

		// Roll (z-axis)
		float sinr = 2.0f * (w * z + x * y);
		float cosr = 1.0f - 2.0f * (y * y + z * z);
		roll = atan2f(sinr, cosr);
	}
};

/*
 * @brief 
 * Quaternion multiplication
*/
inline Quaternion operator*(const Quaternion& P, const Quaternion& Q) {
	return {
		(P.w * Q.w) - (P.x * Q.x) - (P.y * Q.y) - (P.z * Q.z),
		(P.w * Q.x)	+ (P.x * Q.w) + (P.y * Q.z) - (P.z * Q.y),
		(P.w * Q.y) - (P.x * Q.z) + (P.y * Q.w) + (P.z * Q.x),
		(P.w * Q.z) + (P.x * Q.y) - (P.y * Q.x) + (P.z * Q.w)
	};
}
