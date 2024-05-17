#pragma once

#include <GraphicsNull/GraphicsNullDLL.h>

#include <GraphicsFoundation/Resources/RenderPass.h>

class XII_GRAPHICSNULL_DLL xiiGALRenderPassNull final : public xiiGALRenderPass
{
public:
protected:
  friend class xiiGALDeviceNull;
  friend class xiiMemoryUtils;

  xiiGALRenderPassNull(xiiGALDeviceNull* pDeviceNull, const xiiGALRenderPassCreationDescription& creationDescription);

  virtual ~xiiGALRenderPassNull();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;
};

#include <GraphicsNull/Resources/Implementation/RenderPassNull_inl.h>
