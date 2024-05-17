#include <GraphicsNull/GraphicsNullPCH.h>

#include <GraphicsNull/Device/DeviceNull.h>
#include <GraphicsNull/Resources/FenceNull.h>

xiiGALFenceNull::xiiGALFenceNull(xiiGALDeviceNull* pDeviceNull, const xiiGALFenceCreationDescription& creationDescription) :
  xiiGALFence(pDeviceNull, creationDescription)
{
}

xiiGALFenceNull::~xiiGALFenceNull() = default;

xiiResult xiiGALFenceNull::InitPlatform()
{
  return XII_SUCCESS;
}

xiiResult xiiGALFenceNull::DeInitPlatform()
{
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_Resources_Implementation_FenceNull);
