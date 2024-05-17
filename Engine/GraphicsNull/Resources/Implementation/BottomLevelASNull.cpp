#include <GraphicsNull/GraphicsNullPCH.h>

#include <GraphicsNull/Device/DeviceNull.h>
#include <GraphicsNull/Resources/BottomLevelASNull.h>

xiiGALBottomLevelASNull::xiiGALBottomLevelASNull(xiiGALDeviceNull* pDeviceNull, const xiiGALBottomLevelASCreationDescription& creationDescription) :
  xiiGALBottomLevelAS(pDeviceNull, creationDescription)
{
}

xiiGALBottomLevelASNull::~xiiGALBottomLevelASNull() = default;

xiiResult xiiGALBottomLevelASNull::InitPlatform()
{
  return XII_SUCCESS;
}

xiiResult xiiGALBottomLevelASNull::DeInitPlatform()
{
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_Resources_Implementation_BottomLevelASNull);
