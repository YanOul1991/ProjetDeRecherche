/* ======================================================================================
 *  Object.cpp:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#include "Core/Defines/Windows/windowsAPI.h"

#include "Core/OptimEngine.h"
#include "Core/Object/Object.h"

#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <iomanip>
#include <unordered_map>

/*
#define __OPTIM_INTERNAL_REGISTER_OBJECT(_OBJECT_)															\
TypeInfo* _OBJECT_::StaticTypeInfo() {																					\
	static TypeInfo info;																													\
	static bool initialized = false;																							\
																																								\
	if (!initialized) {																														\
		info.name = #_OBJECT_;																											\
		info.size = sizeof(_OBJECT_);																								\
		info.createFn = []() -> void* { return new _OBJECT_(); };										\
																																								\
		GetTypeRegistry()[info.name] = &info;																				\
		initialized = true;																													\
	}																																							\
																																								\
	return &info;																																	\
}																																								\
																																								\
static struct _OPTIM_SYSTEM_REGISTRATION_##_OBJECT_ {														\
	_OPTIM_SYSTEM_REGISTRATION_##_OBJECT_() {																			\
		_OBJECT_::StaticTypeInfo();																									\
		printf("Class registered : %s\n", #_OBJECT_);																\
	}																																							\
} _OPTIM_SYSTEM_REGISTERED_##_OBJECT_;																					\
*/


static std::unordered_map<SGuid, Object*> objectRegistery{};

__OPTIM_INTERNAL_REGISTER_OBJECT(Object);

/*
TypeInfo* Object::StaticTypeInfo()
{
	static TypeInfo info;
	static bool initalized = false;

	if (!initalized) {
		info.name = "Object";
		info.size = sizeof(Object);
		info.createFn = []() { return  reinterpret_cast<void*>(new Object()); };

		GetTypeRegistry()[info.name] = &info;
		initalized = true;
	}
	return &info;
}

static struct __SYSTEM_REFLECTION_REGISTRATION_Object {
	__SYSTEM_REFLECTION_REGISTRATION_Object() {
		Object::StaticTypeInfo();
		printf("Object class registered!\n");
	}
} __SYSTEM_REFLECTED_Object;
*/



Object::Object()
{
	m_guid = Optim::Random::getGetGuid();
	objectRegistery.emplace(m_guid, this);
	
	//std::cout << std::dec
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

CORE_API Object* Object::getObject(const SGuid& guid)
{
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

#define OE_TYPEFACTORY(CLASS_TYPE) {#CLASS_TYPE, []() -> Object* { return new CLASS_TYPE; }}

std::unordered_map<std::string, Object*(*)()> g_typesFactoryRegistery = 
{
	//{"Object", []() -> Object* { return new Object; }},
	OE_TYPEFACTORY(Object),
	//OE_TYPEFACTORY(Mesh),
};

Object* Type::getObject(const char* type)
{
	auto pProc = g_typesFactoryRegistery.find(type);
	if (pProc == g_typesFactoryRegistery.end()) {
		printf("Could not find any function of type: %s\n", type);
		return nullptr;
	}
	return pProc->second();
}