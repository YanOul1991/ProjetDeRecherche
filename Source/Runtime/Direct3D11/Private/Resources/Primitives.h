/* ======================================================================================
 *  Primitives.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Defines/DirectX/msDx11.h"

#include "DirectX11Resources.h"

#include <vector>
#include <sstream>


constexpr float pi = 3.141592f;

struct SGFXVertex
{
  struct
  {
    float x;
    float y;
    float z;
  } position;

  struct
  {
    float u;
    float v;
  } UVCoord;
};

class Mesh
{
public:
  Mesh() = default;

	float posX{0};
	float posY{0};
	float posZ{0};

  VertexBuffer<SGFXVertex>  vertexBuffer;
  IndexBuffer               indexBuffer;
};

inline Mesh createCubeMesh()
{
  Mesh instance = Mesh();

  /*
  SGFXVertex* pData = new SGFXVertex[8]
  {
    { -0.5f, -0.5f, -0.5f , 0.0f, 1.0f }, // 0  
    {  0.5f, -0.5f, -0.5f , 1.0f, 1.0f }, // 1  
    { -0.5f,  0.5f, -0.5f , 0.0f, 0.0f }, // 2  
    {  0.5f,  0.5f, -0.5f , 1.0f, 0.0f }, // 3  
    { -0.5f, -0.5f,  0.5f , 1.0f, 1.0f }, // 4  
    {  0.5f, -0.5f,  0.5f , 0.0f, 1.0f }, // 5  
    { -0.5f,  0.5f,  0.5f , 1.0f, 0.0f }, // 6  
    {  0.5f,  0.5f,  0.5f , 0.0f, 0.0f }  // 7
  };
  instance.vertexBuffer.data = pData;
  uint16* indexData = new uint16[36]
  {
    0, 2, 1,  2, 3, 1,
    1, 3, 5,  3, 7, 5,
    2, 6, 3,  3, 6, 7,
    4, 5, 7,  4, 7, 6,
    0, 4, 2,  2, 4, 6,
    0, 1, 4,  1, 5, 4
  };
  */

  SGFXVertex vertices[8] {
    { -0.5f, -0.5f, -0.5f , 0.0f, 1.0f }, // 0  
    {  0.5f, -0.5f, -0.5f , 1.0f, 1.0f }, // 1  
    { -0.5f,  0.5f, -0.5f , 0.0f, 0.0f }, // 2  
    {  0.5f,  0.5f, -0.5f , 1.0f, 0.0f }, // 3  
    { -0.5f, -0.5f,  0.5f , 1.0f, 1.0f }, // 4  
    {  0.5f, -0.5f,  0.5f , 0.0f, 1.0f }, // 5  
    { -0.5f,  0.5f,  0.5f , 1.0f, 0.0f }, // 6  
    {  0.5f,  0.5f,  0.5f , 0.0f, 0.0f }  // 7
  };

  SGFXVertex* pData = new SGFXVertex[24] {
    // Face 1
    {vertices[2].position, { 0.0f, 0.0f }}, // 0
    {vertices[3].position, { 1.0f, 0.0f }}, // 1
    {vertices[0].position, { 0.0f, 1.0f }}, // 2
    {vertices[1].position, { 1.0f, 1.0f }}, // 3
    // Face 2
    {vertices[3].position, { 0.0f, 0.0f }}, // 4
    {vertices[7].position, { 1.0f, 0.0f }}, // 5
    {vertices[1].position, { 0.0f, 1.0f }}, // 6
    {vertices[5].position, { 1.0f, 1.0f }}, // 7
    // Face 3
    {vertices[6].position, { 0.0f, 0.0f }}, // 8
    {vertices[2].position, { 1.0f, 0.0f }}, // 9
    {vertices[4].position, { 0.0f, 1.0f }}, // 10
    {vertices[0].position, { 1.0f, 1.0f }}, // 11
    // Face 4
    {vertices[6].position, { 0.0f, 0.0f }}, // 12
    {vertices[7].position, { 1.0f, 0.0f }}, // 13
    {vertices[2].position, { 0.0f, 1.0f }}, // 14
    {vertices[3].position, { 1.0f, 1.0f }}, // 15
    // Face 5
    {vertices[0].position, { 0.0f, 0.0f }}, // 16
    {vertices[1].position, { 1.0f, 0.0f }}, // 17
    {vertices[4].position, { 0.0f, 1.0f }}, // 18
    {vertices[5].position, { 1.0f, 1.0f }}, // 19

    // Face 6
    {vertices[4].position, { 0.0f, 0.0f }}, // 20
    {vertices[5].position, { 1.0f, 0.0f }}, // 21
    {vertices[6].position, { 0.0f, 1.0f }}, // 22
    {vertices[7].position, { 1.0f, 1.0f }}, // 23
  };

  instance.vertexBuffer.data = pData;

  uint16* indexData = new uint16[36]
  {
    2, 0, 3,      1, 3, 0,
    7, 6, 5,      4, 5, 6,
    10, 8, 11,    9, 11, 8,
    12, 13, 14,   15, 14, 13,
    18, 16, 19,   17, 19, 16,
    20, 21, 22,   23, 22, 21
  };

  instance.vertexBuffer = VertexBuffer(pData, sizeof(SGFXVertex[24]));
  instance.indexBuffer = IndexBuffer(indexData, sizeof(uint16[36]));

  return instance;
}

inline Mesh createFlatCircle(int precision = 0) 
{
  //std::stringstream ss;
  Mesh instance = Mesh();

  int steps = 3 + precision;
  float radVar = (2 * pi) / steps;

  std::vector<SGFXVertex> verts;

  for (int i = 0; i < steps; i++)
  {
    SGFXVertex v{};
    float rad = (pi / 2) + ((2.0f * pi / steps) * i);

    v.position.x = cos(rad);
    v.position.y = sin(rad);
    v.position.z = 0;

    verts.push_back(v);
  }

  SGFXVertex* pVerts = new SGFXVertex[verts.size()];
  memcpy(pVerts, verts.data(), sizeof(SGFXVertex) * verts.size());

  std::vector<float2> points2d(verts.size());


  for (int i = 0; i < verts.size(); i++)
  {
    points2d[i].x = verts[i].position.x;
    points2d[i].y = verts[i].position.y;
  }

  std::vector<uint16> indices = triangulateEarClip(points2d);

  uint16* pIndexData = new uint16[indices.size()];
  memcpy(pIndexData, indices.data(), sizeof(uint16) * indices.size());

  instance.vertexBuffer = VertexBuffer(pVerts, sizeof(SGFXVertex) * (int)verts.size());
  instance.indexBuffer = IndexBuffer(pIndexData,  sizeof(uint16) * static_cast<int>(indices.size()));

  return instance;
}