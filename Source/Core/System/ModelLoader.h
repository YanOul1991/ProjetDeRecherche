// ModelLoader.h

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Graphics/Mesh.h"

namespace OptimEditor {

/*
 * @brief
 * Read a .obj file and get the data from it.
*/
//void loadObjModel(Mesh& meshObject, const char* filename);

/*
 * @brief
 * Read a .fbx file and get the data from it.
*/
void loadFbxModel(Mesh& meshObject, const char* filename);
} // namespace OptimEditor - END