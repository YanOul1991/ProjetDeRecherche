/* ======================================================================================
 *  Object.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Utilities/Random/Random.h"

#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <iomanip>
#include <unordered_map>

/*
enum ETypes {
	type_none = 0,
	type_int = 0,
	type_float = 0,
	type_bool = 0,
};

struct Type {
	const char* name = 0;
	void* data = 0;
	int type = 0;

	inline void assign(double number) {
		switch (type) {
			case type_int: {
				*((int*)data) = number;
				break;
			}
			default: {
				break;
			}
		}
	}

	inline double read() {
		switch (type) {
			case type_int: {
				return *((int*)data);
			}
			default: {
				break;
			}
		}
	}
};
*/

class Object;

struct Type {
	const char*	name;
	const Type* parent;
	static Object* getObject(const char* type);
};

struct property {
	const char* name;
	void* value;
};

class Object 
{
public:
	template<typename T>
	static T* type_cast(Object* obj) {
		if (obj == nullptr) {
			return nullptr;
		}

		const Type* ti = obj->getTypeInfo();
		const Type* target = &T::typeInfo;

		while (ti) {
			std::cout << "Type comparaison:\n";
			std::cout << "--- Object type : "	<< ti << "\n";
			std::cout << "--- Target type : "	<< target << "\n";
			if (ti == target) {
				return static_cast<T*>(obj);
			}
			ti = ti->parent;
		}
		return nullptr;
	}

	// CLASS TYPE INFO
	static constexpr Type typeInfo = { "Object", nullptr };
	virtual const Type* getTypeInfo() const		{ return &typeInfo; }

	static CORE_API Object* getObject(const SGuid& guid);

	CORE_API Object();
	CORE_API virtual ~Object() = default;

	virtual inline SGuid getGuid() const {
		return m_guid;
	}
	virtual inline void printHello() const {
		printf("Hello! I am a \"Object\" class object\n");
	}

protected:
	SGuid m_guid{};
};

class ChildClass : public Object
{
public:
	// CLASS TYPE INFO
	static constexpr Type typeInfo = { 
		"ChildClass", 
		&Object::typeInfo 
	};

	virtual const Type* getTypeInfo() const { 
		return &typeInfo; 
	}

	inline void printHello() const override { 
		printf("Hello! I am a \"Child class\" class object\n"); 
	}
};

class GrandChildClass : public ChildClass
{
public:
	// CLASS TYPE INFO
	static constexpr Type typeInfo = { 
		"GrandChildClass", 
		&ChildClass::typeInfo 
	};

	virtual const Type* getTypeInfo() const { 
		return &typeInfo; 
	}


	inline void printHello() const override { 
		printf("Hello! I am a \"GrandChildClass\" class object\n"); 
	}
};