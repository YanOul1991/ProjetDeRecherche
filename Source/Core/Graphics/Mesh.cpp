#include "Core/Graphics/Mesh.h"

Mesh::Mesh() {
}

Mesh::~Mesh() noexcept {
}

float4x4 Mesh::getWorldMatrix() const {
  float4x4 l_translation = Optim::Mathematics::getMatrixTranslation(position);
  float4x4 l_rotation    = Optim::Mathematics::getMatrixFromQuaternion(rotation);
  return l_rotation * l_translation;
}

__OPTIM_INTERNAL_REGISTER_OBJECT(Mesh, Object)

OPTIM_DECLARE_PROPERTY(Mesh, position)
OPTIM_DECLARE_PROPERTY(Mesh, rotation)
OPTIM_DECLARE_PROPERTY(Mesh, sourcePath)
// OPTIM_DECLARE_PROPERTY(Mesh, scale)

/*
#define OPTIM_DECLARE_ENUM_FIELD(ENUM, FIELD)                                    \
  static struct __OPTIM_INTERNAL_DECL_ENUM_VAL_##ENUM##_##FIELD {                \
    __OPTIM_INTERNAL_DECL_ENUM_VAL_##ENUM##_##FIELD() {                          \
      static SEnum enumVal = {                                                   \
        .name = #FIELD,                                                          \
        .val  = (uint64)ENUM::FIELD};                                             \
      TypeResolver<decltype(ENUM::FIELD)>::Get()->enumFields.push_back(enumVal); \
      printf("Enum value serialized %s::%s\n", #ENUM, #FIELD);                   \
    }                                                                            \
  } __OPTIM_INTERNAL_ENUM_VAL_##ENUM##_##FIELD;

OPTIM_DECLARE_ENUM_FIELD(MyCustomEnum, value1)
OPTIM_DECLARE_ENUM_FIELD(MyCustomEnum, value2)
OPTIM_DECLARE_ENUM_FIELD(MyCustomEnum, value3)
OPTIM_DECLARE_ENUM_FIELD(MyCustomEnum, value4)
OPTIM_DECLARE_ENUM_FIELD(MyCustomEnum, value5)


static struct __OPTIM_INTERNAL_DECL_ENUM_VAL_MyCustomEnum_value1 {
  __OPTIM_INTERNAL_DECL_ENUM_VAL_MyCustomEnum_value1() {
    static SEnum enumVal = {
      .name = "value1",
      .val  = (uint64)MyCustomEnum::value1
    };
    TypeResolver<decltype(MyCustomEnum::value1)>::Get()->enumFields.push_back(enumVal);
    printf("Enum value serialized MyCustomEnum::value1.\n");
  }
} __OPTIM_INTERNAL_ENUM_EVAL_MyCustomEnum_value1;
*/