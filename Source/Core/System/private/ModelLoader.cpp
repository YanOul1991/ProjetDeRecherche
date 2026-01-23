// ModelLoader.cpp

#define FBXSDK_NAMESPACE_USING 0 // Explicitly use fbxsdk namespace

#include "ThirdParty/fbx/fbxsdk.h"
#include "Core/System/ModelLoader.h"

void OptimEditor::loadFbxModel(Mesh& meshObject, const char* filename) {
	/*
	 * Load FBX into memory
	*/

	fbxsdk::FbxManager* manager = fbxsdk::FbxManager::Create();
	fbxsdk::FbxIOSettings* ios = fbxsdk::FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ios);

	fbxsdk::FbxImporter* importer = fbxsdk::FbxImporter::Create(manager, "");
	importer->Initialize(filename, -1, manager->GetIOSettings());

	fbxsdk::FbxScene* scene = fbxsdk::FbxScene::Create(manager, "scene");
	importer->Import(scene);
	importer->Destroy();

	fbxsdk::FbxNode* root = scene->GetRootNode();

	if (root) {
		for (int i = 0; i < root->GetChildCount(); i++) {

		}
	}

	manager->Destroy();
}
