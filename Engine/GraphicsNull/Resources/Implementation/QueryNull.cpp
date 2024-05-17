#include <GraphicsNull/GraphicsNullPCH.h>

#include <GraphicsNull/Device/DeviceNull.h>
#include <GraphicsNull/Resources/QueryNull.h>

xiiGALQueryNull::xiiGALQueryNull(xiiGALDeviceNull* pDeviceNull, const xiiGALQueryCreationDescription& creationDescription) :
  xiiGALQuery(pDeviceNull, creationDescription)
{
}

xiiGALQueryNull::~xiiGALQueryNull() = default;

xiiResult xiiGALQueryNull::InitPlatform()
{
  return XII_SUCCESS;
}

xiiResult xiiGALQueryNull::DeInitPlatform()
{
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_Resources_Implementation_QueryNull);
