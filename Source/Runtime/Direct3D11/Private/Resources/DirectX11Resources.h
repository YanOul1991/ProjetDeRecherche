/* ======================================================================================
 *  DirectX11Resources.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "DirectX11Shader.h"
#include "DirectX11Buffer.h"
#include "Primitives.h"


/*
#include <iostream>
#include <vector>

struct float2 {
	float x;
	float y;
};

inline float cross(const float2& a, const float2& b, const float2& c)
{
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

inline bool pointInTriangle(const float2& point, const float2& a, const float2& b, const float2& c)
{
	float c1 = cross(a, b, point);
	float c2 = cross(b, c, point);
	float c3 = cross(c, a, point);

	return (c1 >= 0 && c2 >= 0 && c3 >= 0);
}

inline std::vector<uint16> triangulate(const std::vector<float2>& polygon)
{
	std::vector<uint16> indices;
	std::vector<uint16> verts(polygon.size());

	for (int i = 0; i < verts.size(); i++)
		verts[i] = i;

	while (verts.size() > 3)
	{
		bool found = false;

		for (int i = 0; i < verts.size(); i++)
		{
			unsigned i0 = verts[(i + verts.size() - 1) % verts.size()];
			unsigned i1 = verts[i];
			unsigned i2 = verts[(i + 1) % verts.size()];

			const float2& a = polygon[i0];
			const float2& b = polygon[i1];
			const float2& c = polygon[i2];

			// check of convex
			if (cross(a, b, c) <= 0) continue;

			// check if another point is inside the triangle
			bool containsPoint = false;
			for (int j = 0; j < verts.size(); j++)
			{
				if (j == i || j == (i + 1) % verts.size() || j == (i + verts.size() - 1) % verts.size())
					continue;

				if (pointInTriangle(polygon[verts[j]], a, b, c))
				{
					containsPoint = true;
					break;
				}
			}

			if (containsPoint)
				continue;

			indices.push_back(i0);
			indices.push_back(i1);
			indices.push_back(i2);

			verts.erase(verts.begin() + 1);
			found = true;
			break;
		}

		if (!found)
			break;
	}
	if (verts.size() == 3)
	{
		indices.push_back(verts[0]);
		indices.push_back(verts[1]);
		indices.push_back(verts[2]);
	}
	return indices;
}
*/
