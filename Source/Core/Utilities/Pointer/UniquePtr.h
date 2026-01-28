
#pragma once

#include "Core/OptimEngine.h"

template <typename T>
class UniquePtr final
{
public:
  inline UniquePtr(){ 
    //ptr = new T;
    //String::printf("UniquePtr created with no ptr 0x%02x\n", ptr);
  }

  explicit UniquePtr(T* p)
  {
    if (p) {
      ptr = p;
    }
  }

  explicit UniquePtr(T** pp) {
    if (pp) {
      ptr = *pp;
      *pp = nullptr;
    }
    //String::printf("UniquePtr created by pointer to pointer Object 0x%02x\n", ptr);
  }

  ~UniquePtr()
  {
    //String::printf("A UniquePtr out of scope...\n");
    if (ptr != nullptr) {
      //String::printf("Non null pointer has been freed.\n");
      delete ptr;
      ptr = nullptr;
    }
  }

  UniquePtr(const UniquePtr&) = delete;
  UniquePtr& operator=(const UniquePtr&) = delete;

  UniquePtr(UniquePtr&& other) noexcept : ptr{ other.ptr } {
    //String::printf("SmartPointer ownership transform by RVALUE INITIALIZATION. Now holding: 0x%02x\n", other.ptr);
    other.ptr = nullptr;
  }

  UniquePtr& operator=(UniquePtr&& other) noexcept {
    if (&other != this) {
      delete ptr;
      ptr = other.ptr;
      other.ptr = nullptr;
      //String::printf("SmartPointer ownership transform by RVALUE MOVE. Now holding: 0x%02x\n", ptr);
    }
    return *this;
  }

  UniquePtr<T>&& move() {
    //String::printf("UniquePtr returning self by RVALUE...\n");
    return static_cast<UniquePtr<T>&&>(*this);
  }

  /*
   * @brief
   * 
   * Create a new instance of the underlying type.
   * If the UniquePtr already holds an instance, 
   * its gets deleted and a new one is created.
  */
  void init() {
    //String::printf("UniquePtr initalizing new value\n");
    if (ptr != nullptr) {
      delete ptr;
    }
    ptr = new T;
  }

  void free() {
    //String::printf("A UniquePtr | free() callback...\n");
    if (ptr != nullptr) {
      //String::printf("Non null pointer has been freed.\n");
      delete ptr;
      ptr = nullptr;
    }
  }

  T* address() const {
    return ptr;
  }

  T** pAddress() {
    return &ptr;
  }

  T* operator->() const {
    return ptr;
  }
  T& operator*() const { 
    return *ptr; 
  }

private:
  T* ptr{nullptr};
};