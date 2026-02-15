#pragma once

#include "Core/OptimEngine.h"

#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>

struct TypeInfo;
struct FieldInfo;

//struct FunctionInfo {
//  const char* name{};
//  void (*call)(void* object, void** args){};
//  std::vector<const TypeInfo*> paramTypes{};
//  const TypeInfo*              returnType{};
//};

struct SEnum {
  const char* name{};
  uint64       val{};
};

struct FieldInfo {
  const char*     name;
  uint64          offset;
  const TypeInfo* typeInfo;
};

enum class TypeData {
  Primitive,
  Array,
  Structure,
  Enum,
  Object,
};

struct TypeInfo {
  const char* name{};                              // Shared
  size_t      size{};                              // Shared
  void* (*createFn)(){};                           // Object only
  TypeData typeData{};                             // Shared

  void (*get)(void*){};                            // Primitive
  void (*set)(void*, void*){};                     // Primitive

  std::string (*toString)(void*){};                // Primitive only
  void (*fromString)(void*, const std::string&){}; // Primitive only

  const TypeInfo*        baseType{};               // Object only
  std::vector<FieldInfo> fields{};                 // Object, Strcutues


  // Array
  const TypeInfo* arrayElementTypeInfo{};    // For array types
  uint64 (*arrayGetSize)(void*){};           // For array types
  void* (*arrayGetElement)(void*, uint64){}; // For array types
  void (*arrayResize)(void*, uint64){};      // For array types

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
    std::cout << "Get function of type float called for typ resolving.\n";
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
          std::cout << "Setting float from string\n";
          *reinterpret_cast<float*>(ptr) = std::stof(str);
        }
        catch (const std::exception&) {
          std::cout << "Cannot save value as float\n";
        }
      }
    };
    return &info;
  }
};

template <> struct TypeResolver<std::string>
{
  static TypeInfo* Get() 
  {
    static TypeInfo typeInfo;
    static bool init = false;

    if (!init) {
      typeInfo.name     = "string";
      typeInfo.size     = sizeof(std::string);
      typeInfo.typeData = TypeData::Primitive;
      typeInfo.toString = [](void* ptr) -> std::string {
        return *(reinterpret_cast<std::string*>(ptr));
      };
      typeInfo.fromString = [](void* ptr, const std::string& str) -> void {
        *reinterpret_cast<std::string*>(ptr) = str;
      };

      init = true;
    }

    return &typeInfo;
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
      info.typeData   = TypeData::Array;
      info.fields     = {};

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
        return "Enum to string not implemented yet :(\n";
      };

      init = true;
    }
    return &info;
  };
};

namespace Optim::Internal::Reflection 
{

inline std::string indent(int level) {
  return std::string(level * 2, ' ');
}

/**
 * @brief
 * Is the type one that can hold fields,
 * so is it an Object or a Structure.
 */
inline bool TypeHasFields(const TypeInfo* typeInfo) {
  return typeInfo->typeData == TypeData::Object || typeInfo->typeData == TypeData::Structure;
}

inline bool TypeFieldsNested(const TypeInfo* typeInfo) {
  if (TypeHasFields(typeInfo)) {
    for (auto& field : typeInfo->fields) {
      if (TypeHasFields(typeInfo)) {
        return true;
      }
    }
  }
  return false;
}

inline std::string SerializeObject(void* pObject, const TypeInfo* typeInfo, uint32 indentLevel = 0) {
  std::stringstream ss{};

  // If the type of the Object can contain fields,
  // then serialized those fields.
  /*
  if (TypeHasFields(typeInfo)) {
    uint64 fieldIndex = 0;
    uint64 lastFieldIndex  = typeInfo->fields.size() - 1;
    ss << "(";
    for (auto& field : typeInfo->fields) {
      if (fieldIndex == 0) {
        ss << '\n';
      }

      // Get a pointer to the field data using 
      // pointer  arithmeics.
      void* pField = (uint8*)pObject + field.offset;

      ss << field.name << "=";

      ss << SerializeObject(pField, field.typeInfo, indentLevel++);

      if (fieldIndex != lastFieldIndex) {
        ss << ",";
      }
      else {
        ss << ")";
      }
      fieldIndex++;
      ss << '\n';
    }
    ss << '\n';
  }
  */

  // If the object is of type Object or Structure
  if (TypeHasFields(typeInfo)) {
    ss << "(\n";
    bool first = true;
    for (auto& field : typeInfo->fields) {
      if (!first) {
        ss << ",\n";
      }
      first = false;

      void* pField = (uint8*)pObject + field.offset;

      ss << indent(indentLevel + 1);
      ss << field.name;
      ss << "=";
      ss << SerializeObject(pField, field.typeInfo, indentLevel + 1);
    }

    ss << "\n";
    ss << indent(indentLevel);
    ss << ")";
  }

  if (typeInfo->typeData == TypeData::Primitive) {
    ss << typeInfo->toString(pObject);
  }
  return ss.str();
}

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