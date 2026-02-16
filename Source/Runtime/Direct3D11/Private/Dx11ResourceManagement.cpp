// Dx11ResourceManagement.cpp

#include "Private/Dx11ResourceManagement.h"

constexpr uint64 maskIndex      = 0x00000000FFFFFFFF;
constexpr uint64 maskGeneration = 0xFFFFFFFF00000000;

static constexpr uint32 _index(const ResourceHandle& val) {
  return static_cast<uint32>(val.data & maskIndex);
}

static constexpr uint32 _gen(const ResourceHandle& val) {
  return static_cast<uint32>((val.data & maskGeneration) >> 32);
}

ResourceHandle GraphicResourceRegistery::registerResource(EResourceTypes param_resourceType, void* param_pResource) {
  ResourceHandle ret_resourceHandle{};

  if (validIndices.size() == 0) {
    ResourceEntryData l_resEntryData{};

    l_resEntryData.pResource    = param_pResource;
    l_resEntryData.generation   = 1;
    l_resEntryData.resourceType = param_resourceType;
    l_resEntryData.valid        = true;

    ret_resourceHandle.data = entries.size() | ((uint64)l_resEntryData.generation << 32);

    entries.push_back(l_resEntryData);
  }
  else {
    ResourceEntryData& l_entryRealloc = entries[validIndices[0]];
    l_entryRealloc.pResource          = param_pResource;
    l_entryRealloc.generation++;
    l_entryRealloc.resourceType = param_resourceType;
    l_entryRealloc.valid        = true;

    ret_resourceHandle.data = (uint64)validIndices[0] | ((uint64)l_entryRealloc.generation << 32);
    // printf("Reallocared resource to available slot: %d\n", validIndices[0]);
    validIndices.erase(validIndices.begin());
  }

  return ret_resourceHandle;
}

void GraphicResourceRegistery::freeResource(ResourceHandle resourceHandle) {
  uint32             l_generation  = _gen(resourceHandle);
  uint32             l_index       = _index(resourceHandle);
  ResourceEntryData& l_targetEntry = entries[l_index];

  // printf("Trying to free resource at index %d\nGeneration type %d\n", l_index, l_generation);

  switch (l_targetEntry.resourceType) {
  case EResourceTypes::VertexBuffer: {
    // printf("The resource is a vertex buffer.\n");
    delete reinterpret_cast<Dx11VertexBuffer*>(l_targetEntry.pResource);
    break;
  }
  case EResourceTypes::IndexBuffer: {
    // printf("The resource is an indexbuffer.\n");
    delete reinterpret_cast<Dx11IndexBuffer*>(l_targetEntry.pResource);
    break;
  }
  default: {
    printf("Error unkown resource type at :\n--- File | %s\n--- Line | %d\n--- Function | %s\n", __FILE__, __LINE__, __FUNCTION__);
    break;
  }
  } // switch case - END

  l_targetEntry.pResource = nullptr;
  l_targetEntry.valid     = false;
  validIndices.push_back(l_index);

  for (auto& i : validIndices) {
    printf("--- %d\n", i);
  }
}

bool GraphicResourceRegistery::validateHandle(ResourceHandle* param_pHandle, EResourceTypes param_eResourceType) {
  uint32 l_generation = _gen(*param_pHandle);
  uint32 l_index      = _index(*param_pHandle);

  const ResourceEntryData& l_targetResource = entries[l_index];

  if (l_index >= entries.size() || entries.size() == 0) {
    return false;
  }

  return l_targetResource.valid == true &&
         l_targetResource.generation == l_generation &&
         l_targetResource.pResource != nullptr &&
         l_targetResource.resourceType == param_eResourceType;
}

ResourceEntryData* GraphicResourceRegistery::operator[](ResourceHandle& handle) {
  return &entries[_index(handle)];
}

ResourceEntryData* GraphicResourceRegistery::operator[](ResourceHandle* param_pHandle) {
  return &entries[_index(*param_pHandle)];
}