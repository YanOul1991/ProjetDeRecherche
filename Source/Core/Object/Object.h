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

struct TypeInfo;
struct FieldInfo;

enum class TypeData {
  Primitive,
  Structure,
  Object,
};

struct FunctionInfo {
  const char* name{};
  void (*call)(void* object, void** args){};
  std::vector<const TypeInfo*> paramTypes{};
  const TypeInfo*              returnType{};
};

struct FieldInfo {
  const char*     name;
  size_t          offset;
  const TypeInfo* typeInfo;
};

struct TypeInfo {
  const char* name{};
  size_t      size{};
  void* (*createFn)(){};
  TypeData typeData{};

  void (*get)(void*){};
  void (*set)(void*, void*){};

  std::string (*toString)(void*);
  void (*fromString)(void*, const std::string&);

  const TypeInfo*           baseType{};
  std::vector<FieldInfo>    fields{};
  std::vector<FunctionInfo> functions{};
};

#define DECLARE_OBJECT(_TYPE_)       \
 public:                             \
  static TypeInfo* StaticTypeInfo(); \
                                     \
 public:                             \
  TypeInfo* GetTypeInfo() const {    \
    return StaticTypeInfo();         \
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

inline std::unordered_map<std::string, TypeInfo*>& GetTypeRegistry() {
  static std::unordered_map<std::string, TypeInfo*> registry;
  return registry;
}

template<typename T> 
struct TypeResolver {
  static TypeInfo* Get() {
    return T::StaticTypeInfo();
  }
};

template<> struct TypeResolver<int> {
  static TypeInfo* Get() {
    static TypeInfo info = {
      .name     = "int",
      .size     = sizeof(int),
      .createFn = nullptr,
      .typeData = TypeData::Primitive,
      .get      = [](void* ptr) -> void {
        printf("Printing field! %d\n", *((int*)ptr));
      },
      .set = [](void* ptr, void* value) -> void {
        *reinterpret_cast<int*>(ptr) = *reinterpret_cast<int*>(value);
      },
      .toString = [](void* ptr) -> std::string {
        return std::to_string(*reinterpret_cast<int*>(ptr));
      },
      .fromString = [](void* ptr, const std::string& str) -> void {
        try {
          *reinterpret_cast<int*>(ptr) = std::stoi(str);
        }
        catch (const std::exception&) {
          printf("Invalid assigned value.\n");
        }
      }
    };
    return &info;
  }
};

template<> struct TypeResolver<float> {
  static TypeInfo* Get() {
    static TypeInfo info = {
      .name     = "float",
      .size     = sizeof(float),
      .createFn = nullptr,
      .typeData = TypeData::Primitive,
      .get      = [](void* ptr) -> void {
        printf("Printing field! %f\n", *((float*)ptr));
      },
      .set = [](void* ptr, void* value) -> void {
        *reinterpret_cast<float*>(ptr) = *reinterpret_cast<float*>(value);
      },
      .toString = [](void* ptr) -> std::string {
        return std::to_string(*reinterpret_cast<float*>(ptr));
      },
      .fromString = [](void* ptr, const std::string& str) -> void {
        try {
          *reinterpret_cast<float*>(ptr) = std::stof(str);
        }
        catch (const std::exception&) {
          printf("Invalid assigned value.\n");
        }
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

  static Object* getObject(const SGuid& guid);

  int objectField = 10;

  bool isChildOf(const TypeInfo* type) const;

 protected:
  SGuid m_guid{};
};

class CORE_API ChildClass : public Object
{
  DECLARE_OBJECT(ChildClass)
};

void CORE_API printFields(void* object, const TypeInfo* type, int indent);