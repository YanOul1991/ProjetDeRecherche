// ModelLoader.h

#pragma once

#include "Core/Graphics/Mesh.h"
#include "Core/OptimEngine.h"

namespace OptimEditor {

/*
 * @brief
 * Read a .fbx file and get the data from it.
 */
void loadFbxModel(Mesh& meshObject, const char* filename);
} // namespace OptimEditor