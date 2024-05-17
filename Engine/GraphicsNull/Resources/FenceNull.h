#pragma once

#include <GraphicsNull/GraphicsNullDLL.h>

#include <GraphicsFoundation/Resources/Fence.h>

class XII_GRAPHICSNULL_DLL xiiGALFenceNull final : public xiiGALFence
{
public:
  virtual xiiUInt64 GetCompletedValue() override final;

  virtual void Signal(xiiUInt64 uiValue) override final;

  virtual void Wait(xiiUInt64 uiValue) override final;

protected:
  friend class xiiGALDeviceNull;
  friend class xiiMemoryUtils;

  xiiGALFenceNull(xiiGALDeviceNull* pDeviceNull, const xiiGALFenceCreationDescription& creationDescription);

  virtual ~xiiGALFenceNull();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;
};

#include <GraphicsNull/Resources/Implementation/FenceNull_inl.h>
