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
  static String sprintf(const wchar* string, ...);
  static void   printf(const char* format, ...);
  static bool   compare(const String& string1, const String& string2);
  static bool   isEmpty(const String& other);
  static int32  getLiteralSize(const wchar* str);

  String  () noexcept;
  ~String () noexcept;

  // Copy and move

  String(const wchar* str)    noexcept;
  String(const String& other) noexcept;
  String(String&& other)      noexcept;

  // Member functions

  int length()          const;
  const wchar* value()  const;

  // Operator Overloads

  String& operator=   (const wchar* str)    noexcept;
  String& operator=   (const String& other) noexcept;
  String& operator=   (String&& other)      noexcept;
  String& operator+=  (const wchar* str)    noexcept;
  String& operator+=  (const String& other) noexcept;
  String  operator+   (const wchar* str)    noexcept;
  String  operator+   (const String& other) noexcept;

private:
  wchar* m_buffer;
  int m_length;

  void freeBuffer();
};