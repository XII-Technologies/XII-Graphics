#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <GraphicsD3D11/Device/DeviceD3D11.h>
#include <GraphicsD3D11/Resources/FramebufferD3D11.h>
#include <GraphicsD3D11/Resources/RenderPassD3D11.h>
#include <GraphicsD3D11/Resources/TextureD3D11.h>
#include <GraphicsD3D11/Resources/TextureViewD3D11.h>

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALFramebufferD3D11, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALFramebufferD3D11::xiiGALFramebufferD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALFramebufferCreationDescription& creationDescription) :
  xiiGALFramebuffer(pDeviceD3D11, creationDescription)
{
}

xiiGALFramebufferD3D11::~xiiGALFramebufferD3D11() = default;

xiiResult xiiGALFramebufferD3D11::InitPlatform()
{
  return XII_SUCCESS;
}

xiiResult xiiGALFramebufferD3D11::DeInitPlatform()
{
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_Resources_Implementation_FramebufferD3D11);
