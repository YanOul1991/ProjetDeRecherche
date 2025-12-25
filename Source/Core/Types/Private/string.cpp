/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      + string.cpp :
          Definitions of Optim Engine String class
          included in string.h 

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#include "Core/Types/string.h"
#include <cwchar>

int32 String::getLiteralSize(const wchar* str)
{
  if (str == nullptr) return 0;
  int32 i{ 0 };
  while (str[i] != TEXT('\0')) i++;
  return i;
}

String::String() noexcept :
  m_length{ 0 },
  m_buffer{ nullptr } 
{
  allocate(TEXT(""));
}

String::~String() noexcept
{
  freeBuffer();
}

String::String(const wchar* str) noexcept :
  m_length{ String::getLiteralSize(str) },
  m_buffer{ nullptr }
{
  allocate(str);
}

String::String(const String& other) noexcept :
  m_length{ 0 },
  m_buffer{ nullptr }
{
  allocate(other.value());
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
bool String::isAllocated()    const { return m_buffer != nullptr; }

/* #######################################
    Operator Overloads
####################################### */

String& String::operator=(const wchar* str) noexcept
{
  freeBuffer();
  allocate(str);
  return *this;
}

String& String::operator=(const String& other) noexcept
{
  if (this != &other)
  {
    freeBuffer();
    allocate(other.m_buffer);
  }
  return *this;
}

String& String::operator=(String&& other) noexcept
{
  if (&other != this)
  {
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

  if (strSize > 0 && str != nullptr)
  {
    int32 _bufferStrLength{ m_length + strSize };

    // Alloc new buffer memeory
    wchar* newbuffer = new wchar[_bufferStrLength + 1];

    copyToBuffer(0, newbuffer, m_buffer);
    copyToBuffer(m_length, newbuffer, str);

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

void String::allocate(const wchar* str)
{
  m_length = String::getLiteralSize(str);
  m_buffer = new wchar[m_length + 1];
  if (m_length > 0)
  {
    for (int i = 0; i < m_length; i++) m_buffer[i] = str[i];
  }
  m_buffer[m_length] = L'\0';

}

void String::freeBuffer()
{
  delete[] m_buffer;
  m_buffer = nullptr;
  m_length = 0;
}

void String::copyToBuffer(int32 outStart, wchar* outbuffer, const wchar* inBuffer)
{
  int32 writeIndex = outStart;
  int32 readIndex = 0;

  while (inBuffer[readIndex] != L'\0')
  {
    outbuffer[writeIndex] = inBuffer[readIndex]; 
    writeIndex++;
    readIndex++;
  }
}
