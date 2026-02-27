/*
 * Physics.cpp
 */

#include "./Physics.h"

#include "Core/Math/OptimMathematics.h"
#include "Core/Object/Camera/Camera.h"

Raycast Optim::Physics::ScreenToRaycast(float posX, float posY, float width, float height) {
  float ndcX = (2 * (posX) / static_cast<float>(width)) - 1.0f;
  float ndcY = 1.0f - (2 * (posY) / static_cast<float>(height));

  float4 nearPoint = { ndcX, ndcY, 0.0f, 1.0f };
  float4 farPoint  = { ndcX, ndcY, 1.0f, 1.0f };

  // Manually compute projectionMatrix.
  // Eventually, Camera class should have a function to output the result.

  const float a = (float)width / float(height);     // aspect ratio ratio
  constexpr float fov    = mathConst::PI / 3.0f;    // Field of view
  constexpr float n      = 0.1f;                    // near clip
  constexpr float f      = 1000.0f;                 // far clip
  const float     yScale = 1.0f / (tan(fov / 2.0f));

  float4x4 perspectiveMatrix = float4x4{
    yScale / a, 0, 0, 0, 
    0, yScale, 0, 0, 0,
    0, f / (n - f), -1, 
    0, 0, (n * f) / (n - f), 0
  };

  perspectiveMatrix = Optim::Mathematics::getMatrixTranspose(perspectiveMatrix);

  float4x4 viewMatrix = Camera::getViewMatrix().transpose();

  float4x4 viewProjectionInverse = Optim::Mathematics::getMatrixInverse(perspectiveMatrix * viewMatrix);

  float4 posNear = viewProjectionInverse * nearPoint;
  float4 posFar  = viewProjectionInverse * farPoint;

  float3 rayOrigin      = { posNear.x / posNear.w, posNear.y / posNear.w, posNear.z / posNear.w };
  float3 rayFarPosition = { posFar.x / posFar.w, posFar.y / posFar.w, posFar.z / posFar.w };

  float3 rayDirection = normalize(rayFarPosition - rayOrigin);

  return Raycast(rayOrigin, rayFarPosition, rayDirection);
}
