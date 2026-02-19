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
class CORE_API Camera final
{
 public:
  static float3     position;
  static Quaternion rotation;
  static float      pitch; // The X rotation of the camera.
  static float      yaw;   // The Y rotation of the camera.
  static float      roll;  // The Z rotation of the camera.
  static float3     forward;
  static float3     right;
  static float3     up;

  static float4x4 getViewMatrix();

  static float4x4 getProjectionMatrix();
};