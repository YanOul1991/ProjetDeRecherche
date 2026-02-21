/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
      + exception.h:
      + By:
          Yanis Oulmane
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Types/String.h"

class CORE_API Exception
{
 public:
  Exception(int line, const char* file, const char* message) noexcept;
  Exception(int line, const char* file, String&& message);
  ~Exception() noexcept;

  Exception(int line, const char* file, int errorCode, const char* errorString, String&& message);

  const char*  what() const noexcept;
  const String whatDescriptive() const noexcept;
  const char*  type() const noexcept;
  const char*  getFile() const noexcept;
  int32        getLine() const noexcept;
  int32        getErrorCode() const;

 private:
  const int32 m_line;
  const int32 m_errorCode;
  const char* m_file;
  const char* m_errorString;
  String      m_message;
};
