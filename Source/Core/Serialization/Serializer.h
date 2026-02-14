// Serializer.h

#pragma once

#include "Core/Object/Object.h"

#include <vector>

class Serializer
{
 public:
  /**
   * @brief
   * Save all the objects into a scene asset.
   *
   * @param objects
   * The list of all the objects to save.
   *
   * @param sceneName
   * The name of the scene to save without the extension.
   */
  static bool SaveScene(const std::vector<Object*>& objects, const char* sceneName);
};