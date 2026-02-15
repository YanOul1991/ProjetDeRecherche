/* ======================================================================================
 *  TestMeshClass.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Graphics/Vertex.h"
#include "Core/Graphics/Resource/IGraphicResource.h"
#include "Core/Graphics/Resource/GraphicResourceHandle.h"

#include "Core/System/FileStream.h"

#include "Core/Math/OptimMathematics.h"
#include "Core/Math/Quaternion.h"

#include <string>

#include "Core/Object/Object.h"

class CORE_API Mesh : public Object
{
  DECLARE_OBJECT()

 public:
  Mesh();
  virtual ~Mesh() noexcept override;

  float4x4 getWorldMatrix() const;

  float3     position{};
  Quaternion rotation{};

  std::string sourcePath{""};

  Vertex*    vertices{};
  uint32*    indices{};

  uint32     vertexCount{};
  uint32     indexCount{};

  VertexBufferHandle vertexBufferHandle{};
  IndexBufferHandle  indexBufferHandle{};
};

/*
class CORE_API SkinnedMesh : public Mesh
{
  DECLARE_OBJECT()

 public:
   float skinnedMeshValue;
   float3 scale;
   std::vector<float3> listOfStuff;
   MyCustomEnum        customEnum{};
};
*/