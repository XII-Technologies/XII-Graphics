#include <GraphicsNull/GraphicsNullPCH.h>

#include <GraphicsNull/Device/DeviceNull.h>
#include <GraphicsNull/Resources/FramebufferNull.h>
#include <GraphicsNull/Resources/RenderPassNull.h>
#include <GraphicsNull/Resources/TextureNull.h>
#include <GraphicsNull/Resources/TextureViewNull.h>

xiiGALFramebufferNull::xiiGALFramebufferNull(xiiGALDeviceNull* pDeviceNull, const xiiGALFramebufferCreationDescription& creationDescription) :
  xiiGALFramebuffer(pDeviceNull, creationDescription)
{
}

xiiGALFramebufferNull::~xiiGALFramebufferNull() = default;

xiiResult xiiGALFramebufferNull::InitPlatform()
{
  return XII_SUCCESS;
}

xiiResult xiiGALFramebufferNull::DeInitPlatform()
{
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_Resources_Implementation_FramebufferNull);
