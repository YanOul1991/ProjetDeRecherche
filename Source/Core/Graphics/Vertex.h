/* ======================================================================================
 *  SVertex.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Math/OptimMathematics.h"

struct Vertex
{
  float3 position;
  UVCoord uvCoord;
  float3 normal{0, 0, 0};

  void print() const {
    printf("---- Position: <%f, %f, %f>\n---- UV coord: <%f, %f>\n---- Normal  : <%f, %f, %f>\n", 
           position.x, 
           position.y, 
           position.z, 
           uvCoord.u,
           uvCoord.v,
           normal.x, 
           normal.y, 
           normal.z
    );
  }
};