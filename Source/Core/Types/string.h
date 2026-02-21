/* ======================================================================================
 *  string.h:
 *
 *  By:
 *    Yanis Oulmane
 *
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"

class CORE_API String final
{
 public:
  static String sprintf(const char* string, ...);
  static void   printf(const char* format, ...);
  static bool   compare(const String& string1, const String& string2);
  static bool   isEmpty(const String& other);
  static uint32 getLiteralSize(const char* str, uint64 maxSize= 1024);

  String() noexcept;
  ~String() noexcept;

  // Copy and move

  String(const char* str) noexcept;
  String(const String& other) noexcept;
  String(String&& other) noexcept;

  // Member functions

  uint32      length() const;
  uint32      capacity() const;
  const char* value() const;

  // Operator Overloads

  String& operator=(const char* str) noexcept;
  String& operator=(const String& other) noexcept;
  String& operator=(String&& other) noexcept;
  String& operator+=(const char* str) noexcept;
  String& operator+=(const String& other) noexcept;
  String  operator+(const char* str) noexcept;
  String  operator+(const String& other) noexcept;

 private:
  char*  m_buffer;
  uint32 m_length;
  // uint32 m_capacity;

  void freeBuffer();
};