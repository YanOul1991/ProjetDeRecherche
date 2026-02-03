#include "Event.h"

Delegate::Delegate() :
  m_callback{ nullptr }
{ }

// Subscribe to a delegate
void Delegate::subscribe(std::function<void()> callback)
{
  m_callback = callback;
}

void Delegate::unsubscribe() { m_callback = nullptr; }

void Delegate::trigger() { if (m_callback) m_callback(); }

// #################################### Event class definitions

void Event::add(std::function<void()> function)
{
  m_callbacks.push_back(function);
}

void Event::invoke()
{
  for (std::function<void()>& callback : m_callbacks)
  {
    callback();
  }
}
