/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      + exception.cpp:

      + By:
          Yanis Oulmane
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#include "Core/Exception/exception.h"

#include "Core/Types/string.h"

#include <iomanip>
#include <sstream>
#include <string>

Exception::Exception(int line, const char* file, const char* message) noexcept :
    m_line{line},
    m_file{file},
    m_message{String(message)},
    m_errorCode{0} {
}

Exception::Exception(int line, const char* file, String&& message) :
    m_line{line},
    m_file{file},
    m_message{message},
    m_errorCode{0} {
}

Exception::~Exception() noexcept {
}

Exception::Exception(int line, const char* file, int errorCode, const char* errorString, String&& message) :
    m_line{line},
    m_file{file},
    m_errorCode{errorCode},
    m_errorString{errorString},
    m_message{message} {
}

const char* Exception::what() const noexcept {
  return m_message.value();
}

const String Exception::whatDescriptive() const noexcept {
  std::stringstream wss;
  wss
    << type() << "\n\n"
    << "[File]\n" << m_file << "\n\n"
    << "[Line]\n" << m_line << "\n\n"
    << "[Error Code]\n0X" << std::hex << m_errorCode << std::dec << m_errorCode << "\n\n"
    << "[Error String]\n" << m_errorString << "\n\n"
    << "[Error Description]\n" << m_message.value() << "\n\n";

  return String(wss.str().c_str());
}

const char* Exception::type() const noexcept {
  return "Optim Engine Exception";
}

const char* Exception::getFile() const noexcept {
  return m_file;
}

int Exception::getLine() const noexcept {
  return m_line;
}

int Exception::getErrorCode() const {
  return m_errorCode;
}
