/* ======================================================================================
 *  DirectX11Resources.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "DirectX11Shader.h"
#include "DirectX11Buffer.h"

#include <iostream>
#include <vector>
#include <sstream>

struct float2 {
	float x;
	float y;
};

inline float2 getVector(float2& start, float2& end)
{
	return { end.x - start.x, end.y - start.y };
}

inline float getMagnitude(float2& vector)
{
	return sqrtf((vector.x * vector.x) + (vector.y * vector.y));
}

inline float getDot(float2& v1, float2& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y);
}

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

inline std::vector<uint16> triangulateEarClip(std::vector<float2>& points)
{
	std::stringstream ss;

	std::vector<uint16> indices;										// Indices list to be returned
	std::vector<uint16> vertIndex(points.size());		// Ordered list of all vertex indices (indices to compute)

	for (int i = 0; i < vertIndex.size(); i++)
		vertIndex[i] = i;

	// Loop until only 3 vertex indices are left to compute
	while (vertIndex.size() > 3)
	{
		bool earFound = false;

		// loop through all indices left to compute
		for (int i = 0; i < vertIndex.size(); i++)
		{
			uint16 currentIndex = vertIndex[i];
			uint16 lastindex		= vertIndex[(i + vertIndex.size() - 1) % vertIndex.size()];
			uint16 nextIndex = vertIndex[(static_cast<unsigned long long>(i) + 1) % vertIndex.size()];

			//ss << "Checking following indices\n" << lastindex << "\n" << currentIndex << "\n" << nextIndex << "\n";

			//MessageBoxA(0, ss.str().c_str(), "DEBUG", 0);
			// Check if the current index and its two 
			// other indices form a convex angle

			float2 v1 = getVector(points[currentIndex], points[lastindex]);
			float2 v2 = getVector(points[currentIndex], points[nextIndex]);

			float angle = acosf((getDot(v1, v2)) / (getMagnitude(v1) * getMagnitude(v2)));

			// If not convex continue
			if (angle >= 180) continue;

			// Check if the triangle contains any other point inside
			bool containsPoint = false;

			for (int j = 0; j < vertIndex.size(); j++)
			{
				// Skip if comparing current vertices
				if (j == i || j == (static_cast<unsigned long long>(i) + 1) % vertIndex.size() || j == (i + vertIndex.size() - 1) % vertIndex.size()) continue;

				if (pointInTriangle(points[vertIndex[j]], points[lastindex], points[currentIndex], points[nextIndex]))
				{
					containsPoint = true;
					break;
				}
			}
			
			// If the current triangle contains another vertex inside 
			// then the ear is not valid and continue checking next index;
			if (containsPoint) continue;

			// If passed checks add three indices to make triangle
			indices.push_back(nextIndex);
			indices.push_back(currentIndex);
			indices.push_back(lastindex);

			// Remove current index from list to compute
			vertIndex.erase(vertIndex.begin() + i);
			//MessageBoxA(0, "Erasing current index", "DEBUG", 0);

			earFound = true;

			break;
		}

		if (!earFound) break;
	}

	if (vertIndex.size() == 3)
	{
		indices.push_back(vertIndex[0]);
		indices.push_back(vertIndex[2]);
		indices.push_back(vertIndex[1]);
	}

	return indices;
}

//inline std::vector<uint16> triangulate(const std::vector<float2>& polygon)
//{
//	std::vector<uint16> indices;
//	std::vector<uint16> verts(polygon.size());
//
//	for (int i = 0; i < verts.size(); i++)
//		verts[i] = i;
//
//	while (verts.size() > 3)
//	{
//		bool found = false;
//
//		for (int i = 0; i < verts.size(); i++)
//		{
//			unsigned i0 = verts[(i + verts.size() - 1) % verts.size()];
//			unsigned i1 = verts[i];
//			unsigned i2 = verts[(i + 1) % verts.size()];
//
//			const float2& a = polygon[i0];
//			const float2& b = polygon[i1];
//			const float2& c = polygon[i2];
//
//			// check of convex
//			if (cross(a, b, c) <= 0) continue;
//
//			// check if another point is inside the triangle
//			bool containsPoint = false;
//			for (int j = 0; j < verts.size(); j++)
//			{
//				if (j == i || j == (i + 1) % verts.size() || j == (i + verts.size() - 1) % verts.size())
//					continue;
//
//				if (pointInTriangle(polygon[verts[j]], a, b, c))
//				{
//					containsPoint = true;
//					break;
//				}
//			}
//
//			if (containsPoint)
//				continue;
//
//			indices.push_back(i0);
//			indices.push_back(i1);
//			indices.push_back(i2);
//
//			verts.erase(verts.begin() + 1);
//			found = true;
//			break;
//		}
//
//		if (!found)
//			break;
//	}
//	if (verts.size() == 3)
//	{
//		indices.push_back(verts[0]);
//		indices.push_back(verts[1]);
//		indices.push_back(verts[2]);
//	}
//	return indices;
//}
