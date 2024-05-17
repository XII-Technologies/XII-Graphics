#pragma once

#include <GraphicsNull/GraphicsNullDLL.h>

#include <GraphicsFoundation/Resources/Texture.h>

class XII_GRAPHICSNULL_DLL xiiGALTextureNull final : public xiiGALTexture
{
public:
  virtual void SetState(xiiBitflags<xiiGALResourceStateFlags> stateFlags) override final;

  virtual xiiBitflags<xiiGALResourceStateFlags> GetState() const override final;

  virtual const xiiGALSparseTextureProperties& GetSparseProperties() const override final;

protected:
  friend class xiiGALDeviceNull;
  friend class xiiMemoryUtils;

  xiiGALTextureNull(xiiGALDeviceNull* pDeviceNull, const xiiGALTextureCreationDescription& creationDescription);

  virtual ~xiiGALTextureNull();

  virtual xiiResult InitPlatform(const xiiGALTextureData* pInitialData) override final;

  virtual xiiResult DeInitPlatform() override final;
};

#include <GraphicsNull/Resources/Implementation/TextureNull_inl.h>
