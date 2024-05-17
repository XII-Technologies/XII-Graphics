#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

/// \brief Base Graphics Abstraction Layer Object.
class XII_GRAPHICSFOUNDATION_DLL xiiGALObject : public xiiReflectedClass, public xiiRefCounted
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALObject, xiiReflectedClass);

public:
  /// \brief Returns the debug name of this resource.
  XII_NODISCARD xiiStringView GetDebugName() const;

  /// \brief Sets the debug name for this resource.
  void SetDebugName(xiiStringView sDebugName);

protected:
  friend class xiiGALDevice;

  virtual void SetDebugNamePlatform(xiiStringView sName){};

private:
  mutable xiiHashedString m_sDebugName;
};

#include <GraphicsFoundation/Declarations/Implementation/Object_inl.h>
