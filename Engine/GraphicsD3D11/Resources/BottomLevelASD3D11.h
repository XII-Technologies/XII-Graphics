#pragma once

#include <GraphicsD3D11/GraphicsD3D11DLL.h>

#include <GraphicsFoundation/Resources/BottomLevelAS.h>

class XII_GRAPHICSD3D11_DLL xiiGALBottomLevelASD3D11 final : public xiiGALBottomLevelAS
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALBottomLevelASD3D11, xiiGALBottomLevelAS);

public:
  virtual xiiUInt32 GetGeometryDescriptionIndex(xiiStringView sName) const override final;

  virtual xiiUInt32 GetGeometryIndex(xiiStringView sName) const override final;

  virtual xiiUInt32 GetActualGeometryCount() const override final;

  virtual xiiGALScratchBufferSizeDescription GetScratchBufferSizeDescription() const override final;

  virtual void SetState(xiiBitflags<xiiGALResourceStateFlags> stateFlags) override final;

  virtual xiiBitflags<xiiGALResourceStateFlags> GetState() const override final;

protected:
  friend class xiiGALDeviceD3D11;
  friend class xiiMemoryUtils;

  xiiGALBottomLevelASD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALBottomLevelASCreationDescription& creationDescription);

  virtual ~xiiGALBottomLevelASD3D11();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;
};

#include <GraphicsD3D11/Resources/Implementation/BottomLevelASD3D11_inl.h>
