#include <GraphicsNull/GraphicsNullPCH.h>

#include <GraphicsNull/Device/DeviceNull.h>
#include <GraphicsNull/Resources/TopLevelASNull.h>

xiiGALTopLevelASNull::xiiGALTopLevelASNull(xiiGALDeviceNull* pDeviceNull, const xiiGALTopLevelASCreationDescription& creationDescription) :
  xiiGALTopLevelAS(pDeviceNull, creationDescription)
{
}

xiiGALTopLevelASNull::~xiiGALTopLevelASNull() = default;

xiiResult xiiGALTopLevelASNull::InitPlatform()
{
  return XII_SUCCESS;
}

xiiResult xiiGALTopLevelASNull::DeInitPlatform()
{
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_Resources_Implementation_TopLevelASNull);
