/**
 * Physics.h
 */

#pragma once

#include "./Raycast.h"
#include "Core/Graphics/Mesh.h"
#include "Core/OptimEngine.h"
#include "Core/Utilities/Pointer/UniquePtr.h"

#include <vector>

namespace Optim::Physics {

/**
 * \brief
 * Converts a position in screen space to a world space Raycast struct object.
 *
 * \param posX
 * The x position on the screen space.
 *
 * \param posY
 * The y position on the screen space.
 *
 * \param width
 * The width of the screen.
 *
 * \param height
 * The height of the screen.
 *
 * \return
 * A Raycast struct object.
 */
Raycast ScreenToRaycast(float posX, float posY, float width, float height);

UniquePtr<Mesh>* GetCollision(const Raycast& raycast, std::vector<UniquePtr<Mesh>>& meshList);

} // namespace Optim::Physics