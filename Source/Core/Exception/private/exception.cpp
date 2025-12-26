/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      + exception.cpp:

      + By:
          Yanis Oulmane
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#include "Core/Types/string.h"

#include "Core/Exception/exception.h"
#include <string>
#include <sstream>
#include <iomanip>

op::Exception::Exception(int line, const wchar* file, const wchar* message) noexcept :
  m_line{ line },
  m_file{ file },
  m_message{ String(message) },
  m_errorCode { 0 }
{ }

op::Exception::Exception(int line, const wchar* file, String&& message) :
  m_line{ line }, 
  m_file{ file }, 
  m_message{ message }, 
  m_errorCode{ 0 }
{ }

op::Exception::~Exception() noexcept
{ }

op::Exception::Exception(int line, const wchar * file, int errorCode, const wchar* errorString, String&& message) :
  m_line { line },
  m_file { file },
  m_errorCode { errorCode },
  m_errorString{ errorString },
  m_message{ message }
{
}

const wchar* op::Exception::what() const noexcept
{
  return m_message.value();
}

const String op::Exception::whatDescriptive() const noexcept
{
  std::wstringstream wss;
  wss
    << type() << L"\n\n"
    << TEXT("[File]\n") << m_file << L"\n\n"
    << TEXT("[Line]\n") << m_line << L"\n\n"
    << TEXT("[Error Code]\n0X") << std::hex << m_errorCode << std::dec << m_errorCode << L"\n\n"
    << TEXT("[Error String]\n") << m_errorString << L"\n\n"
    << TEXT("[Error Description]\n") << m_message.value() << "\n\n";

  return String(wss.str().c_str());
}

const wchar* op::Exception::type() const noexcept
{
  return TEXT("Optim Engine Exception");
}

const wchar* op::Exception::getFile() const noexcept
{
  return m_file;
}

int op::Exception::getLine() const noexcept
{
  return m_line;
}

int op::Exception::getErrorCode() const
{
    return m_errorCode;
}
