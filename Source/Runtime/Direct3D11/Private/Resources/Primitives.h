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
#include "DirectX11Buffer.h"
#include "DirectX11Shader.h"

struct SGFXVertex
{
  struct
  {
    float x;
    float y;
    float z;
  } position;
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

  SGFXVertex* pData = new SGFXVertex[8]
  {
    { -0.5f, -0.5f, -0.5f }, // 0  
    {  0.5f, -0.5f, -0.5f }, // 1  
    { -0.5f,  0.5f, -0.5f }, // 2  
    {  0.5f,  0.5f, -0.5f }, // 3  
    { -0.5f, -0.5f,  0.5f }, // 4  
    {  0.5f, -0.5f,  0.5f }, // 5  
    { -0.5f,  0.5f,  0.5f }, // 6  
    {  0.5f,  0.5f,  0.5f }  // 7
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

  instance.vertexBuffer = VertexBuffer(pData, sizeof(SGFXVertex[8]));
  instance.indexBuffer = IndexBuffer(indexData, sizeof(uint16[36]));

  return instance;
}