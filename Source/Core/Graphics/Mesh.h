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

#include "Core/Object/Object.h"

class CORE_API Mesh final : public Object
{
	DECLARE_OBJECT(Mesh)
public:

	Mesh() = default;
	virtual ~Mesh() noexcept override final;

	float4x4 getWorldMatrix() const;

	float3			position{};
	Quaternion	rotation{};
	Vertex*			vertices{};
	uint32*			indices{};
	uint32			vertexCount{};
	uint32			indexCount{};

	VertexBufferHandle	vertexBufferHandle {};
	IndexBufferHandle		indexBufferHandle {};
};