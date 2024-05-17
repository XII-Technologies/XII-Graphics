#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Declarations/DeviceObject.h>
#include <GraphicsFoundation/Declarations/GraphicsTypes.h>

/// \brief Base class for all GAL resources (textures, buffers, etc).
class XII_GRAPHICSFOUNDATION_DLL xiiGALResource : public xiiGALDeviceObject
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALResource, xiiGALDeviceObject);

public:
protected:
  friend class xiiGALDevice;

  xiiGALResource(xiiGALDevice* pDevice);

  xiiHashTable<xiiUInt32, xiiGALBufferViewHandle>  m_BufferViews;
  xiiHashTable<xiiUInt32, xiiGALTextureViewHandle> m_TextureViews;

  xiiGALBufferViewHandle  m_DefaultBufferViews[xiiGALBufferViewType::ENUM_COUNT];
  xiiGALTextureViewHandle m_DefaultTextureViews[xiiGALTextureViewType::ENUM_COUNT];
};

/// \brief Base class for all GAL resource views (texture and buffer views).
class XII_GRAPHICSFOUNDATION_DLL xiiGALResourceView : public xiiGALDeviceObject
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALResourceView, xiiGALDeviceObject);

public:
protected:
  friend class xiiGALDevice;

  xiiGALResourceView(xiiGALDevice* pDevice);
};

#include <GraphicsFoundation/Resources/Implementation/Resource_inl.h>
