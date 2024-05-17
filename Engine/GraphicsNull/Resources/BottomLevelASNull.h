#pragma once

#include <GraphicsNull/GraphicsNullDLL.h>

#include <GraphicsFoundation/Resources/BottomLevelAS.h>

class XII_GRAPHICSNULL_DLL xiiGALBottomLevelASNull final : public xiiGALBottomLevelAS
{
public:
  virtual xiiUInt32 GetGeometryDescriptionIndex(xiiStringView sName) const override final;

  virtual xiiUInt32 GetGeometryIndex(xiiStringView sName) const override final;

  virtual xiiUInt32 GetActualGeometryCount() const override final;

  virtual xiiGALScratchBufferSizeDescription GetScratchBufferSizeDescription() const override final;

  virtual void SetState(xiiBitflags<xiiGALResourceStateFlags> stateFlags) override final;

  virtual xiiBitflags<xiiGALResourceStateFlags> GetState() const override final;

protected:
  friend class xiiGALDeviceNull;
  friend class xiiMemoryUtils;

  xiiGALBottomLevelASNull(xiiGALDeviceNull* pDeviceNull, const xiiGALBottomLevelASCreationDescription& creationDescription);

  virtual ~xiiGALBottomLevelASNull();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;
};

#include <GraphicsNull/Resources/Implementation/BottomLevelASNull_inl.h>
