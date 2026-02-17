/* ======================================================================================
 *  Camera.cpp:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#include "Core/Object/Camera/Camera.h"

#include "Core/Math/OptimMathematics.h"
#include "Core/Math/Quaternion.h"

float3     Camera::position{0.0f, 0.0f, 0.0f};
Quaternion Camera::rotation{1.0f, 0.0, 0.0f, 0.0f};

float Camera::pitch{0.0f};
float Camera::yaw{0.0f};
float Camera::roll{0.0f};

float3 Camera::forward{0.0f, 0.0f, 0.0f};
float3 Camera::right{0.0f, 0.0f, 0.0f};
float3 Camera::up{0.0f, 0.0f, 0.0f};
