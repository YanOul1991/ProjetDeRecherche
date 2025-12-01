#include <ShObjIdl_core.h>
#include <Windows.h>
#include "WindowsShell.h"
#include "WindowsShellTools.h"
#include <atlcomcli.h>

/////////////////////////////////////////////////////////////// FUNCTION : createDesktopShortcut
HRESULT op::win::shell::createDesktopShortcut(
  const std::wstring& _filePath,
  const std::wstring& _shortcutPath,
  const std::wstring& _description)
{
  // COM initialization
  HRESULT hr{ CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE) };
  if (FAILED(hr))return hr;

  //CComPtr<IFileOpenDialog> comFileOpen;
  //hr = comFileOpen.CoCreateInstance(CLSID_FileOpenDialog);

  // Instantiation of COM object: ShellLink -> IShellLink
  CoObj<IShellLinkW> coShellLink{};
  hr = coShellLink.CreateInstance(CLSID_ShellLink, IID_IShellLinkW);
  if (FAILED(hr)) return hr;

  // Set ShellLink targetPath and description
  coShellLink->SetPath(_filePath.c_str());
  coShellLink->SetDescription(_description.c_str());

  CoObj<IPersistFile> coPersistFile{};

  hr = coShellLink->QueryInterface(
    IID_IPersistFile,
    reinterpret_cast<void**>(coPersistFile.ptrAddr()));

  if (FAILED(hr)) return hr;

  coPersistFile->Save(_shortcutPath.c_str(), TRUE);

  CoUninitialize();
  return hr;
} // FUNCTION END: createDesktopShortcut

/////////////////////////////////////////////////////////////// FUNCTION : getFilePath
// Get the location of a file selected through the File Open Dialog
HRESULT op::win::shell::getFilePath(std::wstring* _str)
{
  // Initialize COM
  HRESULT hr{ CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE) };
  if (FAILED(hr)) return hr;

  // Create Smart COM Object of type FileOpenDialog
  CComPtr<IFileOpenDialog> comFileOpenDialog;
  hr = comFileOpenDialog.CoCreateInstance(CLSID_FileOpenDialog);
  //CoObj<IFileOpenDialog> coFileOpenDialog;
  //hr = coFileOpenDialog.CreateInstance(CLSID_FileOpenDialog, IID_IFileOpenDialog);
  if (FAILED(hr)) return hr;

  // Open the Window's File Open Dialog
  comFileOpenDialog->Show(NULL);
  //hr = coFileOpenDialog->Show(NULL);
  if (FAILED(hr)) return hr;

  // Create a ShellItem COM object
  //CoObj<IShellItem> coShellItem;
  CComPtr<IShellItem> comShellItem;
  comFileOpenDialog->GetResult(&comShellItem);
  //hr = coFileOpenDialog->GetResult(coShellItem.ptrAddr());
  if (FAILED(hr)) return hr;

  // Get the directory
  PWSTR pszDirectoryPath;
  //hr = coShellItem->GetDisplayName(SIGDN_FILESYSPATH, &pszDirectoryPath);
  hr = comShellItem->GetDisplayName(SIGDN_FILESYSPATH, &pszDirectoryPath);

  if (FAILED(hr))
  {
    CoTaskMemFree(pszDirectoryPath);
    return hr;
  }

  // Output directory path to a std::wstringstream
  *_str = pszDirectoryPath;
  CoTaskMemFree(pszDirectoryPath);

  CoUninitialize();
  return hr;
}

// @brief   Get the path of a chosen directory through Window's file opem dialog.
// @param   std::wstring* _str - dwbhj
HRESULT op::win::shell::getDirPath(std::wstring* _str)
{
  // Initialize COM
  HRESULT hr{ CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE) };
  if (FAILED(hr)) return hr;

  MessageBoxA(NULL, "COM successfully Initialized.", "Debug", MB_OK);

  // Declaration for COM object implementing IFileOpenDialog interface
  // Declaration of DWORD for dialog options
  CoObj<IFileOpenDialog> coFileOpenDialog{};

  hr = coFileOpenDialog.CreateInstance(CLSID_FileOpenDialog, IID_IFileOpenDialog);
  DWORD dwOptions;

  // Get default options
  hr = coFileOpenDialog->GetOptions(&dwOptions);
  if (FAILED(hr)) return hr;

  coFileOpenDialog->SetOptions(dwOptions | FOS_PICKFOLDERS);
  hr = coFileOpenDialog->Show(NULL);

  if (FAILED(hr)) return hr;

  CoObj<IShellItem> coShellItem{};

  hr = coFileOpenDialog->GetResult(coShellItem.ptrAddr());

  if (FAILED(hr)) return hr;

  PWSTR dirPath{};
  hr = coShellItem->GetDisplayName(SIGDN_FILESYSPATH, &dirPath);

  if (FAILED(hr))
  {
    CoTaskMemFree(dirPath);
    return hr;
  }

  *_str = dirPath;
  CoTaskMemFree(dirPath);

  CoUninitialize();
  return hr;
}