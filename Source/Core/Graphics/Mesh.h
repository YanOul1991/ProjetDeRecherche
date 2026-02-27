
#pragma once

#include "Core/Graphics/Resource/GraphicResourceHandle.h"
#include "Core/Graphics/Vertex.h"
#include "Core/Math/OptimMathematics.h"
#include "Core/Math/Quaternion.h"
#include "Core/Object/Object.h"
#include "Core/OptimEngine.h"
#include "Core/System/FileStream.h"

#include <string>

#define UPROPERTY()

class CORE_API Mesh : public Object
{
  DECLARE_OBJECT()

 public:
  Mesh();
  virtual ~Mesh() noexcept override;

  float4x4 getWorldMatrix() const;

  float3     position{};
  Quaternion rotation{};

  std::string sourcePath{ "" };
  std::string texturePath{ "" };

  Vertex* vertices{};
  uint32* indices{};

  uint32 vertexCount{};
  uint32 indexCount{};

  VertexBufferHandle    vertexBufferHandle{};
  IndexBufferHandle     indexBufferHandle{};
  TextureResourceHandle textureHandle{};
};