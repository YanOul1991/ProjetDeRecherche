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

#include "Core/Object/Object.h"

#include "Core/Math/OptimMathematics.h"
#include "Core/Math/Quaternion.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <unordered_map>
#include <set>

class Mesh final : public Object
{
public:
	OE_OBJECT(Mesh, Object)

	inline Mesh() = default;
	inline virtual ~Mesh() noexcept override final{}

	inline float4x4 getWorldMatrix() const
	{
		float4x4 l_translation = Optim::Mathematics::getMatrixTranslation(position);
		float4x4 l_rotation		 = Optim::Mathematics::getMatrixFromQuaternion(rotation);
		return l_rotation * l_translation;
	}

	float3			position{};
	Quaternion	rotation{};
	Vertex*			vertices{};
	uint32*			indices{};
	uint32			vertexCount{};
	uint32			indexCount{};

	VertexBufferHandle	vertexBufferHandle	{};
	IndexBufferHandle		indexBufferHandle		{};



	/*
	* @brief
	* Read data for a mesh object from an ".obj". Sets up the vertex buffer
	* and the index buffer of a Mesh class object. The function only fills
	* those two fields it does not make any call to a graphic module to get the
	* graphic resource handles.
	* 
	* @param meshObj
	* lvalue reference to a Mesh object, to fill the vertex and index buffers.
	* 
	* @param filename
	* The relative path of the obj file.
	*/
	static inline void setMeshFromOBJFile(Mesh& meshObj, const char* filename) 
	{
		std::ifstream input(filename);

		if (!input.is_open()) {
			printf("Cannot open file at %s\n", filename);
			return;
		}

		std::vector<Vertex>		vertices{};
		std::vector<float3>		_vectorPositions{};
		std::vector<UVCoord>	_vectorUvCoord{};
		std::vector<float3>   _vectorNormals{};
		std::vector<uint32>		_indices{};
		std::unordered_map<std::string, uint32> _mapFaceTriplets{};
		std::set<std::string> _setInstanciatedTriplets{};

		std::string line;

		uint32 _indicesCount{};

		while (std::getline(input, line)) {
			if (line.c_str()[0] == '#') {
				continue;
			}

			// Get all vertices data
			const char* objName		= "o "; // Mesh object name
			const char* vertex		= "v "; // Vertex position
			const char* normal		= "vn"; // Vertex Normals
			const char* vertexUv	= "vt"; // Vertex UV position
			const char* faceData	= "f "; // Faces data

			/*
				* Read each line representing a vertex position.
				* 
				* Each v line will only read 3 float value for
				* position x, y, z if extra chars are after the 
				* z value, they will be ignored.
				* 
				* The position is then added to a list of positions
				* to be computed later, they are added in the same 
				* order thei are red from the file.
			*/
			if (memcmp(vertex, line.c_str(), 2) == 0) {
				const char* ptr_start = line.c_str() + 2;
				const char* ptr_y;
				const char* ptr_z;
				const char* ptr_end = line.c_str() + line.size();

				ptr_y = strchr(ptr_start, ' ') + 1;
				ptr_z = strchr(ptr_y, ' ') + 1;

				errno = 0;

				float posX = strtof(ptr_start, (char**)(&ptr_y));
				float posY = strtof(ptr_y, (char**)(&ptr_z));
				float posZ = strtof(ptr_z, (char**)(&ptr_end));
				_vectorPositions.push_back({posX, posY, posZ});
			}

			/*
			* Read each line representing a vertex normal.
			* 
			* Each vn line only read the first three floats
			* it finds ignores the rest.
			* 
			* The normal is then added to a list of normals 
			* to be computed later, they are added in the same
			* order they are red from the file.
			*/
			if (memcmp(normal, line.c_str(), 2) == 0) {
				const char* ptr_start = line.c_str() + 3;
				const char* ptr_y;
				const char* ptr_z;
				const char* ptr_end = line.c_str() + line.size();

				ptr_y = strchr(ptr_start, ' ') + 1;
				ptr_z = strchr(ptr_y, ' ') + 1;

				errno = 0;

				float norm_x = strtof(ptr_start, (char**)(&ptr_y));
				float norm_y = strtof(ptr_y, (char**)(&ptr_z));
				float norm_z = strtof(ptr_z, (char**)(&ptr_end));

				//printf("Getting normal data | (%f, %f, %f)\n", norm_x, norm_y, norm_z);
				_vectorNormals.push_back({norm_x, norm_y, norm_z});
			}

			/*
			* Read each line representing a UV coordinate.
			* 
			* Each vt line only read the first two floats
			* it finds ignores the rest.
			* 
			* The UV coord is then added to a list of UV coords
			* to be computed later, they are added in the same
			* order they are red from the file.
			*/
			if (memcmp(vertexUv, line.c_str(), 2) == 0) {
				const char* ptr_u = line.c_str() + 3;
				const char* ptr_v;
				const char* ptr_end = line.c_str() + line.size();

				ptr_v = strchr(ptr_u, ' ') + 1;

				errno = 0;

				float text_u = strtof(ptr_u, (char**)(&ptr_v));
				float text_v = strtof(ptr_v, (char**)(&ptr_end));

				//printf("UV Value (%f, %f)\n", text_u, text_v);
				_vectorUvCoord.push_back({text_u, text_v});
			}

			/* 
				* Read face data to create final vertices and index them.
			*/
			if (memcmp(faceData, line.c_str(), 2) == 0) {
				// Make a copy of the string line
				uint64 str_size = line.size() + 1;
				char* str_line = new char[str_size - 2];

				memcpy(str_line, line.c_str() + 2, str_size - 2);

				// A list of all triplets in the current reading face;
				std::vector<char*> vector_pCstrVerts;

				// Add the start of the line as the first vertex triplet
				vector_pCstrVerts.push_back(str_line);

				// Get all triplets in face defintion
				char* ptr_next = str_line;
				while (ptr_next != nullptr) {

					ptr_next = strchr(ptr_next, ' ');

					if (ptr_next != nullptr) {
						*ptr_next = '\0';
						ptr_next++;
						vector_pCstrVerts.push_back(ptr_next);
					}
				}

				/*
				 * If the face is quad add vertex indices as such:
				*/
				if (vector_pCstrVerts.size() == 4) {
					int ind[6] = {
						0, 2, 1,
						0, 3, 2
					};

					int winding[4] {
						0, 2, 1, 3
					};

					for (int i = 0; i < 6; i++) {
						auto search = _mapFaceTriplets.find(vector_pCstrVerts[ind[i]]);
						if (search == _mapFaceTriplets.end()) {
							_mapFaceTriplets.emplace(std::make_pair(vector_pCstrVerts[ind[i]], _indicesCount));
							_indicesCount++;
						}
						_indices.push_back(_mapFaceTriplets[vector_pCstrVerts[ind[i]]]);
					}

					char* cstr_vposition;	// Vertex position value
					char* cstr_normal;		// Vertex normal value
					char* cstr_uvcoord;		// Vertex UV value

					for (int i = 0; i < vector_pCstrVerts.size(); i++) {
						auto search = _setInstanciatedTriplets.find(vector_pCstrVerts[winding[i]]);

						if (search != _setInstanciatedTriplets.end()) {
							//printf("Triplet already existing: %s\n", vector_pCstrVerts[winding[i]]);
							continue;
						}
						else {
							//printf("creating new Triplet: %s\n", vector_pCstrVerts[winding[i]]);
							_setInstanciatedTriplets.insert(vector_pCstrVerts[winding[i]]);
						}

						cstr_vposition = vector_pCstrVerts[winding[i]];

						cstr_uvcoord = strchr(cstr_vposition, '/');
						*cstr_uvcoord = '\0';
						cstr_uvcoord++;

						cstr_normal		= strchr(cstr_uvcoord, '/');
						*cstr_normal = '\0';
						cstr_normal++;

						try {
							int index_v		= std::stoi(cstr_vposition);
							int index_vt	= std::stoi(cstr_uvcoord);
							int index_vn	= std::stoi(cstr_normal);
							//printf("v: %d | vt %d | vn %d\n", index_v, index_vt, index_vn);

							//printf("vcount %llu | vt count %llu | vn count %llu", _vectorPositions.size(), _vectorUvCoord)

							Vertex _newVert {
								.position = _vectorPositions[index_v - 1],
								.uvCoord	= _vectorUvCoord[index_vt - 1],
								.normal		= _vectorNormals[index_vn - 1]
							};

							vertices.push_back(_newVert);
						}
						catch (const std::exception& e) {
							std::cerr << "Error: could not convert to integer\n" << e.what() << '\n';
						}
					} // vertex instanciate loop end
				} // Enf of if for eching if face is a quad

				/* ----------------------------------------------------
				 * If the face is a triangle
				---------------------------------------------------- */
				if (vector_pCstrVerts.size() == 3) {
					int winding[3]{
						0, 2, 1
					};

					/*
					 * Add vertices and indices in the order 
					 * at which they appear in the face description.
					*/
					for (int i = 0; i < 3; i++) {
						auto search = _mapFaceTriplets.find(vector_pCstrVerts[winding[i]]);
						if (search == _mapFaceTriplets.end()) {
							_mapFaceTriplets.emplace(std::make_pair(vector_pCstrVerts[winding[i]], _indicesCount));
							_indicesCount++;
						}
						_indices.push_back(_mapFaceTriplets[vector_pCstrVerts[winding[i]]]);
					}

					char* cstr_vposition;	// Vertex position value
					char* cstr_normal;		// Vertex normal value
					char* cstr_uvcoord;		// Vertex UV value

					for (int i = 0; i < vector_pCstrVerts.size(); i++) {
						auto search = _setInstanciatedTriplets.find(vector_pCstrVerts[winding[i]]);

						if (search != _setInstanciatedTriplets.end()) {
							//printf("Triplet already existing: %s\n", vector_pCstrVerts[winding[i]]);
							continue;
						}
						else {
							//printf("creating new Triplet: %s\n", vector_pCstrVerts[winding[i]]);
							_setInstanciatedTriplets.insert(vector_pCstrVerts[winding[i]]);
						}

						cstr_vposition = vector_pCstrVerts[winding[i]];

						cstr_uvcoord = strchr(cstr_vposition, '/');
						*cstr_uvcoord = '\0';
						cstr_uvcoord++;

						cstr_normal		= strchr(cstr_uvcoord, '/');
						*cstr_normal = '\0';
						cstr_normal++;

						try {
							int index_v		= std::stoi(cstr_vposition);
							int index_vt	= std::stoi(cstr_uvcoord);
							int index_vn	= std::stoi(cstr_normal);
							//printf("v: %d | vt %d | vn %d\n", index_v, index_vt, index_vn);

							//printf("vcount %llu | vt count %llu | vn count %llu", _vectorPositions.size(), _vectorUvCoord)

							Vertex _newVert {
								.position = _vectorPositions[index_v - 1],
								.uvCoord	= _vectorUvCoord[index_vt - 1],
								.normal		= _vectorNormals[index_vn - 1]
							};

							vertices.push_back(_newVert);
						}
						catch (const std::exception& e) {
							std::cerr << "Error: could not convert to integer\n" << e.what() << '\n';
						}
					}
				}
				// Free resources
				delete[] str_line;
			} // END OF IF

		} // while (std::getline(input, line)) - END

		// Chage V value of each vertex UV coordinates.
		for (size_t i = 0; i < vertices.size(); i++) {
			vertices[i].uvCoord.v = 1.0f - vertices[i].uvCoord.v;
		}

		meshObj.vertexCount = (uint32)vertices.size();
		meshObj.indexCount  = (uint32)_indices.size();

		meshObj.vertices = new Vertex[meshObj.vertexCount];
		meshObj.indices = new uint32[meshObj.indexCount];

		memcpy(meshObj.vertices, vertices.data(), (sizeof(Vertex)* meshObj.vertexCount));
		memcpy(meshObj.indices, _indices.data(), (sizeof(uint32)* meshObj.indexCount));

		input.close();
	} // END OF FUNCTION

