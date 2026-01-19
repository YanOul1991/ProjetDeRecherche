/* ======================================================================================
 *  Primitives.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Math/OptimMathematics.h"
#include "Core/Graphics/Vertex.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Private/Resources/Buffer/DirectX11Buffer.h"

class Mesh
{
public:
  Mesh() = default;

	float posX{0};
	float posY{0};
	float posZ{0};

  VertexBuffer<Vertex>  vertexBuffer;
  IndexBuffer           indexBuffer;
};

inline Mesh createCubeMesh() {
  Mesh instance = Mesh();

  Vertex vertices[8] {
    { -0.5f, -0.5f, -0.5f , 0.0f, 1.0f }, // 0  
    {  0.5f, -0.5f, -0.5f , 1.0f, 1.0f }, // 1  
    { -0.5f,  0.5f, -0.5f , 0.0f, 0.0f }, // 2  
    {  0.5f,  0.5f, -0.5f , 1.0f, 0.0f }, // 3  
    { -0.5f, -0.5f,  0.5f , 1.0f, 1.0f }, // 4  
    {  0.5f, -0.5f,  0.5f , 0.0f, 1.0f }, // 5  
    { -0.5f,  0.5f,  0.5f , 1.0f, 0.0f }, // 6  
    {  0.5f,  0.5f,  0.5f , 0.0f, 0.0f }  // 7
  };

  Vertex* pData = new Vertex[24] {
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
    {vertices[4].position, { 0.0f, 1.0f }}, // 20
    {vertices[5].position, { 1.0f, 1.0f }}, // 21
    {vertices[6].position, { 0.0f, 0.0f }}, // 22
    {vertices[7].position, { 1.0f, 0.0f }}, // 23
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

  instance.vertexBuffer = VertexBuffer(pData, sizeof(Vertex[24]));
  instance.indexBuffer = IndexBuffer(indexData, sizeof(uint16[36]));

  return instance;
}