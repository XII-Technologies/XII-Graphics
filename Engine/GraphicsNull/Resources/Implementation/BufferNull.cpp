#include <GraphicsNull/GraphicsNullPCH.h>

#include <GraphicsNull/Device/DeviceNull.h>
#include <GraphicsNull/Resources/BufferNull.h>

xiiGALBufferNull::xiiGALBufferNull(xiiGALDeviceNull* pDeviceNull, const xiiGALBufferCreationDescription& creationDescription) :
  xiiGALBuffer(pDeviceNull, creationDescription)
{
}

xiiGALBufferNull::~xiiGALBufferNull() = default;

xiiResult xiiGALBufferNull::InitPlatform(const xiiGALBufferData* pInitialData)
{
  return XII_SUCCESS;
}

xiiResult xiiGALBufferNull::DeInitPlatform()
{
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_Resources_Implementation_BufferNull);
