/* ======================================================================================
 *  TestMeshClass.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Math/OptimMathematics.h"
#include "Core/Graphics/Vertex.h"
#include "Core/Graphics/Resource/IGraphicResource.h"
#include "Core/Graphics/Resource/IVertexBuffer.h"
#include "Core/Graphics/Resource/IIndexBuffer.h"

class TestMeshClass final
{
public:
	inline TestMeshClass() = default;
	float3 position{};

	Vertex* vertices{};
	uint32* indices{};

	uint32 vertexCount{};
	uint32 indexCount{};

	IVertexBuffer*	pVertexBuffer{};
	IIndexBuffer*		pIndexBuffer{};

	static inline TestMeshClass createSkinnedCubeTestMeshClass()
	{
		TestMeshClass instance;

		// Vertices
		Vertex vertices[8]{
			{ {-0.5f, -0.5f, -0.5f} , 0.0f, 1.0f, {-0.5f, -0.5f, -0.5f} }, // 0  
			{ { 0.5f, -0.5f, -0.5f} , 1.0f, 1.0f, { 0.5f, -0.5f, -0.5f} }, // 1  
			{ {-0.5f,  0.5f, -0.5f} , 0.0f, 0.0f, {-0.5f,  0.5f, -0.5f} }, // 2  
			{ { 0.5f,  0.5f, -0.5f} , 1.0f, 0.0f, { 0.5f,  0.5f, -0.5f} }, // 3  
			{ {-0.5f, -0.5f,  0.5f} , 1.0f, 1.0f, {-0.5f, -0.5f,  0.5f} }, // 4  
			{ { 0.5f, -0.5f,  0.5f} , 0.0f, 1.0f, { 0.5f, -0.5f,  0.5f} }, // 5  
			{ {-0.5f,  0.5f,  0.5f} , 1.0f, 0.0f, {-0.5f,  0.5f,  0.5f} }, // 6  
			{ { 0.5f,  0.5f,  0.5f} , 0.0f, 0.0f, { 0.5f,  0.5f,  0.5f} }  // 7
		};

		Vertex* pData = new Vertex[24]{
			// Face 1
			{vertices[2].position, { 0.0f, 0.0f }, vertices[2].normal}, // 0
			{vertices[3].position, { 1.0f, 0.0f }, vertices[3].normal}, // 1
			{vertices[0].position, { 0.0f, 1.0f }, vertices[0].normal}, // 2
			{vertices[1].position, { 1.0f, 1.0f }, vertices[1].normal}, // 3
			// Face 2
			{vertices[3].position, { 0.0f, 0.0f }, vertices[3].normal}, // 4
			{vertices[7].position, { 1.0f, 0.0f }, vertices[7].normal}, // 5
			{vertices[1].position, { 0.0f, 1.0f }, vertices[1].normal}, // 6
			{vertices[5].position, { 1.0f, 1.0f }, vertices[5].normal}, // 7
			// Face 3
			{vertices[6].position, { 0.0f, 0.0f }, vertices[6].normal}, // 8
			{vertices[2].position, { 1.0f, 0.0f }, vertices[2].normal}, // 9
			{vertices[4].position, { 0.0f, 1.0f }, vertices[4].normal}, // 10
			{vertices[0].position, { 1.0f, 1.0f }, vertices[0].normal}, // 11
			// Face 4
			{vertices[6].position, { 0.0f, 0.0f }, vertices[6].normal}, // 12
			{vertices[7].position, { 1.0f, 0.0f }, vertices[7].normal}, // 13
			{vertices[2].position, { 0.0f, 1.0f }, vertices[2].normal}, // 14
			{vertices[3].position, { 1.0f, 1.0f }, vertices[3].normal}, // 15
			// Face 5
			{vertices[0].position, { 0.0f, 0.0f }, vertices[0].normal}, // 16
			{vertices[1].position, { 1.0f, 0.0f }, vertices[1].normal}, // 17
			{vertices[4].position, { 0.0f, 1.0f }, vertices[4].normal}, // 18
			{vertices[5].position, { 1.0f, 1.0f }, vertices[5].normal}, // 19
			// Face 6
			{vertices[4].position, { 0.0f, 1.0f }, vertices[4].normal}, // 20
			{vertices[5].position, { 1.0f, 1.0f }, vertices[5].normal}, // 21
			{vertices[6].position, { 0.0f, 0.0f }, vertices[6].normal}, // 22
			{vertices[7].position, { 1.0f, 0.0f }, vertices[7].normal}, // 23
		};

		instance.vertexCount = 24;
		instance.vertices = pData;

		uint32* indexData = new uint32[36]
		{
			2, 0, 3,      1, 3, 0,
			7, 6, 5,      4, 5, 6,
			10, 8, 11,    9, 11, 8,
			12, 13, 14,   15, 14, 13,
			18, 16, 19,   17, 19, 16,
			20, 21, 22,   23, 22, 21
		};

		instance.indexCount = 36;
		instance.indices = indexData;

		return instance;
	}
};