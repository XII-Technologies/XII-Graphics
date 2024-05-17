#include <GraphicsNull/GraphicsNullPCH.h>

#include <GraphicsNull/Device/DeviceNull.h>
#include <GraphicsNull/Resources/BufferNull.h>
#include <GraphicsNull/Resources/BufferViewNull.h>

xiiGALBufferViewNull::xiiGALBufferViewNull(xiiGALDeviceNull* pDeviceNull, xiiGALBuffer* pBuffer, const xiiGALBufferViewCreationDescription& creationDescription) :
  xiiGALBufferView(pDeviceNull, pBuffer, creationDescription)
{
}

xiiGALBufferViewNull::~xiiGALBufferViewNull() = default;

xiiResult xiiGALBufferViewNull::InitPlatform()
{
  return XII_SUCCESS;
}

xiiResult xiiGALBufferViewNull::DeInitPlatform()
{
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_Resources_Implementation_BufferViewNull);
