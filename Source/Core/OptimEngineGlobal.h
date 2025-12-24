/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      + OptimEngineGlobal.h :
          Optim Engine global definitions.

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#pragma once

using int8        = signed char;
using int16       = short;
using int32       = int;
using int64       = long long;
using uint8       = unsigned char;
using uint16      = unsigned short;
using uint32      = unsigned int;
using uint64      = unsigned long long;
using wchar       = wchar_t;

#if !defined(__cplusplus)
  #error C++ is not defined.
#endif

/*
  OS defines
*/
#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS
#elif defined(__linux__)
    #define OS_LINUX
#endif

/*
  Compiler defines
*/
#if defined(_MSC_VER)
    #define COMPILER_MSVC
#elif defined(__GNUC__)
    #define COMPILER_GNU
#else
  #error No valid compiler found.
#endif

/*
    Compiler specific abstraction
*/
#if defined(OS_WINDOWS) && defined(COMPILER_MSVC)
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
    #define FORCE_INLINE __forceinline
    #define WINDOW_PROC
#elif defined(COMPILER_GNU)     
    #define FORCE_INLINE inline __attribute__((always_inline))
#endif

#ifdef OS_WINDOWS
    #ifndef UNICODE
        #define UNICODE
    #endif // !UNICODE

    #ifndef WIN_32_LEAN_AND_MEAN
        #define WIN_32_LEAN_AND_MEAN
    #endif // !WIN_32_LEAN_AND_MEAN
    
    #include <Windows.h>
    #include <ShObjIdl.h>
    #include <ShlGuid.h>
    #include <objbase.h>
    #include <ShellScalingApi.h>

    #pragma comment(lib, "kernel32")
    #pragma comment(lib, "user32")
    #pragma comment(lib, "gdi32")
    #pragma comment(lib, "Shell32")
    #pragma comment(lib, "Ole32")

#endif // OS_WINDOWS

#if defined(UNICODE)
  #define STRING(_TEXT_) L##_TEXT_
#else
  #define STRING(_TEXT_) _TEXT_ 
#endif

#define PROC_PTR(T) T*(*)()
#define PROC_PTR_PARAMS(T)

namespace op::system
{
  template <typename T>
  inline T LOAD_LIB_PROC(const char* _path_, const char* _proc_name_)
  {
    // WINDOWS DEFINITION
#if defined(OS_WINDOWS) 
    HMODULE pMod = LoadLibraryA(_path_);
    if (!pMod)
      return 0;

    T proc = (T)GetProcAddress(pMod, _proc_name_);

    if (!proc)
      return nullptr;
    return proc;
#endif // OS_WINDOWS
  }
}


/*
  Custom String class for optimized and more controled operations 
  with the Optime Engine APIs.
*/
class String final
{
public:
  // STATIC FIELDS
  inline static int32 getLiteralSize(const wchar* str)
  {
    if (str == nullptr)
      return 0;

    int32 i{ 0 };
    while (str[i] != STRING('\0')) i++;
    return i;
  }

  inline String() noexcept :
    m_length{ 0 },
    m_buffer{ nullptr }
  {
    allocate(STRING(""));
  }

  inline String(const wchar* str) noexcept :
    m_length{ String::getLiteralSize(str) },
    m_buffer{ nullptr }
  {
    allocate(str);
  }

  inline String(String&& other) noexcept :
    m_length { 0 },
    m_buffer { nullptr }
  {
    m_buffer = other.m_buffer;
    other.freeBuffer();
  }

  inline ~String()
  { 
    freeBuffer();
  }

  inline int32 length() const 
  { 
    return m_length; 
  }

  inline const wchar* value() const 
  {
    return m_buffer;
  }

  inline bool isAllocated() const
  {
    return m_buffer != nullptr;
  }

  /////////////////////////
  //// OPERATOR OVERLOADING
  /////////////////////////

  inline void operator=(const wchar* str) noexcept
  {
    freeBuffer();
    allocate(str);
  }

  inline String& operator=(const String& other) noexcept
  {
    if (this != &other)
    {
      freeBuffer();
      if (other.m_buffer != nullptr)
      {
        allocate(other.m_buffer);
      }
    }
    return *this;
  }

  inline String& operator=(String&& other) noexcept
  {
    if (this != &other)
    {
      freeBuffer();
      m_buffer = other.m_buffer;
      other.freeBuffer();
    }
    return *this;
  }

  inline String& operator+(const wchar* str) noexcept
  {
    int32 strSize{ String::getLiteralSize(str) };

    if (strSize > 0 && str != nullptr)
    {
      int32 _bufferStrLength{ m_length + strSize };

      // Alloc new buffer memeory
      wchar* newbuffer = new wchar[_bufferStrLength + 1];

      int32 readIndex = 0;
      int32 outindex  = 0;

      // Move original string to new location
      //while (m_buffer[readIndex])
      //{
      //  newbuffer[outindex] = m_buffer[readIndex];
      //  readIndex++;
      //  outindex++;
      //}

      copyToBuffer(0, newbuffer, m_buffer);
      copyToBuffer(m_length, newbuffer, str);

      //outindex = m_length;

      //readIndex = 0;

      //// Append new string to new location
      //while (str[readIndex] != L'\0')
      //{
      //  newbuffer[outindex] = str[readIndex];
      //  readIndex++;
      //  outindex++;
      //}

      newbuffer[_bufferStrLength] = '\0';

      // Free old buffer
      freeBuffer();

      m_buffer    = newbuffer;
      m_length    = _bufferStrLength;
      newbuffer   = nullptr;
    }
    return *this;
  }

private:
  wchar* m_buffer;
  int32 m_length;

  inline void allocate(const wchar* str)
  {
    m_length = String::getLiteralSize(str);
    m_buffer = new wchar[m_length + 1];

    if (m_length > 0)
    {
      for (int i = 0; i < m_length; i++) m_buffer[i] = str[i];
    }

    m_buffer[m_length] = L'\0';
  }

  inline void freeBuffer()
  {
    delete[] m_buffer;
    m_buffer = nullptr;
    m_length = 0;
  }

  inline void copyToBuffer(int writeBufferStartIndex, wchar* writeBuffer, const wchar* readBuffer)
  {
    int32 outIndex            = writeBufferStartIndex;
    int32 readBufferLength    = String::getLiteralSize(readBuffer);
    int32 writeBufferLength   = String::getLiteralSize(writeBuffer);

    if (outIndex > writeBufferLength) 
      return;

    int32 readIndex{ 0 };

    while (readBuffer[readIndex] != L'\0')
    {
      writeBuffer[outIndex] = readBuffer[readIndex];
      outIndex++;
      readIndex++;
    }
  }
};