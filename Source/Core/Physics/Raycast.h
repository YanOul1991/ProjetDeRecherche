#pragma once

#include "Core/Math/OptimMathematics.h"
#include "Core/OptimEngine.h"

struct Collision {

};

struct Raycast {
  inline Raycast(const float3& _origin, const float3& _farPos, const float3& _direction) :
      origin(_origin),
      farPosition(_farPos),
      direction(_direction) {
  }

  const float3 origin;
  const float3 farPosition;
  const float3 direction;
};