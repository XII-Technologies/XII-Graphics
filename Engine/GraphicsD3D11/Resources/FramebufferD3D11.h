#pragma once

#include <GraphicsD3D11/GraphicsD3D11DLL.h>

#include <GraphicsFoundation/Resources/Framebuffer.h>

class XII_GRAPHICSD3D11_DLL xiiGALFramebufferD3D11 final : public xiiGALFramebuffer
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALFramebufferD3D11, xiiGALFramebuffer);

protected:
  friend class xiiGALDeviceD3D11;
  friend class xiiMemoryUtils;

  xiiGALFramebufferD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALFramebufferCreationDescription& creationDescription);

  virtual ~xiiGALFramebufferD3D11();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;
};

#include <GraphicsD3D11/Resources/Implementation/FramebufferD3D11_inl.h>
