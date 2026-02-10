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

struct TypeInfo {
	const char* name;
	size_t size;
	void* (*createFn)();
};

inline std::unordered_map<std::string, TypeInfo*>& GetTypeRegistry() { 
  static std::unordered_map<std::string, TypeInfo*> registry; 
	return registry; 
}

#define DECLARE_OBJECT(_TYPE_)																								\
	public: static TypeInfo* StaticTypeInfo();																	\
	public: virtual TypeInfo* GetTypeInfo() const {return StaticTypeInfo(); }		\

class Object;

struct Type {
	const char*	name;
	const Type* parent;
	static Object* getObject(const char* type);
};

#define OE_OBJECT(OBJECT, PARENT_OBJECT) \
static constexpr Type typeInfo = { #OBJECT, &PARENT_OBJECT::typeInfo }; \
virtual const Type* getTypeInfo() const override { return &typeInfo; } \


class CORE_API Object 
{
	DECLARE_OBJECT(Object)

public:
	Object();
	virtual ~Object();

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

	 static Object* getObject(const SGuid& guid);

protected:
	SGuid m_guid{};
	virtual void onTick(){}

private:
	inline Object(SGuid param_guid) noexcept 
	{
		m_guid = param_guid;
	}
};