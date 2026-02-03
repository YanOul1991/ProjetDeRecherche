// Dx11ResourceManagement.h

#pragma once

#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Private/Resources/Dx11VertexBuffer.h"
#include "Private/Resources/Dx11IndexBuffer.h"
#include "Private/Resources/Dx11VertexShader.h"
#include "Private/Resources/Dx11PixelShader.h"
#include "Private/Resources/Dx11Texture.h"
#include "Private/Resources/Dx11Sampler.h"
#include "Core/Utilities/Pointer/UniquePtr.h"

namespace Optim::RHI::Directx11
{

struct ResourceEntryData
{
  void* pResource;
  uint32 generation;
  EResourceTypes resourceType;
  bool valid;
};

class GraphicResourceRegistery
{
public:
  inline ResourceHandle registerResource(EResourceTypes param_resourceType, void* param_pResource)
  {
    ResourceHandle ret_resourceHandle{};

    if (validIndices.size() == 0) {
      ResourceEntryData l_resEntryData{};

      l_resEntryData.pResource = param_pResource;
      l_resEntryData.generation = 1;
      l_resEntryData.resourceType = param_resourceType;
      l_resEntryData.valid = true;

      ret_resourceHandle.data = entries.size() | ((uint64)l_resEntryData.generation<< 32);
      // printf("Created a new entry at index: %llu \n", entries.size());
      entries.push_back(l_resEntryData);
    }
    else {
      ResourceEntryData& l_entryRealloc = entries[validIndices[0]];
      l_entryRealloc.pResource = param_pResource;
      l_entryRealloc.generation++;
      l_entryRealloc.resourceType = param_resourceType;
      l_entryRealloc.valid = true;

      ret_resourceHandle.data = (uint64)validIndices[0] | ((uint64)l_entryRealloc.generation << 32);
      // printf("Reallocared resource to available slot: %d\n", validIndices[0]);
      validIndices.erase(validIndices.begin());
    }

    return ret_resourceHandle;
  }

  /*
   * Frees resources on a handle
  */
  inline void freeResource(ResourceHandle resourceHandle) 
  {
    if (!validate(&resourceHandle)) {
      //printf("Cannot free the handle, the resource is invalid.\n");
      return;
    }

    uint32 l_generation = getHandleGen(&resourceHandle);
    uint32 l_index = getHandleIndex(&resourceHandle);
    ResourceEntryData& l_targetEntry = entries[l_index];

    //printf("Trying to free resource at index %d\nGeneration type %d\n", l_index, l_generation);

    switch (l_targetEntry.resourceType) {
      case EResourceTypes::VertexBuffer : {
        //printf("The resource is a vertex buffer.\n");
        delete reinterpret_cast<Dx11VertexBuffer*>(l_targetEntry.pResource);
        break;
      }
      case EResourceTypes::IndexBuffer: {
        //printf("The resource is an indexbuffer.\n");
        delete reinterpret_cast<Dx11IndexBuffer*>(l_targetEntry.pResource);
        break;
      }
      default: {
        printf("Error unkown resource type at :\n--- File | %s\n--- Line | %d\n--- Function | %s\n",__FILE__, __LINE__, __FUNCTION__);
        break;
      }
    } // switch case - END

    l_targetEntry.pResource = nullptr;
    l_targetEntry.valid = false;
    validIndices.push_back(l_index);

    // printf("Resource has been freed. New free indices list\n");

    for (auto& i : validIndices) {
      printf("--- %d\n", i);
    }
  }

  /*
   * @brief
   * DEPRECATED - Use validateHandle() instead.
   * This function does not resource type
  */
  inline bool validate(ResourceHandle* pHandle) 
  {
    uint32 l_generation = getHandleGen(pHandle);
    uint32 l_index = getHandleIndex(pHandle);

    if (l_index >= entries.size() || entries.size() == 0) {
      return false;
    }

    return entries[l_index].valid == true && l_generation == entries[l_index].generation;
  }

  inline bool validateHandle(ResourceHandle* param_pHandle, EResourceTypes param_eResourceType)
  {
    uint32 l_generation = getHandleGen(param_pHandle);
    uint32 l_index = getHandleIndex(param_pHandle);
    const ResourceEntryData& l_targetResource = entries[l_index];

    if (l_index >= entries.size() || entries.size() == 0) {
      return false;
    }

    return 
      l_targetResource.valid == true &&
      l_targetResource.generation == l_generation &&
      l_targetResource.pResource != nullptr &&
      l_targetResource.resourceType == param_eResourceType;
  }

  inline uint32 getHandleIndex(ResourceHandle* pHandle) const 
  {
    return (uint32)(pHandle->data & 0x00000000FFFFFFFF);
  }

  inline uint32 getHandleGen(ResourceHandle* pHandle) const 
  {
    return (uint32)((pHandle->data & 0xFFFFFFFF00000000) >> 32);
  }

  ResourceEntryData* operator[](ResourceHandle& handle) 
  {
    return &entries[getHandleIndex(&handle)];
  }

  ResourceEntryData* operator[](ResourceHandle* param_pHandle)
  {
    return &entries[getHandleIndex(param_pHandle)];
  }

  std::vector<ResourceEntryData> entries{};
  std::vector<uint32> validIndices{};
};

}