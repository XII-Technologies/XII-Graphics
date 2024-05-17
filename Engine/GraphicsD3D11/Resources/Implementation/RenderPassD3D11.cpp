#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <GraphicsD3D11/Device/DeviceD3D11.h>
#include <GraphicsD3D11/Resources/RenderPassD3D11.h>

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALRenderPassD3D11, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALRenderPassD3D11::xiiGALRenderPassD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALRenderPassCreationDescription& creationDescription) :
  xiiGALRenderPass(pDeviceD3D11, creationDescription)
{
}

xiiGALRenderPassD3D11::~xiiGALRenderPassD3D11() = default;

xiiResult xiiGALRenderPassD3D11::InitPlatform()
{
  return XII_SUCCESS;
}

xiiResult xiiGALRenderPassD3D11::DeInitPlatform()
{
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_Resources_Implementation_RenderPassD3D11);
