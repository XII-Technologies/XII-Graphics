#include <GraphicsNull/GraphicsNullPCH.h>

#include <GraphicsNull/Device/DeviceNull.h>
#include <GraphicsNull/States/BlendStateNull.h>

xiiGALBlendStateNull::xiiGALBlendStateNull(xiiGALDeviceNull* pDeviceNull, const xiiGALBlendStateCreationDescription& creationDescription) :
  xiiGALBlendState(pDeviceNull, creationDescription)
{
}

xiiGALBlendStateNull::~xiiGALBlendStateNull() = default;

xiiResult xiiGALBlendStateNull::InitPlatform()
{
  return XII_SUCCESS;
}

xiiResult xiiGALBlendStateNull::DeInitPlatform()
{
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_States_Implementation_BlendStateNull);
