#include "Core/Math/OptimMathematics.h"

TypeInfo* float3::StaticTypeInfo() {
  static TypeInfo info;
  static bool     init = false;

  if (!init) {
    info.name       = "float3";
    info.size       = sizeof(float3);
    info.createFn   = nullptr;
    info.typeData   = TypeData::Structure;
    info.get        = nullptr;
    info.set        = [](void* ptr, void* val) -> void {
      *reinterpret_cast<float3*>(ptr) = *reinterpret_cast<float3*>(val);
    };
    info.toString   = nullptr;
    info.fromString = nullptr;

    GetTypeRegistry()[info.name] = &info;
    init = true;
  }

  return &info;
}

static struct __OPTIM_INTERNAL_STRUCT_float3_DECLTYPE__ {
  __OPTIM_INTERNAL_STRUCT_float3_DECLTYPE__() {
    float3::StaticTypeInfo();
    printf("Struct type declared: %s\n", float3::StaticTypeInfo()->name);
  }
}__OPTIM_INTERNAL_STRUCT_float3__;

OPTIM_DECLARE_PROPERTY(float3, x)
OPTIM_DECLARE_PROPERTY(float3, y)
OPTIM_DECLARE_PROPERTY(float3, z)