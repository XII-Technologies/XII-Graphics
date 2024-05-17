#include <GraphicsNull/GraphicsNullPCH.h>

#include <GraphicsNull/Device/DeviceNull.h>
#include <GraphicsNull/Resources/TextureNull.h>
#include <GraphicsNull/Resources/TextureViewNull.h>

xiiGALTextureViewNull::xiiGALTextureViewNull(xiiGALDeviceNull* pDeviceNull, xiiGALTexture* pTexture, const xiiGALTextureViewCreationDescription& creationDescription) :
  xiiGALTextureView(pDeviceNull, pTexture, creationDescription)
{
}

xiiGALTextureViewNull::~xiiGALTextureViewNull() = default;

xiiResult xiiGALTextureViewNull::InitPlatform()
{
  return XII_SUCCESS;
}

xiiResult xiiGALTextureViewNull::DeInitPlatform()
{
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_Resources_Implementation_TextureViewNull);
