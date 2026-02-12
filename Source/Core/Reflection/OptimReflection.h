
#pragma once

#include "Core/OptimEngine.h"
#include "Core/Reflection/Internal/TypeReflection.h"

#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>

#define DECLARE_OBJECT()                 \
 public:                                 \
  static TypeInfo* StaticTypeInfo();     \
  TypeInfo*        GetTypeInfo() const { \
    return StaticTypeInfo();             \
  }

#define DECLARE_STRUCT()                  \
  static TypeInfo* StaticTypeInfo();      \
  TypeInfo*        GetTypeInfo() const {  \
    return StaticTypeInfo();              \
  }

#define __OPTIM_INTERNAL_REGISTER_OBJECT(_OBJECT_, Base)     \
  TypeInfo* _OBJECT_::StaticTypeInfo() {                     \
    static TypeInfo info;                                    \
    static bool     initialized = false;                     \
                                                             \
    if (!initialized) {                                      \
      info.name     = #_OBJECT_;                             \
      info.size     = sizeof(_OBJECT_);                      \
      info.createFn = []() -> void* {                        \
        return new _OBJECT_();                               \
      };                                                     \
      info.baseType                = Base::StaticTypeInfo(); \
      GetTypeRegistry()[info.name] = &info;                  \
      initialized                  = true;                   \
    }                                                        \
                                                             \
    return &info;                                            \
  }                                                          \
                                                             \
  static struct _OPTIM_SYSTEM_REGISTRATION_##_OBJECT_ {      \
    _OPTIM_SYSTEM_REGISTRATION_##_OBJECT_() {                \
      _OBJECT_::StaticTypeInfo();                            \
    }                                                        \
  } _OPTIM_SYSTEM_REGISTERED_##_OBJECT_;

#define OPTIM_DECLARE_PROPERTY(CLASS, FIELD)                      \
  static struct __OPTIM_INTERNAL_DECLFIELD_##CLASS##_##FIELD {    \
    __OPTIM_INTERNAL_DECLFIELD_##CLASS##_##FIELD() {              \
                                                                  \
      static FieldInfo fieldInfo = {                              \
        .name     = #FIELD,                                       \
        .offset   = offsetof(CLASS, CLASS::FIELD),                \
        .typeInfo = TypeResolver<decltype(CLASS::FIELD)>::Get()}; \
                                                                  \
      CLASS::StaticTypeInfo()->fields.push_back(fieldInfo);       \
    };                                                            \
  } __OPTIM_INTERNAL_FIELD_##CLASS##_##FIELD;

namespace Optim::Internal::Type {

}

inline std::unordered_map<std::string, TypeInfo*>& GetTypeRegistry() {
  static std::unordered_map<std::string, TypeInfo*> registry;
  return registry;
}