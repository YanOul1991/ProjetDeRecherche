/**
  String.cpp
 */

#include "Core/Types/String.h"

#include "Core/OptimEngine.h"

#include <clocale>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

/**
 * \brief
 * Get the length of a string literal. The length does NOT include the null terminator '\0'.
 */
uint32 String::getLiteralSize(const char* str, uint64 maxSize) {
  return static_cast<uint32>(strnlen(str, maxSize));
}

String String::sprintf(const char* format, ...) {
  va_list args;
  va_start(args, format);

  // Make copy of args get needed buffer
  // size to hold output.
  va_list argsCpy;
  va_copy(argsCpy, args);
  int needed = ::vsnprintf(nullptr, 0, format, argsCpy);
  va_end(argsCpy);

  // In case of formatting error
  // return empty string
  if (needed < 0) {
    va_end(args);
    return String();
  }

  // Allocate buffer
  // String class automatically adds null terminator
  std::vector<char> buffer(needed + 1);

  ::vsnprintf(buffer.data(), buffer.size(), format, args);

  va_end(args);

  return String(buffer.data());
}

void String::printf(const char* format, ...) {
  char    buffer[512]{};
  va_list args{};

  va_start(args, format);

  ::vprintf(format, args);

  va_end(args);
}

bool String::isEmpty(const String& other) {
  return other.length() == 0 || other.m_buffer[0] == '\0' || other.m_buffer == nullptr;
}

bool String::compare(const String& string1, const String& string2) {
  if (string1.length() != string2.length()) {
    return false;
  }
  return memcmp(string1.value(), string2.value(), sizeof(char) * string1.length()) == 0;
}

String::String() noexcept :
    m_length{0},
    m_buffer{nullptr} {
  m_buffer    = new char[1];
  m_buffer[0] = '\0';
}

String::~String() noexcept {
  //printf("String has been deleted at following address: 0x%02x\n", this);
  freeBuffer();
}

String::String(const char* str) noexcept :
    m_length{String::getLiteralSize(str)},
    m_buffer{nullptr} {
  //printf("[String] C-String constructor at 0x%02x\n", this);
  m_buffer = new char[m_length + 1];
  memcpy(m_buffer, str, sizeof(char) * m_length);
  m_buffer[m_length] = '\0';
  //printf("[String] Value: %s\n", m_buffer);
}

String::String(const String& other) noexcept :
    m_length{0},
    m_buffer{nullptr} {
  //printf("[String] Copy constructor...\n");
  m_length = other.length();
  m_buffer = new char[m_length + 1];
  memcpy(m_buffer, other.value(), sizeof(char) * m_length);
  m_buffer[m_length] = '\0';
}

String::String(String&& other) noexcept :
    m_length{other.m_length},
    m_buffer{other.m_buffer} {
  //printf("[String] Move constructor...\n");
  other.m_buffer = nullptr;
  other.m_length = 0;
}

uint32 String::length() const {
  return m_length;
}
uint32 String::capacity() const {
  return 0;
}
const char* String::value() const {
  //printf("[String] Returning buffer value: %s\n", m_buffer);
  return m_buffer;
}

/* #######################################
    Operator Overloads
####################################### */

String& String::operator=(const char* str) noexcept {
  //printf("Using overload: operator=(const char* str)\n");
  freeBuffer();
  int size = getLiteralSize(str);
  m_length = size;
  m_buffer = new char[size + 1];
  memcpy(m_buffer, str, sizeof(char) * m_length);
  m_buffer[m_length] = '\0';
  return *this;
}

String& String::operator=(const String& other) noexcept {
  //printf("[String] Copy assignement...");
  if (this != &other) {
    freeBuffer();
    m_length = other.m_length;
    m_buffer = new char[m_length + 1];
    memcpy(m_buffer, other.m_buffer, sizeof(char) * m_length);
    m_buffer[m_length] = '\0';
  }
  return *this;
}

String& String::operator=(String&& other) noexcept {
  if (&other != this) {
    freeBuffer();
    m_buffer       = other.m_buffer;
    m_length       = other.m_length;
    other.m_buffer = nullptr;
    other.m_length = 0;
  }
  return *this;
}

String& String::operator+=(const char* str) noexcept {
  uint32 strSize{String::getLiteralSize(str)};

  if (strSize > 0 && str != nullptr) {
    uint32 _bufferStrLength{m_length + strSize};

    // Alloc new buffer memeory
    char* newbuffer = new char[_bufferStrLength + 1];

    memcpy(newbuffer, m_buffer, sizeof(char) * m_length);
    memcpy(newbuffer + m_length, str, sizeof(char) * strSize);

    newbuffer[_bufferStrLength] = '\0';

    // Free old buffer
    freeBuffer();

    m_buffer  = newbuffer;
    m_length  = _bufferStrLength;
    newbuffer = nullptr;
  }
  return *this;
}

String& String::operator+=(const String& other) noexcept {
  uint32 otherSize{other.length()};
  if (otherSize > 0 && other.m_buffer != nullptr) {
    *this += other.m_buffer;
  }
  return *this;
}

String String::operator+(const char* str) noexcept {
  String _newStr = String(this->value());
  _newStr       += str;
  return _newStr;
}

String String::operator+(const String& other) noexcept {
  String _newStr = String(this->value());
  _newStr       += other;
  return _newStr;
}

void String::freeBuffer() {
  if (m_buffer != nullptr) {
    delete[] m_buffer;
    m_buffer = nullptr;
  }
  m_length = 0;
}