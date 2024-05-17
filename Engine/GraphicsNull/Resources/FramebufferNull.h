#pragma once

#include <GraphicsNull/GraphicsNullDLL.h>

#include <GraphicsFoundation/Resources/Framebuffer.h>

class XII_GRAPHICSNULL_DLL xiiGALFramebufferNull final : public xiiGALFramebuffer
{
public:
protected:
  friend class xiiGALDeviceNull;
  friend class xiiMemoryUtils;

  xiiGALFramebufferNull(xiiGALDeviceNull* pDeviceNull, const xiiGALFramebufferCreationDescription& creationDescription);

  virtual ~xiiGALFramebufferNull();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;
};

#include <GraphicsNull/Resources/Implementation/FramebufferNull_inl.h>
