#pragma once

#include <ShaderCompiler/ShaderCompiler.h>

#include <Foundation/Basics.h>
#include <Foundation/Logging/Log.h>

/// \brief XII ComPtr to automatically free resources.
template <typename T>
struct xiiComPtr
{
public:
  xiiComPtr() {}
  ~xiiComPtr()
  {
    if (m_pObject != nullptr)
    {
      m_pObject->Release();
      m_pObject = nullptr;
    }
  }

  xiiComPtr(const xiiComPtr& other) :
    m_pObject(other.m_pObject)
  {
    if (m_pObject)
    {
      m_pObject->AddRef();
    }
  }

  T*       operator->() { return m_pObject; }
  T* const operator->() const { return m_pObject; }

  T** Put()
  {
    XII_ASSERT_DEV(m_pObject == nullptr, "Can only put into an empty xiiComPtr");
    return &m_pObject;
  }

  T* RawPtr()
  {
    return m_pObject;
  }

  T** RawDblPtr()
  {
    return &m_pObject;
  }

  bool operator==(nullptr_t)
  {
    return m_pObject == nullptr;
  }

  bool operator!=(nullptr_t)
  {
    return m_pObject != nullptr;
  }

private:
  T* m_pObject = nullptr;
};
