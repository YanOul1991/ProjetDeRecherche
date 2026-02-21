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

float3 Camera::forward{0.0f, 0.0f, -1.0f};
float3 Camera::right{1.0f, 0.0f, 0.0f};
float3 Camera::up{0.0f, 1.0f, 0.0f};

float4x4 Camera::getViewMatrix() {
  return float4x4 {
    Camera::right.x, Camera::up.x, -Camera::forward.x, 0, 
    Camera::right.y, Camera::up.y, -Camera::forward.y, 0, 
    Camera::right.z, Camera::up.z, -Camera::forward.z, 0, 
    -dotProduct(Camera::right, Camera::position), -dotProduct(Camera::up, Camera::position), dotProduct(Camera::forward, Camera::position), 1
  };
}

float4x4 Camera::getProjectionMatrix() {
  return float4x4();
}


