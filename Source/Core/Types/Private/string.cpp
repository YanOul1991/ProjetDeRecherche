/* ======================================================================================
 *  String.cpp
 *
 *  By:
 *    Yanis Oulmane
 *
====================================================================================== */

#include "Core/OptimEngine.h"
#include "Core/Types/String.h"

#include <cstring>
#include <cstdarg>
#include <cwchar>
#include <cstdlib>
#include <locale.h>

int32 String::getLiteralSize(const wchar* str)
{
  setlocale(LC_ALL, "");
  return (int32)wcsnlen(str, 256);
}

String String::sprintf(const wchar* string,...)
{
  wchar buffer[256];
  va_list args;

  va_start(args, string);

  vswprintf(buffer, 256, string, args);

  va_end(args);

  return String(buffer);
}

void String::printf(const char* format, ...)
{
  char buffer[256]{};
  va_list args{};

  va_start(args, format);

  vprintf(format, args);

  va_end(args);
}

bool String::isEmpty(const String& other)
{
  return other.length() == 0 || other.m_buffer[0] == '\0' || other.m_buffer == nullptr;
}

bool String::compare(const String& string1, const String& string2)
{
  if (string1.length() != string2.length()) {
    return false;
  }
  return memcmp(string1.value(), string2.value(), sizeof(wchar) * string1.length()) == 0;
}

String::String() noexcept :
  m_length{ 0 },
  m_buffer{ nullptr } 
{
  m_buffer = new wchar[1];
  m_buffer[0] = '\0';
}

String::~String() noexcept
{
  printf("String has been deleted :D At following address:\n0x%02x\n", this);
  freeBuffer();
}

String::String(const wchar* str) noexcept :
  m_length{ String::getLiteralSize(str) },
  m_buffer{ nullptr }
{
  m_buffer = new wchar[m_length + 1];
  memcpy(m_buffer, str, sizeof(*str) * (m_length + 1));
}

String::String(const String& other) noexcept :
  m_length{ 0 },
  m_buffer{ nullptr }
{
  m_length = other.length();
  m_buffer = new wchar[m_length];
  memcpy(m_buffer, other.value(), sizeof(wchar) * m_length);
}

String::String(String&& other) noexcept :
  m_length{ other.m_length },
  m_buffer{ other.m_buffer }
{
  other.m_buffer = nullptr;
  other.m_length = 0;
}

int32 String::length()        const { return m_length; }
const wchar* String::value()  const { return m_buffer; }

/* #######################################
    Operator Overloads
####################################### */

String& String::operator=(const wchar* str) noexcept
{
  printf("Using overload: operator=(const wchar* str)\n");
  freeBuffer();
  int size = getLiteralSize(str);
  m_length = size;
  m_buffer = new wchar[size + 1];
  memcpy(m_buffer, str, sizeof(*m_buffer) * (size + 1));
  return *this;
}

String& String::operator=(const String& other) noexcept
{
  if (this != &other)
  {
    freeBuffer();
    m_length = other.m_length;
    m_buffer = new wchar[m_length + 1];
    memcpy(m_buffer, other.m_buffer, sizeof(*m_buffer) * (m_length + 1));
  }
  return *this;
}

String& String::operator=(String&& other) noexcept
{
  if (&other != this) {
    freeBuffer();
    m_buffer = other.m_buffer;
    m_length = other.m_length;
    other.m_buffer = nullptr;
    other.m_length = 0;
  }
  return *this;
}

String& String::operator+=(const wchar* str) noexcept
{
  int32 strSize{ String::getLiteralSize(str) };

  if (strSize > 0 && str != nullptr) {
    int32 _bufferStrLength{ m_length + strSize };

    // Alloc new buffer memeory
    wchar* newbuffer = new wchar[_bufferStrLength + 1];

    memcpy(newbuffer, m_buffer, sizeof(*m_buffer) * m_length);
    memcpy(newbuffer + m_length, str, sizeof(wchar) * strSize);

    newbuffer[_bufferStrLength] = '\0';

    // Free old buffer
    freeBuffer();

    m_buffer = newbuffer;
    m_length = _bufferStrLength;
    newbuffer = nullptr;
  }
  return *this;
}

String& String::operator+=(const String& other) noexcept
{
  int32 otherSize{ other.length() };
  if (otherSize > 0 && other.m_buffer != nullptr)
  {
    *this += other.m_buffer;
  }
  return *this;
}

String String::operator+(const wchar* str) noexcept
{
  String _newStr = String(this->value());
  _newStr += str;
  return _newStr;
}

String String::operator+(const String& other) noexcept
{
  String _newStr = String(this->value());
  _newStr += other;
  return _newStr;
}

void String::freeBuffer()
{
  delete[] m_buffer;
  m_buffer = nullptr;
  m_length = 0;
}