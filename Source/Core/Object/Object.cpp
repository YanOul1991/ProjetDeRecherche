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

OPTIM_DECLARE_PROPERTY(Object, objectField)

Object::Object() {
  m_guid = Optim::Random::getGetGuid();
  objectRegistery.emplace(m_guid, this);
}

Object::~Object() {
  auto target = objectRegistery.find(m_guid);

  if (target != objectRegistery.end()) {
    objectRegistery.erase(target);
  }
}

Object* Object::getObject(const SGuid& guid) {
  return nullptr;
}

bool Object::isChildOf(const TypeInfo* baseType) const {
  const TypeInfo* currentType = GetTypeInfo();

  while (currentType != nullptr) {
    if (currentType == baseType) {
      return true;
    }  

    currentType = currentType->baseType;
  }
  return false;
}



void CORE_API printFields(void* object, const TypeInfo* type, int indent) {
  //std::cout << type->name << '\n';

  for (const FieldInfo& fieldInfo : type->fields) {
    void* pField = (uint8*)object + fieldInfo.offset;

    for (size_t i = 0; i < indent; i++) {
      std::cout << " ";
    }
    std::cout << fieldInfo.name << ": ";
    if (fieldInfo.typeInfo->typeData == TypeData::Primitive) {
      std::cout << fieldInfo.typeInfo->toString(pField) << '\n';
    }
    else {
      std::cout << "\n";
      printFields(pField, fieldInfo.typeInfo, indent + indent);
    }
  }
}

// Since Object is the base type for everything else 
// it ise defined manually for specific fields.

TypeInfo* Object::StaticTypeInfo() {
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


__OPTIM_INTERNAL_REGISTER_OBJECT(ChildClass, Object)