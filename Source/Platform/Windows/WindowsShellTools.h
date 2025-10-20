#pragma once

namespace OShell
{
  // Call the release function of COM objects
  template <class T>
  void safeRelease(T** ppT)
  {
    if (*ppT)
    {
      (*ppT)->Release();
      *ppT = NULL;
    }
  }

  // Smart COM Object.
  template <class T>
  class CoObj
  {
  public:
    explicit CoObj() {};
    ~CoObj() { safeRelease(&m_ptr); }

    // No copy
    CoObj(const CoObj&) = delete;
    CoObj& operator=(const CoObj&) = delete;

    // Move semantics
    CoObj(CoObj&& _other) noexcept : m_ptr{ _other.m_ptr }
    {
      _other.m_ptr = nullptr;
    }

    CoObj& operator=(CoObj&& _other) noexcept
    {
      if (this != &_other)
      {
        m_ptr = _other.m_ptr;
        _other.m_ptr = nullptr;
      }
      return *this;
    }

    // Create COM Object instance
    HRESULT CreateInstance(const IID& _rclsid, const IID& _riid, DWORD dwClsContext = CLSCTX_INPROC_SERVER)
    {
      return CoCreateInstance(
        _rclsid,
        NULL,
        dwClsContext,
        _riid,
        reinterpret_cast<void**>(&m_ptr));
    }

    bool isNull() { return m_ptr == nullptr; }
    T& operator*() const { return &m_ptr; }
    T* operator->() const { return m_ptr; }
    T** ptrAddr() { return &m_ptr; }

  private:
    T* m_ptr{};
  };
}