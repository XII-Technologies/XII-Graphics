#pragma once

#include <GraphicsNull/GraphicsNullDLL.h>

#include <GraphicsFoundation/Shader/InputLayout.h>

class XII_GRAPHICSNULL_DLL xiiGALInputLayoutNull final : public xiiGALInputLayout
{
public:
protected:
  friend class xiiGALDeviceNull;
  friend class xiiMemoryUtils;

  xiiGALInputLayoutNull(xiiGALDeviceNull* pDeviceNull, const xiiGALInputLayoutCreationDescription& creationDescription);

  virtual ~xiiGALInputLayoutNull();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;
};

#include <GraphicsNull/Shader/Implementation/InputLayoutNull_inl.h>
