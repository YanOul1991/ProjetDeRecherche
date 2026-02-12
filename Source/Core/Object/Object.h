/* ======================================================================================
 *  Object.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Utilities/Random/Random.h"

#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>

#include "Core/Math/OptimMathematics.h"
#include "Core/Math/Quaternion.h"

struct TypeInfo;
struct FieldInfo;

// Description of a field of a class type object
struct FieldInfo {
  const char*     name;
  size_t          offset;
  const TypeInfo* typeInfo;
};

// Description of a type same for primitives and
// class object types.

struct TypeInfo {
  const char* name{};
  size_t      size{};
  void* (*createFn)(){};
  void (*get)(void*){};
  void (*set)(void*, void*){};
  std::vector<FieldInfo> fields{};
};

#define DECLARE_OBJECT(_TYPE_)            \
 public:                                  \
  static TypeInfo* StaticTypeInfo();      \
                                          \
 public:                                  \
  virtual TypeInfo* GetTypeInfo() const { \
    return StaticTypeInfo();              \
  }

#define __OPTIM_INTERNAL_REGISTER_OBJECT(_OBJECT_)      \
  TypeInfo* _OBJECT_::StaticTypeInfo() {                \
    static TypeInfo info;                               \
    static bool     initialized = false;                \
                                                        \
    if (!initialized) {                                 \
      info.name     = #_OBJECT_;                        \
      info.size     = sizeof(_OBJECT_);                 \
      info.createFn = []() -> void* {                   \
        return new _OBJECT_();                          \
      };                                                \
                                                        \
      GetTypeRegistry()[info.name] = &info;             \
      initialized                  = true;              \
    }                                                   \
                                                        \
    return &info;                                       \
  }                                                     \
                                                        \
  static struct _OPTIM_SYSTEM_REGISTRATION_##_OBJECT_ { \
    _OPTIM_SYSTEM_REGISTRATION_##_OBJECT_() {           \
      _OBJECT_::StaticTypeInfo();                       \
    }                                                   \
  } _OPTIM_SYSTEM_REGISTERED_##_OBJECT_;

#define OPTIM_DECLARE_PROPERTY(CLASS, FIELD)                        \
  static inline struct __OPTIM_INTERNAL_DECLFIELD_##CLASS_##FIELD { \
    __OPTIM_INTERNAL_DECLFIELD_##CLASS_##FIELD() {                  \
                                                                    \
      static FieldInfo fieldInfo = {                                \
        .name     = #FIELD,                                         \
        .offset   = offsetof(CLASS, CLASS::FIELD),                  \
        .typeInfo = TypeResolver<decltype(CLASS::FIELD)>::Get()};   \
                                                                    \
      CLASS::StaticTypeInfo()->fields.push_back(fieldInfo);         \
                                                                    \
      printf("Field member for Object class initalized!\n");        \
    };                                                              \
  } __OPTIM_INTERNAL_FIELD_##CLASS_##FIELD;                         \


inline std::unordered_map<std::string, TypeInfo*>& GetTypeRegistry() {
  static std::unordered_map<std::string, TypeInfo*> registry;
  return registry;
}

template <typename T> struct TypeResolver {
  static TypeInfo* Get() {
    return T::StaticTypeInfo();
  }
};

template<> struct TypeResolver<int> {
  static TypeInfo* Get() {
    static TypeInfo info = { 
      .name = "int", 
      .size = sizeof(int), 
      .createFn = nullptr,
      .get = [](void* ptr) -> void {
        printf("Printing field! %d\n", *((int*)ptr));
      },
      .set = [](void* ptr, void* value) -> void {
        *reinterpret_cast<int*>(ptr) = *reinterpret_cast<int*>(value);
      }
    };
    return &info;
  }
};

template<> struct TypeResolver<float> {
  static TypeInfo* Get() {
    static TypeInfo info = { 
      "float", 
      sizeof(float), 
      nullptr,
      [](void* ptr) -> void {
        printf("Printing field! %f\n", *((float*)ptr));
      },
      [](void* ptr, void* value) -> void {
        *reinterpret_cast<float*>(ptr) = *reinterpret_cast<float*>(value);
      }
    };
    return &info;
  }
};

template<> struct TypeResolver<float3> {
  static TypeInfo* Get() {
    static TypeInfo info = { 
      "float3", 
      sizeof(float), 
      nullptr,
      [](void* ptr) -> void {
        printf("X: %f, Y: %f, Z: %f\n", (*(float3*)ptr).x, (*(float3*)ptr).y, (*(float3*)ptr).z);
      },
      [](void* ptr, void* value) -> void {
        *reinterpret_cast<float3*>(ptr) = *reinterpret_cast<float3*>(value);
      }
    };
    return &info;
  }
};

template<> struct TypeResolver<Quaternion> {
  static TypeInfo* Get() {
    static TypeInfo info = { 
      "Quaternion", 
      sizeof(float), 
      nullptr,
      [](void* ptr) -> void {
        printf("W: %f, X: %f, Y: %f, Z: %f\n", (*(Quaternion*)ptr).w, (*(Quaternion*)ptr).x, (*(Quaternion*)ptr).y, (*(Quaternion*)ptr).z);
      },
      [](void* ptr, void* value) -> void {
        *reinterpret_cast<Quaternion*>(ptr) = *reinterpret_cast<Quaternion*>(value);
      }
    };
    return &info;
  }
};

class CORE_API Object
{
  DECLARE_OBJECT(Object)

 public:
  Object();
  virtual ~Object();

  /*
  template <typename T>
  static T* cast(Object* obj) {
    if (obj == nullptr) {
      return nullptr;
    }

    const Type* ti     = obj->getTypeInfo();
    const Type* target = &T::typeInfo;

    while (ti) {
      std::cout << "Type comparaison:\n";
      std::cout << "--- Object type : " << ti << "\n";
      std::cout << "--- Target type : " << target << "\n";
      if (ti == target) {
        return static_cast<T*>(obj);
      }
      ti = ti->parent;
    }
    return nullptr;
  }
   */

  static Object* getObject(const SGuid& guid);

  int   objectField = 10;
  float3 secondField = { 378.999f, -278.38f , 43.3f};

 protected:
  SGuid m_guid{};
};

class CORE_API ChildClass : public Object
{
  DECLARE_OBJECT(ChildClass)
};