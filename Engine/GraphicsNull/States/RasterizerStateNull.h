#pragma once

#include <GraphicsNull/GraphicsNullDLL.h>

#include <GraphicsFoundation/States/RasterizerState.h>

class XII_GRAPHICSNULL_DLL xiiGALRasterizerStateNull final : public xiiGALRasterizerState
{
public:
protected:
  friend class xiiGALDeviceNull;
  friend class xiiMemoryUtils;

  xiiGALRasterizerStateNull(xiiGALDeviceNull* pDeviceNull, const xiiGALRasterizerStateCreationDescription& creationDescription);

  virtual ~xiiGALRasterizerStateNull();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;
};

#include <GraphicsNull/States/Implementation/RasterizerStateNull_inl.h>
