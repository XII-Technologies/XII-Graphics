#include <GraphicsNull/GraphicsNullPCH.h>

#include <GraphicsNull/Device/DeviceNull.h>
#include <GraphicsNull/Shader/ShaderNull.h>

xiiGALShaderNull::xiiGALShaderNull(xiiGALDeviceNull* pDeviceNull, const xiiGALShaderCreationDescription& creationDescription) :
  xiiGALShader(pDeviceNull, creationDescription)
{
}

xiiGALShaderNull::~xiiGALShaderNull() = default;

xiiResult xiiGALShaderNull::InitPlatform()
{
  return XII_SUCCESS;
}

xiiResult xiiGALShaderNull::DeInitPlatform()
{
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_Shader_Implementation_ShaderNull);
