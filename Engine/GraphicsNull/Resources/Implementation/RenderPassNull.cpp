#include <GraphicsNull/GraphicsNullPCH.h>

#include <GraphicsNull/Device/DeviceNull.h>
#include <GraphicsNull/Resources/RenderPassNull.h>

xiiGALRenderPassNull::xiiGALRenderPassNull(xiiGALDeviceNull* pDeviceNull, const xiiGALRenderPassCreationDescription& creationDescription) :
  xiiGALRenderPass(pDeviceNull, creationDescription)
{
}

xiiGALRenderPassNull::~xiiGALRenderPassNull() = default;

xiiResult xiiGALRenderPassNull::InitPlatform()
{
  return XII_SUCCESS;
}

xiiResult xiiGALRenderPassNull::DeInitPlatform()
{
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_Resources_Implementation_RenderPassNull);
