// ModelLoader.cpp

#define FBXSDK_NAMESPACE_USING 0 // Explicitly use fbxsdk namespace

#include "ThirdParty/fbx/fbxsdk.h"
#include "Core/System/ModelLoader.h"
#include <map>

#pragma warning(disable: 4244);
#pragma warning(disable: 4267);

// Aliases for frequently used types

using fbxV4 = fbxsdk::FbxVector4;
using fbxV2 = fbxsdk::FbxVector2;
using fbxVector4array = fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxVector4>;

void OptimEditor::loadFbxModel(Mesh& param_meshObject, const char* param_cstrFilename) 
{

	printf("FBX SDK Version: %d.%d.%d\n", FBXSDK_VERSION_MAJOR, FBXSDK_VERSION_MINOR, FBXSDK_VERSION_REVISION);
	/*
	 * Create FBX SDK manager and create 
	 * IOS settings object.
	*/
	fbxsdk::FbxManager* pManager = fbxsdk::FbxManager::Create();
	fbxsdk::FbxIOSettings* pIos = fbxsdk::FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(pIos);

	pManager->GetIOSettings()->SetBoolProp(IMP_FBX_MATERIAL, true);
	pManager->GetIOSettings()->SetBoolProp(IMP_FBX_TEXTURE, true);
	pManager->GetIOSettings()->SetBoolProp(IMP_FBX_LINK, false);
	pManager->GetIOSettings()->SetBoolProp(IMP_FBX_SHAPE, false);
	pManager->GetIOSettings()->SetBoolProp(IMP_FBX_GOBO, false);
	pManager->GetIOSettings()->SetBoolProp(IMP_FBX_ANIMATION, true);
	pManager->GetIOSettings()->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);

	/*
	 * Create FBX Importer and declare the filename to import.
	 * File path is relative to executable.
	*/
	fbxsdk::FbxImporter* pImporter = fbxsdk::FbxImporter::Create(pManager, "");
	if (!pImporter->Initialize(param_cstrFilename, -1, pManager->GetIOSettings())) {
		printf("ERROR\nFile| %s\nLine| %d\nFunction| %s\n",__FILE__, __LINE__, __FUNCTION__);
		printf("Call to function: fbxsdk::FbxImporter::Initialize(); for file at %s has failed.\n", param_cstrFilename);
		printf("Error status| %s\n\n", pImporter->GetStatus().GetErrorString());
		pManager->Destroy();
		exit(-1);
	}

	/*
	 * If imported has been initalized correctly, create a scene container and load
	 * scene from file. Once content of file are in the scene the importer can then
	 * be safely destroyed.
	*/
	fbxsdk::FbxScene* pScene = fbxsdk::FbxScene::Create(pManager, "scene");
	pImporter->Import(pScene);
	pImporter->Destroy();

	fbxsdk::FbxGeometryConverter converter(pManager);
	converter.Triangulate(pScene, true);

	/*
	 * Get the root node of the scene
	*/
	fbxsdk::FbxNode* pRoot = pScene->GetRootNode();

	if (pRoot == nullptr) {
		printf("Failed to get fbx root node:\n");
		exit(-1);
	}

	//for (int i = 0; i < pRoot->GetChildCount(); i++) {

	//}

	//printf("child node child count: %d\n", l_pChildNode->GetChildCount());
	//printf("Child node class type: %s\n", l_pChildNode->GetNodeAttribute()->GetClassId().GetName());

	fbxsdk::FbxNode* l_pChildNode = pRoot->GetChild(0);
	fbxsdk::FbxNodeAttribute::EType l_eNodeType;

	if (l_pChildNode->GetNodeAttribute() == nullptr) {
		printf("Error the node has no attribute.\n");
		pManager->Destroy();
		return;
	}

	l_eNodeType = (l_pChildNode->GetNodeAttribute()->GetAttributeType());

	switch (l_eNodeType) {
		case fbxsdk::FbxNodeAttribute::eUnknown:
			break;
		case fbxsdk::FbxNodeAttribute::eNull:
			break;
		case fbxsdk::FbxNodeAttribute::eMarker:
			break;
		case fbxsdk::FbxNodeAttribute::eSkeleton:
			break;
		case fbxsdk::FbxNodeAttribute::eMesh: {
			//auto l_locRot = l_pChildNode->LclRotation.Get();
			//printf("Node rotation (%f, %d, %f)\n", l_locRot.mData[0], l_locRot.mData[1], l_locRot.mData[2]);

			fbxsdk::FbxMesh* _pMesh = l_pChildNode->GetMesh();
			int ctrlPointsCount = _pMesh->GetControlPointsCount();

			fbxsdk::FbxVector4* pControlPoints = _pMesh->GetControlPoints();

			//for (int i = 0; i < ctrlPointsCount; i++) {
			//	printf("Vertex: %f, %f, %f, %f\n", pControlPoints[i].mData[0], pControlPoints[i].mData[1], pControlPoints[i].mData[2], pControlPoints[i].mData[3]);
			//}

			//auto normals = _pMesh->GetElementNormal(0)->GetDirectArray();
			fbxsdk::FbxGeometryElementNormal* pNormalElement = _pMesh->GetElementNormal();
			//printf("Normal Count: %d\n", pNormalElement->GetDirectArray().GetCount());
			//for (int i = 0; i < pNormalElement->GetDirectArray().GetCount(); i++) {
			//	printf("Normal: <%f, %f, %f, %f>\n", 
			//				 pNormalElement->GetDirectArray()[i].mData[0],
			//				 pNormalElement->GetDirectArray()[i].mData[1],
			//				 pNormalElement->GetDirectArray()[i].mData[2],
			//				 pNormalElement->GetDirectArray()[i].mData[3]
			//	);
			//}

			/*
			*/
			if (pNormalElement->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex) {
				printf("FbxGeometryElementNormal mapping mode eByPolygonVertex\n");
			}
			else if (pNormalElement->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint) {
				printf("FbxGeometryElementNormal mapping mode eByControlPoint\n");
			}

			fbxsdk::FbxGeometryElementUV* pUvElement = _pMesh->GetElementUV(0);

			printf("UV count %d\n", pUvElement->GetDirectArray().GetCount());

			printf("UV set name: %s\n", pUvElement->GetName());

			//if (pUvElement->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex) {
			//	printf("FbxGeometryElementUV mapping mode eByPolygonVertex\n");
			//}
			//else if (pUvElement->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint) {
			//	printf("FbxGeometryElementUV mapping mode eByControlPoint\n");
			//}

			int polyCount = _pMesh->GetPolygonCount();
			//printf("Polygon count %d\n", polyCount);
			//printf("Polygon size: %d\n", _pMesh->GetPolygonSize(0));
			//printf("Polygon vertex count %d\n", _pMesh->GetPolygonVertexCount());

			//param_meshObject.vertexCount = _pMesh->GetPolygonVertexCount();
			//param_meshObject.vertices = new Vertex[_pMesh->GetPolygonVertexCount()];

			//int polyVertex = _pMesh->GetPolygonVertex(0, 0);

			std::map<Vertex, uint32> l_mapVertexIndex;
			std::vector<Vertex> l_vertices{};
			std::vector<uint32> l_indices{};
			Vertex vertData{};

			for (int i = 0; i < polyCount; i++) {
				for (int j = 0; j < _pMesh->GetPolygonSize(i); j++) {
					//printf("Polygon size: %d\n", _pMesh->GetPolygonSize(i));
					fbxV4 normal;
					fbxV2 uv;
					bool unmapped;

					int indexControlPoint	= _pMesh->GetPolygonVertex(i, j);
					int vertIndexNormal	= _pMesh->GetPolygonVertexNormal(i, j, normal);
					int indexUV = _pMesh->GetPolygonVertexUV(i, j, pUvElement->GetName(), uv, unmapped);

					vertData.position.x = pControlPoints[indexControlPoint].mData[0];
					vertData.position.y = pControlPoints[indexControlPoint].mData[1];
					vertData.position.z = pControlPoints[indexControlPoint].mData[2];

					vertData.normal.x = normal.mData[0];
					vertData.normal.y = normal.mData[1];
					vertData.normal.z = normal.mData[2];

					vertData.uvCoord.u = uv.mData[0];
					vertData.uvCoord.v = 1.0f - uv.mData[1];

					// Check if unique vertex has already being processed
					// If so only add its index
					auto it = l_mapVertexIndex.find(vertData);
					if (it != l_mapVertexIndex.end()) {
						//printf("Vertex already indexed\n");
						l_indices.push_back(l_mapVertexIndex[vertData]);
					}
					else {
						//printf("Making new instance of vertex\n");
						uint32 l_newIndex = (uint32)l_mapVertexIndex.size();
						l_vertices.push_back(vertData);
						l_mapVertexIndex.emplace(std::make_pair(vertData, l_newIndex));
						l_indices.push_back(l_newIndex);
					}
				} // end of loop iterating through each polygon's vertices
			} // end of for loop iterating through each polygon

			param_meshObject.vertices = new Vertex[l_vertices.size()];
			param_meshObject.vertexCount = l_vertices.size();

			param_meshObject.indexCount = l_indices.size();
			param_meshObject.indices = new uint32[l_indices.size()];

			memcpy(param_meshObject.vertices, l_vertices.data(), sizeof(Vertex) * l_vertices.size());
			memcpy(param_meshObject.indices, l_indices.data(), sizeof(uint32) * l_indices.size());
		} // case scope
			break;
		case fbxsdk::FbxNodeAttribute::eNurbs:
			break;
		case fbxsdk::FbxNodeAttribute::ePatch:
			break;
		case fbxsdk::FbxNodeAttribute::eCamera:
			break;
		case fbxsdk::FbxNodeAttribute::eCameraStereo:
			break;
		case fbxsdk::FbxNodeAttribute::eCameraSwitcher:
			break;
		case fbxsdk::FbxNodeAttribute::eLight:
			break;
		case fbxsdk::FbxNodeAttribute::eOpticalReference:
			break;
		case fbxsdk::FbxNodeAttribute::eOpticalMarker:
			break;
		case fbxsdk::FbxNodeAttribute::eNurbsCurve:
			break;
		case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface:
			break;
		case fbxsdk::FbxNodeAttribute::eBoundary:
			break;
		case fbxsdk::FbxNodeAttribute::eNurbsSurface:
			break;
		case fbxsdk::FbxNodeAttribute::eShape:
			break;
		case fbxsdk::FbxNodeAttribute::eLODGroup:
			break;
		case fbxsdk::FbxNodeAttribute::eSubDiv:
			break;
		case fbxsdk::FbxNodeAttribute::eCachedEffect:
			break;
		case fbxsdk::FbxNodeAttribute::eLine:
			break;
		default:
			break;
	}
	pManager->Destroy();
}
