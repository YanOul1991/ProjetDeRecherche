#pragma once

#include "Core/OptimEngine.h"

#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>

//template<typename T> class TList final
//{
// public:
//  TList() = default;
//
//  inline void initalAlloc(uint32 param_allocCount) {
//    pData     = new T[param_allocCount];
//    capacity  = param_allocCount;
//    count     = 0;
//  }
//
//  inline void Add(const T& element) {
//    if (count < capacity) {
//      pData[count + 1] = element;
//      count++;
//    }
//  }
//
//  inline uint32 Count() const {
//    return count;
//  }
//
// private:
//  T* pData{};
//  uint32 count{0};
//  uint32 capacity{0};
//};
//
//TList<int> myList;

enum class TypeData {
  Primitive,
  Array,
  Structure,
  Enum,
  Object,
};

struct TypeInfo;
struct FieldInfo;

//struct FunctionInfo {
//  const char* name{};
//  void (*call)(void* object, void** args){};
//  std::vector<const TypeInfo*> paramTypes{};
//  const TypeInfo*              returnType{};
//};

struct FieldInfo {
  const char*     name;
  uint64          offset;
  const TypeInfo* typeInfo;
};

struct SEnum {
  const char* name{};
  uint64       val{};
};

struct TypeInfo {
  const char* name{};                              // Shared
  size_t      size{};                              // Shared
  void* (*createFn)(){};                           // Object
  TypeData typeData{};                             // Shared

  void (*get)(void*){};                            // Primitive
  void (*set)(void*, void*){};                     // Primitive

  std::string (*toString)(void*){};                // Primitive
  void (*fromString)(void*, const std::string&){}; // Primitive

  const TypeInfo*        baseType{};               // Object
  std::vector<FieldInfo> fields{};                 // Object, Primitives

  //std::vector<FunctionInfo> functions{};           // !!! Not yet implemented !!! Object

  // Array
  const TypeInfo* arrayElementTypeInfo{};
  uint64 (*arrayGetSize)(void*){};
  void* (*arrayGetElement)(void*, uint64){};
  void (*arrayResize)(void*, uint64){};

  // Enum Types data
  std::vector<SEnum> enumFields{};
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
      //info.functions  = {};

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

enum class MyCustomEnum {
  value1,
  value2,
  value3,
  value4,
  value5
};

template<> struct TypeResolver<MyCustomEnum>
{
  static TypeInfo* Get() {
    static bool init = false;
    static TypeInfo info;

    if (!init) {
      info.name       = "MyCustomEnum",
      info.size       = sizeof(MyCustomEnum);
      info.typeData   = TypeData::Enum;

      info.toString = [](void* pEnum) -> std::string {
        return "hello!";
      };

      init = true;
    }
    return &info;
  };
};


namespace Optim::Internal::Reflection 
{

template<typename T> TypeInfo* getTypeInfo() {
  return TypeResolver<T>::Get();
}

inline void printTypeInfo(const TypeInfo* pTypeInfo) 
{
  switch (pTypeInfo->typeData) {
  case TypeData::Enum: {
    std::cout << "Type of " << pTypeInfo->name << " : Enum " << '\n';
    std::cout << "  Enumerator count: " << pTypeInfo->enumFields.size() << "\n";

    for (const SEnum& eField : pTypeInfo->enumFields) {
      std::cout << "     Enumerator : " << eField.name << " -> " << eField.val << '\n';
    }

    break;
  };
  default:
    printf("Unkown Type\n");
    break;
  }
}

} // namespace Optim::Internal::Reflection

//static inline struct __OPTIM_INTERNAL_DECL_ENUM_VAL_MyCustomEnum_value1 {
//  __OPTIM_INTERNAL_DECL_ENUM_VAL_MyCustomEnum_value1() {
//    static SEnum enumVal = {
//      .name = "value1",
//      .val  = (uint64)MyCustomEnum::value1
//    };
//    TypeResolver<decltype(MyCustomEnum::value1)>::Get()->enumFields.push_back(enumVal);
//    printf("Enum value serialized MyCustomEnum::value1.\n");
//  }
//} __OPTIM_INTERNAL_ENUM_EVAL_MyCustomEnum_value1;