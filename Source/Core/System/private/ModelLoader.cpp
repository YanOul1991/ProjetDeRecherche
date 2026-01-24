// ModelLoader.cpp

#define FBXSDK_NAMESPACE_USING 0 // Explicitly use fbxsdk namespace

#include "ThirdParty/fbx/fbxsdk.h"
#include "Core/System/ModelLoader.h"

// Aliases for frequently used types

using fbxV4 = fbxsdk::FbxVector4;
using fbxV2 = fbxsdk::FbxVector2;
using fbxVector4array = fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxVector4>;

void OptimEditor::loadFbxModel(Mesh& param_meshObject, const char* param_cstrFilename) 
{
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
			fbxsdk::FbxMesh* _pMesh = l_pChildNode->GetMesh();
			int ctrlPointsCount = _pMesh->GetControlPointsCount();

			fbxsdk::FbxVector4* pControlPoints = _pMesh->GetControlPoints();

			for (int i = 0; i < ctrlPointsCount; i++) {
				printf("Vertex: %f, %f, %f, %f\n", pControlPoints[i].mData[0], pControlPoints[i].mData[1], pControlPoints[i].mData[2], pControlPoints[i].mData[3]);
			}

			//auto normals = _pMesh->GetElementNormal(0)->GetDirectArray();
			fbxsdk::FbxGeometryElementNormal* pNormalElement = _pMesh->GetElementNormal();
			printf("Normal Count: %d\n", pNormalElement->GetDirectArray().GetCount());
			for (int i = 0; i < pNormalElement->GetDirectArray().GetCount(); i++) {
				printf("Normal: <%f, %f, %f, %f>\n", 
							 pNormalElement->GetDirectArray()[i].mData[0],
							 pNormalElement->GetDirectArray()[i].mData[1],
							 pNormalElement->GetDirectArray()[i].mData[2],
							 pNormalElement->GetDirectArray()[i].mData[3]
				);
			}

			if (pNormalElement->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex) {
				printf("FbxGeometryElementNormal mapping mode eByPolygonVertex\n");
			}
			else if (pNormalElement->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint) {
				printf("FbxGeometryElementNormal mapping mode eByControlPoint\n");
			}

			fbxsdk::FbxGeometryElementUV* pUvElement = _pMesh->GetElementUV(0);
			printf("UV count %d\n", pUvElement->GetDirectArray().GetCount());

			if (pUvElement->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex) {
				printf("FbxGeometryElementUV mapping mode eByPolygonVertex\n");
			}
			else if (pUvElement->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint) {
				printf("FbxGeometryElementUV mapping mode eByControlPoint\n");
			}

			int polyCount = _pMesh->GetPolygonCount();
			printf("Polygon count %d\n", polyCount);
			printf("Polygon size: %d\n", _pMesh->GetPolygonSize(0));
			printf("Polygon vertex count %d\n", _pMesh->GetPolygonVertexCount());

			//param_meshObject.vertexCount = _pMesh->GetPolygonVertexCount();
			//param_meshObject.vertices = new Vertex[_pMesh->GetPolygonVertexCount()];

			//int polyVertex = _pMesh->GetPolygonVertex(0, 0);

			std::vector<Vertex> l_vertices{};
			Vertex vertData{};

			for (int i = 0; i < polyCount; i++) {
				for (int j = 0; j < _pMesh->GetPolygonSize(i); j++) {
					fbxV4 normal;
					fbxV2 uv;

					int indexControlPoint	= _pMesh->GetPolygonVertex(i, j);
					int vertIndexNormal	= _pMesh->GetPolygonVertexNormal(i, j, normal);

					vertData.position.x = pControlPoints[indexControlPoint].mData[0];
					vertData.position.y = pControlPoints[indexControlPoint].mData[1];
					vertData.position.z = pControlPoints[indexControlPoint].mData[2];

					vertData.normal.x = normal.mData[0];
					vertData.normal.y = normal.mData[1];
					vertData.normal.z = normal.mData[2];

					/*
					 * Get UV data
					*/
					if (pUvElement->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex) {
						int indexUV	= _pMesh->GetTextureUVIndex(i, j);

						if (pUvElement->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect) {
							vertData.uvCoord.u = pUvElement->GetDirectArray().GetAt(indexUV).mData[0];
							vertData.uvCoord.v = pUvElement->GetDirectArray().GetAt(indexUV).mData[1];
						}
						else {
							int l_directIndex = pUvElement->GetIndexArray().GetAt(indexUV);
							vertData.uvCoord.u = pUvElement->GetDirectArray().GetAt(l_directIndex).mData[0];
							vertData.uvCoord.v = pUvElement->GetDirectArray().GetAt(l_directIndex).mData[1];
						}
					}
					else if (pUvElement->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint) {
						if (pUvElement->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect) {
							vertData.uvCoord.u = pUvElement->GetDirectArray().GetAt(indexControlPoint).mData[0];
							vertData.uvCoord.v = pUvElement->GetDirectArray().GetAt(indexControlPoint).mData[1];
						}
						else {
							int l_directIndex = pUvElement->GetIndexArray().GetAt(indexControlPoint);
							vertData.uvCoord.u = pUvElement->GetDirectArray().GetAt(l_directIndex).mData[0];
							vertData.uvCoord.v = pUvElement->GetDirectArray().GetAt(l_directIndex).mData[1];
						}
					}

					l_vertices.push_back(vertData);
				} // end of loop iterating through each polygon's vertices
			} // end of for loop iterating through each polygon

			param_meshObject.vertices = new Vertex[l_vertices.size()];
			param_meshObject.vertexCount = l_vertices.size();

			memcpy(param_meshObject.vertices, l_vertices.data(), sizeof(Vertex) * l_vertices.size());
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
