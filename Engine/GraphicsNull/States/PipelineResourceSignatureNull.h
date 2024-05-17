#pragma once

#include <GraphicsNull/GraphicsNullDLL.h>

#include <GraphicsFoundation/States/PipelineResourceSignature.h>

class XII_GRAPHICSNULL_DLL xiiGALPipelineResourceSignatureNull final : public xiiGALPipelineResourceSignature
{
public:
  virtual bool IsCompatibleWith(const xiiGALPipelineResourceSignature* pPipelineResourceSignature) const override final;

protected:
  friend class xiiGALDeviceNull;
  friend class xiiMemoryUtils;

  xiiGALPipelineResourceSignatureNull(xiiGALDeviceNull* pDeviceNull, const xiiGALPipelineResourceSignatureCreationDescription& creationDescription);

  virtual ~xiiGALPipelineResourceSignatureNull();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;
};

#include <GraphicsNull/States/Implementation/PipelineResourceSignatureNull_inl.h>
