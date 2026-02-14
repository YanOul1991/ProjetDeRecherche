#include "Core/Math/OptimMathematics.h"

#include <sstream>

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
    info.toString     = [](void* pFloat3) -> std::string {
      std::stringstream ss{};
      ss << "(";
      ss << "x=" << reinterpret_cast<float3*>(pFloat3)->x;
      ss << ",";
      ss << "y=" << reinterpret_cast<float3*>(pFloat3)->y;
      ss << ",";
      ss << "z=" << reinterpret_cast<float3*>(pFloat3)->z;
      ss << ")";
      return ss.str();
    };

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