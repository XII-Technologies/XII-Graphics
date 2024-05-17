#pragma once

#include <GraphicsNull/GraphicsNullDLL.h>

#include <GraphicsFoundation/States/BlendState.h>

class XII_GRAPHICSNULL_DLL xiiGALBlendStateNull final : public xiiGALBlendState
{
public:
protected:
  friend class xiiGALDeviceNull;
  friend class xiiMemoryUtils;

  xiiGALBlendStateNull(xiiGALDeviceNull* pDeviceNull, const xiiGALBlendStateCreationDescription& creationDescription);

  virtual ~xiiGALBlendStateNull();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;
};

#include <GraphicsNull/States/Implementation/BlendStateNull_inl.h>
