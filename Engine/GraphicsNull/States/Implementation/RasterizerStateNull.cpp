#include <GraphicsNull/GraphicsNullPCH.h>

#include <GraphicsNull/Device/DeviceNull.h>
#include <GraphicsNull/States/RasterizerStateNull.h>

xiiGALRasterizerStateNull::xiiGALRasterizerStateNull(xiiGALDeviceNull* pDeviceNull, const xiiGALRasterizerStateCreationDescription& creationDescription) :
  xiiGALRasterizerState(pDeviceNull, creationDescription)
{
}

xiiGALRasterizerStateNull::~xiiGALRasterizerStateNull() = default;

xiiResult xiiGALRasterizerStateNull::InitPlatform()
{
  return XII_SUCCESS;
}

xiiResult xiiGALRasterizerStateNull::DeInitPlatform()
{
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_States_Implementation_RasterizerStateNull);
