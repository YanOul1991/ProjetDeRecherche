// Dx11ResourceManagement.h

#pragma once

#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Utilities/Pointer/UniquePtr.h"
#include "Private/Resources/Dx11IndexBuffer.h"
#include "Private/Resources/Dx11PixelShader.h"
#include "Private/Resources/Dx11Sampler.h"
#include "Private/Resources/Dx11Texture.h"
#include "Private/Resources/Dx11VertexBuffer.h"
#include "Private/Resources/Dx11VertexShader.h"

/* =================================================================
  REMAKING OF RESOURCE REGISTERY
================================================================= */

struct ResourceEntryData {
  void*          pResource;
  uint32         generation;
  EResourceTypes resourceType;
  bool           valid;
};

class GraphicResourceRegistery
{
 public:
  /**
   * @brief
   * Adds a resource to the regsitery.
   *
   * @param resourceType
   * The type of the resource given by the EResourceTypes enum.
   *
   * @param pResource
   * A raw pointer to the graphics resource related to its type.
   */
  ResourceHandle registerResource(EResourceTypes resourceType, void* pResource);

  /**
   * @brief
   * Free a graphic resource.
   *
   * @param resourceHandle
   * A handle to the resource to free.
   */
  void freeResource(ResourceHandle resourceHandle);

  /**
   * @
   * brief
   * Validates a resource handle. If the handle still points to a valid
   * resource and check the resource type.
   */
  bool validateHandle(ResourceHandle* pHandle, EResourceTypes resourceType);

  ResourceEntryData* operator[](ResourceHandle& handle);
  ResourceEntryData* operator[](ResourceHandle* param_pHandle);

  std::vector<ResourceEntryData> entries{};
  std::vector<uint32>            validIndices{};
};