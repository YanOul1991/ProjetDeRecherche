#pragma once

#include <string>
#include <Windows.h>

namespace op::win::shell
{
  HRESULT createDesktopShortcut(
    const std::wstring& _filePath,
    const std::wstring& _shortcutPath,
    const std::wstring& _description);

  HRESULT getFilePath(std::wstring* _str);
  HRESULT getDirPath(std::wstring* _str);
}
