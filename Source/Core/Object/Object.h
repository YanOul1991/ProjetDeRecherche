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

typedef struct Type {
	const char*	name;
	const Type* parent;
	static Object* getObject(const char* type);
} Type;

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
	virtual const Type* getTypeInfo() const { return &typeInfo; }

	CORE_API static Object* getObject(const SGuid& guid);

	CORE_API Object();
	CORE_API virtual ~Object() {};

	//virtual inline SGuid getGuid() const {
	//	return m_guid;
	//}

protected:
	//SGuid m_guid{};
};