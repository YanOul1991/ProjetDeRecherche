/* ======================================================================================
 *  Camera.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/Math/OptimMathematics.h"
#include "Core/Math/Quaternion.h"
#include "Core/OptimEngine.h"

/*
 * @brief
 * Camera class for scene render view.
 */
class Camera final
{
 public:
  CORE_API static float3     position;
  CORE_API static Quaternion rotation;

  CORE_API static float pitch; // The X rotation of the camera.
  CORE_API static float yaw;   // The Y rotation of the camera.
  CORE_API static float roll;  // The Z rotation of the camera.

  CORE_API static float3 forward;
  CORE_API static float3 right;
  CORE_API static float3 up;
};