/* ======================================================================================
 *  SVertex.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"

// Vertex Structure

struct SVertex
{
	struct
	{
		float x;
		float y;
	} position;

	struct
	{
		uint8 r;
		uint8 g;
		uint8 b;
		uint8 a;
	} color;
};

class RectangleMesh
{
public:
	inline RectangleMesh(float _posX, float _posY, float _width, float _height) :
		posX{ _posX }, posY{ _posY }, width{ _width }, height{ _height }
	{ 
		// Top left
		vertices[0].position.x = posX;
		vertices[0].position.y = posY;
		// Bottom Left
		vertices[1].position.x = posX;
		vertices[1].position.y = height;
		// Top Right
		vertices[2].position.x = width;
		vertices[2].position.y = posY;
		// Bottom Right
		vertices[3].position.x = width;
		vertices[3].position.y = height;

		for (int i = 0; i < 4; i++)
		{
			vertices[i].color.r = 255;
			vertices[i].color.g = 255;
			vertices[i].color.b = 255;
			vertices[i].color.a = 255;
		}

		indices = new int[6] { 0, 1, 2, 2, 1, 4 };
	}

	inline ~RectangleMesh()
	{
		delete[] indices;
	}

	float posX;
	float posY;
	float width;
	float height;

	SVertex		vertices[4];
	int*				indices;
};