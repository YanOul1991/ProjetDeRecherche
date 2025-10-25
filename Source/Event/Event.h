#pragma once

#ifdef DLL_LIB
#define EVENT_API __declspec(dllexport)
#else
#define EVENT_API __declspec(dllimport)
#endif

#include "Global/OptimEngineGlobal.h"
#include <functional>
#include <unordered_map>

// Forward declarations
class Object;
class Event;
class Delegate;

// EVENT MACRO

#define DECLARE_EVENT(OwnerType, EventName) \
class EventName : Event { \
  friend class OwnerType; \
private:\
  void broadcast() { invoke(); }\
public:\
  void subscribe(std::function<void()> function) { add(function); }\
};\

class EVENT_API Delegate
{
public:
  Delegate();
  void subscribe(std::function<void()> callback);
  void unsubscribe();
  void trigger();
  std::function<void()> m_callback;

private:
};

class EVENT_API Event
{
public:
  std::vector<std::function<void()>> m_callbacks;
  void add(std::function<void()> function);
  void invoke();
};
