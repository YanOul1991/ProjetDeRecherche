/* ======================================================================================
 *  Object.cpp:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#include "Core/Object/Object.h"
#include "Core/OptimEngine.h"

#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>

static std::unordered_map<SGuid, Object*> objectRegistery{};

Object::Object() 
{
  m_guid = Optim::Random::getGetGuid();
  objectRegistery.emplace(m_guid, this);
  printf("[Object] Object class object instanciated\n");
}

Object::~Object() 
{
  auto target = objectRegistery.find(m_guid);

  if (target != objectRegistery.end()) {
    objectRegistery.erase(target);
  }
}

Object* Object::getObject(const SGuid& guid) {
  return nullptr;
}

bool Object::isChildOf(const TypeInfo* baseType) const 
{
  const TypeInfo* currentType = GetTypeInfo();

  while (currentType != nullptr) {
    if (currentType == baseType) {
      return true;
    }  

    currentType = currentType->baseType;
  }
  return false;
}

/**
 * @brief 
 * Print all the fields of and instance of a reflected object
 * and its reflected fields.
 */
void CORE_API printFields(void* object, const TypeInfo* type, int indent) 
{
  for (const FieldInfo& fieldInfo : type->fields) {
    void* pField = (uint8*)object + fieldInfo.offset;

    for (size_t i = 0; i < indent; i++) {
      std::cout << " ";
    }
    std::cout << fieldInfo.name << ": ";
    if (fieldInfo.typeInfo->typeData == TypeData::Primitive) {
      /**
       * If the the data of the type is a primitive,
       * then simply then call its toString method to 
       * print it.
       */
      std::cout << fieldInfo.typeInfo->toString(pField) << '\n';
    }
    else if (fieldInfo.typeInfo->typeData == TypeData::Array) {
      /**
       * If the field is of an array type, then use the arrayRelatedFields,
       * to get the size of the array type object.
       * 
       * When iterating must check if the TypeData of the elements,
       * if they are of type primitive, then their values can simply 
       * be printed, else recall the printField function to get
       * the field of the data.
       */
      uint64 l_arrSize = fieldInfo.typeInfo->arrayGetSize(pField);
      std::cout << "Array [" << l_arrSize << "] " << fieldInfo.typeInfo->arrayElementTypeInfo->name << '\n';

      for (uint64 i = 0; i < l_arrSize; i++) {
        void* pElement = fieldInfo.typeInfo->arrayGetElement(pField, i);

        if (fieldInfo.typeInfo->arrayElementTypeInfo->typeData == TypeData::Primitive) {
          std::cout << fieldInfo.typeInfo->arrayElementTypeInfo->toString(pElement) << "\n";
        }
        else {
          std::cout << "[" << i << "]:\n";
          printFields(pElement, fieldInfo.typeInfo->arrayElementTypeInfo, indent + 2);
        }
      }
    }
    else if (fieldInfo.typeInfo->typeData == TypeData::Enum) {
      std::cout << fieldInfo.typeInfo->name << '\n';
    }
    else {
      /**
       * If the field is either a Structure or an Object
       * then call the 
       */
      std::cout << "\n";
      printFields(pField, fieldInfo.typeInfo, indent + 2);
    }
  }
}

void CORE_API printTypeFields(const TypeInfo* type, int indent) 
{
  for (size_t i = 0; i < indent; i++) {
    std::cout << " ";
  }

  std::cout << type->name << ":\n";

  for (const FieldInfo& fieldInfo : type->fields) {
    // Print indentation
    for (size_t i = 0; i < (indent + indent); i++) {
      std::cout << " ";
    }

    std::cout << fieldInfo.name << ": " << fieldInfo.typeInfo->name << '\n';
    if (fieldInfo.typeInfo->typeData == TypeData::Array) {
      std::cout << "Array type : Element type: " << fieldInfo.typeInfo->arrayElementTypeInfo->name << '\n';
    }
  }

  //if (type->baseType) {
  //  for (size_t i = 0; i < (indent + indent); i++) {
  //    std::cout << " ";
  //  }
  //  std::cout << "Inherited fields from:\n";
  //  printTypeFields(type->baseType, indent + indent);
  //}

  std::cout << '\n';
}

// Since Object is the base type for everything else 
// it ise defined manually for specific fields.

TypeInfo* Object::StaticTypeInfo() 
{
  static TypeInfo info;
  static bool     initialized = false;
  if (!initialized) {
    info.name     = "Object";
    info.size     = sizeof(Object);
    info.createFn = []() -> void* {
      return new Object();
    };
    info.baseType                = nullptr;
    GetTypeRegistry()[info.name] = &info;
    initialized                  = true;
  }
  return &info;
}

static struct _OPTIM_SYSTEM_REGISTRATION_Object {
  _OPTIM_SYSTEM_REGISTRATION_Object() {
    Object::StaticTypeInfo();
  }
} _OPTIM_SYSTEM_REGISTERED_Object;

OPTIM_DECLARE_PROPERTY(Object, objectField)