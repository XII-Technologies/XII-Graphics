#include <GraphicsNull/GraphicsNullPCH.h>

#include <GraphicsNull/Device/DeviceNull.h>
#include <GraphicsNull/Resources/TextureNull.h>

xiiGALTextureNull::xiiGALTextureNull(xiiGALDeviceNull* pDeviceNull, const xiiGALTextureCreationDescription& creationDescription) :
  xiiGALTexture(pDeviceNull, creationDescription)
{
}

xiiGALTextureNull::~xiiGALTextureNull() = default;

xiiResult xiiGALTextureNull::InitPlatform(const xiiGALTextureData* pInitialData)
{
  return XII_SUCCESS;
}

xiiResult xiiGALTextureNull::DeInitPlatform()
{
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_Resources_Implementation_TextureNull);
