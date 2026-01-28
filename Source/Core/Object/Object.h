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

class Object;

struct Type {
	const char*	name;
	const Type* parent;
	static Object* getObject(const char* type);
};

#define OE_OBJECT(OBJECT, PARENT_OBJECT) \
static constexpr Type typeInfo = { #OBJECT, &PARENT_OBJECT::typeInfo }; \
virtual const Type* getTypeInfo() const override { return &typeInfo; } \


class Object 
{
public:
	// CLASS TYPE INFO
	static constexpr Type typeInfo = { "Object", nullptr };
	virtual const Type* getTypeInfo() const { return &typeInfo; }

	template<typename T>
	static T* cast(Object* obj) {
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

	CORE_API static Object* getObject(const SGuid& guid);
	CORE_API Object();
	CORE_API virtual ~Object();

protected:
	SGuid m_guid{};

	CORE_API virtual void onTick(){}

private:
	CORE_API inline Object(SGuid param_guid) noexcept 
	{
		m_guid = param_guid;
	}
};