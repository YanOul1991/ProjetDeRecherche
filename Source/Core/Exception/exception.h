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
  Exception(int line, const wchar* file, const wchar* message) noexcept;
  Exception(int line, const wchar* file, String&& message);
  ~Exception() noexcept;

  Exception(int line, const wchar* file, int errorCode, const wchar* errorString, String&& message);

  const wchar*  what()              const noexcept;
  const String  whatDescriptive()   const noexcept;
  const wchar*  type()              const noexcept;
  const wchar*  getFile()           const noexcept;
  int           getLine()           const noexcept;
  int           getErrorCode()      const;

private:
  const int m_line;
  const int m_errorCode;
  const wchar* m_file;
  const wchar* m_errorString;
  String m_message;
};
