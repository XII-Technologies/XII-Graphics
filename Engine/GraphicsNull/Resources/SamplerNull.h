#pragma once

#include <GraphicsNull/GraphicsNullDLL.h>

#include <GraphicsFoundation/Resources/Sampler.h>

class XII_GRAPHICSNULL_DLL xiiGALSamplerNull final : public xiiGALSampler
{
public:
protected:
  friend class xiiGALDeviceNull;
  friend class xiiMemoryUtils;

  xiiGALSamplerNull(xiiGALDeviceNull* pDeviceNull, const xiiGALSamplerCreationDescription& creationDescription);

  virtual ~xiiGALSamplerNull();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;
};

#include <GraphicsNull/Resources/Implementation/SamplerNull_inl.h>
