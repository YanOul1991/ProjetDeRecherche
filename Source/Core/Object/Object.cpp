/* ======================================================================================
 *  Object.cpp:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#include "Core/Object/Object.h"

#include "Core/Defines/Windows/windowsAPI.h"
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

  // std::cout << std::dec
  //	<< "[Object] A new Object was initialized:\n"
  //	<< "----- Registery count : " << objectRegistery.size() << '\n'
  //	<< "----- Object type     : " << objectRegistery[m_guid]->getTypeInfo()->name << '\n'
  //	<< "----- Object address  : " << std::hex << "0x" << this << '\n' << std::dec;
}

Object::~Object() {
  auto target = objectRegistery.find(m_guid);

  if (target != objectRegistery.end()) {
    objectRegistery.erase(target);
  }
}

CORE_API Object* Object::getObject(const SGuid& guid) {
  return nullptr;
  /*
  Object* pObj = nullptr;

  if (objectRegistery.contains(guid)) {
          pObj = objectRegistery[guid];
          printf("Found an Object with the follwing ID: ");
  }
  else {
          printf("Could not find an Object with the follwing ID: ");
  }

  for (int i = 0; i < 16; i++) {
          if (i == 4 || i == 6 || i == 8 || i == 10) {
                  printf("-");
          }
          printf("%02X", guid.data[i]);
  }
  printf("\n");

  return pObj;
  */
}



/*
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
*/

/**
 * Class member fields registration.
 */

__OPTIM_INTERNAL_REGISTER_OBJECT(Object);
__OPTIM_INTERNAL_REGISTER_OBJECT(ChildClass);

OPTIM_DECLARE_PROPERTY(Object, objectField)
OPTIM_DECLARE_PROPERTY(Object, secondField)

/*
static inline struct __OPTIM_INTERNAL_FIELD_object_objectField {
  __OPTIM_INTERNAL_FIELD_object_objectField() {

    static FieldInfo fieldInfo = {
      .name     = "objectField",
      .offset   = offsetof(Object, Object::objectField),
      .typeInfo = TypeResolver<decltype(Object::objectField)>::Get()
    };

    Object::StaticTypeInfo()->fields.push_back(fieldInfo);

    printf("Field member for Object class initalized!\n");
  };
} __OPTIM_INTERNAL_FIELD_object_objectField_REGISTERED;

static inline struct __OPTIM_INTERNAL_FIELD_Object_secondField{
  __OPTIM_INTERNAL_FIELD_Object_secondField() {

    static FieldInfo fieldInfo = {
      .name     = "secondField",
      .offset   = offsetof(Object, Object::secondField),
      .typeInfo = TypeResolver<decltype(Object::secondField)>::Get()
    };

    Object::StaticTypeInfo()->fields.push_back(fieldInfo);

    printf("Field member for Object class initalized!\n");
  };
} __OPTIM_INTERNAL_FIELD_Object_secondField_REGISTERED;
*/

