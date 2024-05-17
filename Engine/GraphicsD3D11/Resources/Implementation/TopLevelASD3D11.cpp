#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <GraphicsD3D11/Device/DeviceD3D11.h>
#include <GraphicsD3D11/Resources/TopLevelASD3D11.h>

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALTopLevelASD3D11, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALTopLevelASD3D11::xiiGALTopLevelASD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALTopLevelASCreationDescription& creationDescription) :
  xiiGALTopLevelAS(pDeviceD3D11, creationDescription)
{
}

xiiGALTopLevelASD3D11::~xiiGALTopLevelASD3D11() = default;

xiiResult xiiGALTopLevelASD3D11::InitPlatform()
{
  xiiLog::Error("xiiGALTopLevelAS resource is unsupported in Direct3D11.");

  return XII_FAILURE;
}

xiiResult xiiGALTopLevelASD3D11::DeInitPlatform()
{
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_Resources_Implementation_TopLevelASD3D11);