	// TESTING FUNCTION - Create a simple cube mesh.
	static inline Mesh createSkinnedCubeTestMeshClass()
	{
		Mesh instance;

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

	/*
	 * @brief
	 * Mesh from world grid 
	*/
	static inline Mesh createWorldGrid()
	{
		Mesh ret_mesh;
		float l_dist = 1.0f;
		float l_span = 100.0f;
		int		l_lineCount = (int)(((l_span * 2) / l_dist) - 1);
		int		l_vertexCount = l_lineCount * 4;

		Vertex* l_pVertices = new Vertex[l_vertexCount];
		uint32* l_pIndices	= new uint32[l_vertexCount];

		float l_pos = -l_span + l_dist;

		for (int32 i = 0; i < l_lineCount; i++) {
			// VERTICAL GRID LINES
			l_pVertices[i * 2].position = {
				-l_span, 0, l_pos
			};
			l_pVertices[i * 2 + 1].position = {
				l_span, 0, l_pos
			};

			// HORIZONTAL GRID LINES
			l_pVertices[(l_lineCount * 2) + (i * 2)].position = {
				l_pos, 0, -l_span
			};
			l_pVertices[(l_lineCount * 2) + (i * 2 + 1)].position = {
				l_pos, 0, l_span
			};

			// Increment distances
			l_pos += l_dist;
		}

		for (int32 i = 0; i < l_vertexCount; i++) {
			l_pIndices[i] = i;
		}

		ret_mesh.vertexCount	= l_vertexCount;
		ret_mesh.indexCount		= l_vertexCount;
		ret_mesh.vertices			= l_pVertices;
		ret_mesh.indices			= l_pIndices;

		return ret_mesh;
	}
};