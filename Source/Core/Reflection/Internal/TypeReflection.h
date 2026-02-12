#pragma once

#include "Core/OptimEngine.h"

#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>

enum class TypeData {
  Primitive,
  Array,     // Value Types
  Structure, // Value Types
  Object,
};

struct TypeInfo;
struct FieldInfo;

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
  const char* name{};                            // Shared
  size_t      size{};                            // Shared
  void* (*createFn)(){};                         // Object
  TypeData typeData{};                           // Shared

  void (*get)(void*){};                          // Primitive
  void (*set)(void*, void*){};                   // Primitive

  std::string (*toString)(void*);                // Primitive
  void (*fromString)(void*, const std::string&); // Primitive

  const TypeInfo*           baseType{};          // Object
  std::vector<FieldInfo>    fields{};            // Object, Primitives
  std::vector<FunctionInfo> functions{};         // !!! Not yet implemented !!! Object

  // Array
  const TypeInfo* arrayElementTypeInfo{};
  uint64 (*arrayGetSize)(void*){};
  void* (*arrayGetElement)(void*, uint64){};
  void (*arrayResize)(void*, uint64){};
};

template<typename T> struct TypeResolver {
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
        }
      }
    };
    return &info;
  }
};

// TypeResolve for array types

template <typename ListType> struct TypeResolver<std::vector<ListType>>
{
  static TypeInfo* Get() {
    static bool init = false;
    static TypeInfo info;

    if (!init) {
      info.name       = "std::vector",
      info.size       = sizeof(std::vector<ListType>);
      info.createFn   = nullptr;
      info.typeData   = TypeData::Array;
      info.get        = nullptr;
      info.set        = nullptr;
      info.toString   = nullptr;
      info.fromString = nullptr;
      info.baseType   = nullptr;
      info.fields     = {};
      info.functions  = {};

      info.arrayElementTypeInfo = TypeResolver<ListType>::Get();

      info.arrayGetSize = [](void* arr) -> uint64 {
        return reinterpret_cast<std::vector<ListType>*>(arr)->size();
      };

      info.arrayGetElement = [](void* arr, uint64 index) -> void* {
        return &((*reinterpret_cast<std::vector<ListType>*>(arr))[index]);
      };

      info.arrayResize = [](void* arr, uint64 size) -> void {
        reinterpret_cast<std::vector<ListType>*>(arr)->resize(size);
      };

      init = true;
    }
    return &info;
  };
};