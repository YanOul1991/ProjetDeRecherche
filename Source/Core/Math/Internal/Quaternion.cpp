#include "Core/Math/Quaternion.h"

TypeInfo* Quaternion::StaticTypeInfo() {
  static TypeInfo info;
  static bool     init = false;

  if (!init) {
    info.name       = "Quaternion";
    info.size       = sizeof(Quaternion);
    info.createFn   = nullptr;
    info.typeData   = TypeData::Structure;

    info.get        = nullptr;
    info.set        = [](void* ptr, void* val) -> void {
      *reinterpret_cast<Quaternion*>(ptr) = *reinterpret_cast<Quaternion*>(val);
    };

    info.toString = [](void* pType) -> std::string {
      std::stringstream ss{};
      ss << "(";
      ss << "w=" << reinterpret_cast<Quaternion*>(pType)->w;
      ss << ",";
      ss << "x=" << reinterpret_cast<Quaternion*>(pType)->x;
      ss << ",";
      ss << "y=" << reinterpret_cast<Quaternion*>(pType)->y;
      ss << ",";
      ss << "z=" << reinterpret_cast<Quaternion*>(pType)->z;
      ss << ")";
      return ss.str();
    };

    info.fromString = nullptr;

    GetTypeRegistry()[info.name] = &info;
    init = true;
  }

  return &info;
}

static struct __OPTIM_INTERNAL_STRUCT_Quaternion_DECLTYPE__ {
  __OPTIM_INTERNAL_STRUCT_Quaternion_DECLTYPE__() {
    Quaternion::StaticTypeInfo();
    printf("Struct type declared: %s\n", Quaternion::StaticTypeInfo()->name);
  }
}__OPTIM_INTERNAL_STRUCT_Quaternion__;

OPTIM_DECLARE_PROPERTY(Quaternion, w)
OPTIM_DECLARE_PROPERTY(Quaternion, x)
OPTIM_DECLARE_PROPERTY(Quaternion, y)
OPTIM_DECLARE_PROPERTY(Quaternion, z)