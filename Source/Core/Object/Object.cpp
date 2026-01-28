/* ======================================================================================
 *  Object.cpp:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#include "Core/Defines/Windows/windowsAPI.h"

#include "Core/OptimEngine.h"
#include "Core/Object/Object.h"
//#include "Core/Graphics/Mesh.h"

#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <iomanip>
#include <unordered_map>

static std::unordered_map<SGuid, Object*> objectRegistery{};

Object::Object()
{
	m_guid = Optim::Random::getGetGuid();
	objectRegistery.emplace(m_guid, this);
	
	std::cout << std::dec
		<< "[Object] A new Object was initialized:\n"
		<< "----- Registery count : " << objectRegistery.size() << '\n'
		<< "----- Object type     : " << objectRegistery[m_guid]->getTypeInfo()->name << '\n'
		<< "----- Object address  : " << std::hex << "0x" << this << '\n' << std::dec;
	
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