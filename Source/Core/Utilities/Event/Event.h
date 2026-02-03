// Event.g

#pragma once

#include "Core/OptimEngine.h"
#include <cstdio>
#include <vector>

enum class EBinding
{
	Instanced,
	Static,
	Lambda
};

//struct SCallback
//{
//	EBinding type;
//	void *instance;
//	void (*staticProc)();
//	uint64 lambdaHandle;
//	void (*thunk)(void*, Args...);
//};

//class Delegate
//{
//public:
//	template<typename T, typename U>
//	inline void addObject(T*, void(U::*proc)())
//	{
//
//	}
//
//private:
//	void* instance;
//	void (*thunk)(void*);
//};

class Event
{
public:
	template<typename T, void (T::*func)()>
	void subscribe(T* instance)
	{
		EventCallback l_callBack{};
		l_callBack.instance = instance;
		l_callBack.Thunk = [](void* obj) { 
					T* inst = reinterpret_cast<T*>(obj); 
					(inst->*func)(); 
		};
		callBacks.push_back(static_cast<EventCallback&&>(l_callBack));
	}
	inline void broadcast()
	{
		for (auto& callback : callBacks) {
			if (callback.instance != nullptr && callback.Thunk != nullptr) {
				callback.Thunk(callback.instance);
			}
		}
	}
private:
	struct EventCallback {
		void* instance;
		void (*Thunk)(void*);
	};

	std::vector<EventCallback> callBacks{};
};

template<typename... Args>
class EventArgs
{
public:
	template<typename T, void(T::*func)(Args...)>
	void subscribe(T* instance)
	{
		printf("Member function size %llu.\n", sizeof(func));

		Callback l_callBack{};
		l_callBack.subscriber = instance;

		l_callBack.thunk = [](void* obj, Args... args) { 
					T* inst = reinterpret_cast<T*>(obj); 
					(inst->*func)(args...); 
		};


		l_callBack.procPtr = (void*)&func;
		printf("Proc ptr at                       | %p.\n", l_callBack.procPtr);

		callbacks.push_back(static_cast<Callback&&>(l_callBack));
	}

	void broadcast(Args... args)
	{
		printf("Variable args event broadcast.\n");
		for (Callback& cb : callbacks) {
			if (cb.subscriber != nullptr && cb.thunk != nullptr) {
				cb.thunk(cb.subscriber, args...);
			}
		}
	}

	/*
	template<typename T, void(T::*func)(Args...)>
	void unsubscribe(T* instance)
	{
		void* l_procPtr = reinterpret_cast<void*>(&func);
		printf("trying to remove", l_procPtr);

		callbacks.erase(
			std::remove_if(
				callbacks.begin(), 
				callbacks.end(), 
				[&](const Callback& cb) {
					return cb.subscriber == instance && cb.procPtr == l_procPtr;
				},
				callbacks.end())
		);

		printf("Instance at %p removed its function at : %p", instance, func);
	}
	*/

private:
	struct Callback
	{
		void* subscriber;
		void (*thunk)(void*, Args...);
		void* procPtr;
	};

	std::vector<Callback> callbacks{};
};