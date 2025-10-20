#include "Object.h"
#include "Editor/MainWindow.h"
#include "Random/Random.h"

// Constructor definition
Object::Object(MainWindow& ptrMainWindow) :
  pMainWindow{ ptrMainWindow },
  m_guid{get_random_id()}
{ 
  // MessageBoxA(0, "A new Object!", "DEBUG", MB_OK + MB_ICONASTERISK);
  sm_registery.insert({m_guid, this});
  test();
}

std::unordered_map<guid_t, Object *> Object::sm_registery{};

std::unique_ptr<Object> Object::instanciate(const Object&& object)
{
  // Create a copy of the given object parameter
  // And return it as a smart pointer
  return std::make_unique<Object>(object);
}

void Object::destroy(Object& object)
{
  delete(sm_registery[object.getInstanceID()]);
}


guid_t Object::getInstanceID() const { return m_guid; }

Object::~Object()
{
  sm_registery.erase(m_guid);
}

std::string Object::id() const 
{
  return get_guid_str(m_guid);
}

void Object::update() 
{ 
  std::string strId = id();
  MessageBoxA(0, strId.c_str(), "An Object's update was called! ", MB_OK);
}

void Object::test()
{
  //pMainWindow.OnClick().subscribe([this]() {update();});
}
