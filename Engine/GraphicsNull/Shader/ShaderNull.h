#pragma once

#include <GraphicsNull/GraphicsNullDLL.h>

#include <GraphicsFoundation/Shader/InputLayout.h>
#include <GraphicsFoundation/Shader/Shader.h>

class XII_GRAPHICSNULL_DLL xiiGALShaderNull final : public xiiGALShader
{
public:
protected:
  friend class xiiGALDeviceNull;
  friend class xiiMemoryUtils;

  xiiGALShaderNull(xiiGALDeviceNull* pDeviceNull, const xiiGALShaderCreationDescription& creationDescription);

  virtual ~xiiGALShaderNull();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;
};

#include <GraphicsNull/Shader/Implementation/ShaderNull_inl.h>
