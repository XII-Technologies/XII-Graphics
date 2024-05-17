#include <GraphicsNull/GraphicsNullPCH.h>

#include <GraphicsNull/Device/DeviceNull.h>
#include <GraphicsNull/States/DepthStencilStateNull.h>

xiiGALDepthStencilStateNull::xiiGALDepthStencilStateNull(xiiGALDeviceNull* pDeviceNull, const xiiGALDepthStencilStateCreationDescription& creationDescription) :
  xiiGALDepthStencilState(pDeviceNull, creationDescription)
{
}

xiiGALDepthStencilStateNull::~xiiGALDepthStencilStateNull() = default;

xiiResult xiiGALDepthStencilStateNull::InitPlatform()
{
  return XII_SUCCESS;
}

xiiResult xiiGALDepthStencilStateNull::DeInitPlatform()
{
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_States_Implementation_DepthStencilStateNull);
