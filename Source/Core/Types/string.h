/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      + string.h :
          Declarations of Optim Engine String class.

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#pragma once

#include <Core/OptimEngineGlobal.h>

class CORE_API String final
{
public:
  static int32 getLiteralSize(const wchar* str);

  String  () noexcept;
  ~String () noexcept;

  String  (const wchar* str)   noexcept;
  String  (const String& other) noexcept;
  String  (String&& other)      noexcept;

  // Member functions

  int length() const;
  const wchar* value() const;
  bool isAllocated() const;

  // Operator Overloads

  String& operator=   (const wchar* str)    noexcept;
  String& operator=   (const String& other) noexcept;
  String& operator=   (String&& other)      noexcept;
  String& operator+=  (const wchar* str)    noexcept;
  String& operator+=  (const String& other) noexcept;
  String  operator+   (const wchar* str)    noexcept;
  String  operator+   (const String& other) noexcept;

private:
  wchar_t* m_buffer;
  int m_length;

  void allocate(const wchar* str);
  void freeBuffer();
  void copyToBuffer(int32 outStart, wchar* outbuffer, const wchar* inBuffer);
};