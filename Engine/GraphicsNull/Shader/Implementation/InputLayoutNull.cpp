#include <GraphicsNull/GraphicsNullPCH.h>

#include <GraphicsNull/Device/DeviceNull.h>
#include <GraphicsNull/Shader/InputLayoutNull.h>
#include <GraphicsNull/Shader/ShaderNull.h>

xiiGALInputLayoutNull::xiiGALInputLayoutNull(xiiGALDeviceNull* pDeviceNull, const xiiGALInputLayoutCreationDescription& creationDescription) :
  xiiGALInputLayout(pDeviceNull, creationDescription)
{
}

xiiGALInputLayoutNull::~xiiGALInputLayoutNull() = default;

xiiResult xiiGALInputLayoutNull::InitPlatform()
{
  return XII_SUCCESS;
}

xiiResult xiiGALInputLayoutNull::DeInitPlatform()
{
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_Shader_Implementation_InputLayoutNull);
