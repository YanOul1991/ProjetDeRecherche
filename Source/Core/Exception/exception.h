/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      + exception.h:

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#pragma once

#include <Core/OptimEngineGlobal.h>

namespace op
{
  class Exception
  {
  public:
    Exception(int line, char* file) noexcept;
    ~Exception() noexcept;
    const char* message() const noexcept;
    const char* type() const noexcept;
    int getLine() const noexcept;
    const char* getFile() const noexcept;

  private:
    int m_line;
    char* m_file;
    char* m_message_buffer;
  };
}
