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

/**
 * Class member fields registration.
 */

__OPTIM_INTERNAL_REGISTER_OBJECT(Object);
__OPTIM_INTERNAL_REGISTER_OBJECT(ChildClass);

OPTIM_DECLARE_PROPERTY(Object, objectField)

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
