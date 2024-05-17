#pragma once

#include <GraphicsNull/GraphicsNullDLL.h>

#include <GraphicsFoundation/Resources/BufferView.h>

class XII_GRAPHICSNULL_DLL xiiGALBufferViewNull final : public xiiGALBufferView
{
public:
protected:
  friend class xiiGALDeviceNull;
  friend class xiiMemoryUtils;

  xiiGALBufferViewNull(xiiGALDeviceNull* pDeviceNull, xiiGALBuffer* pBuffer, const xiiGALBufferViewCreationDescription& creationDescription);

  virtual ~xiiGALBufferViewNull();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;
};

#include <GraphicsNull/Resources/Implementation/BufferViewNull_inl.h>
