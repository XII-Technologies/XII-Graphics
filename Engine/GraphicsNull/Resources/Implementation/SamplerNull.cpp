#include <GraphicsNull/GraphicsNullPCH.h>

#include <GraphicsNull/Device/DeviceNull.h>
#include <GraphicsNull/Resources/SamplerNull.h>

xiiGALSamplerNull::xiiGALSamplerNull(xiiGALDeviceNull* pDeviceNull, const xiiGALSamplerCreationDescription& creationDescription) :
  xiiGALSampler(pDeviceNull, creationDescription)
{
}

xiiGALSamplerNull::~xiiGALSamplerNull() = default;

xiiResult xiiGALSamplerNull::InitPlatform()
{
  return XII_SUCCESS;
}

xiiResult xiiGALSamplerNull::DeInitPlatform()
{
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_Resources_Implementation_SamplerNull);
