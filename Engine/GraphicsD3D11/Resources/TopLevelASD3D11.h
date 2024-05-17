#pragma once

#include <GraphicsD3D11/GraphicsD3D11DLL.h>

#include <GraphicsFoundation/Resources/TopLevelAS.h>

class XII_GRAPHICSD3D11_DLL xiiGALTopLevelASD3D11 final : public xiiGALTopLevelAS
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALTopLevelASD3D11, xiiGALTopLevelAS);

public:
  virtual xiiGALTopLevelASInstanceDescription GetInstanceDescription(xiiStringView sName) const override final;

  virtual xiiGALTopLevelASBuildDescription GetBuildDescription() const override final;

  virtual xiiGALScratchBufferSizeDescription GetScratchBufferSizeDescription() const override final;

  virtual void SetState(xiiBitflags<xiiGALResourceStateFlags> stateFlags) override final;

  virtual xiiBitflags<xiiGALResourceStateFlags> GetState() const override final;

protected:
  friend class xiiGALDeviceD3D11;
  friend class xiiMemoryUtils;

  xiiGALTopLevelASD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALTopLevelASCreationDescription& creationDescription);

  virtual ~xiiGALTopLevelASD3D11();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;
};

#include <GraphicsD3D11/Resources/Implementation/TopLevelASD3D11_inl.h>
