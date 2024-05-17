#pragma once

#include <GraphicsD3D11/GraphicsD3D11DLL.h>

#include <GraphicsFoundation/Resources/Texture.h>

struct ID3D11Resource;
struct D3D11_SUBRESOURCE_DATA;

XII_DEFINE_AS_POD_TYPE(D3D11_SUBRESOURCE_DATA);

class XII_GRAPHICSD3D11_DLL xiiGALTextureD3D11 final : public xiiGALTexture
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALTextureD3D11, xiiGALTexture);

public:
  virtual void SetState(xiiBitflags<xiiGALResourceStateFlags> stateFlags) override final;

  virtual xiiBitflags<xiiGALResourceStateFlags> GetState() const override final;

  virtual const xiiGALSparseTextureProperties& GetSparseProperties() const override final;

  ID3D11Resource* GetTexture() const;

protected:
  friend class xiiGALDeviceD3D11;
  friend class xiiMemoryUtils;

  xiiGALTextureD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALTextureCreationDescription& creationDescription);

  virtual ~xiiGALTextureD3D11();

  virtual xiiResult InitPlatform(const xiiGALTextureData* pInitialData) override final;

  virtual xiiResult DeInitPlatform() override final;

  virtual void SetDebugNamePlatform(xiiStringView sName) override final;

  xiiResult CreateFromNativeObject(void* pNativeObject);
  xiiResult CreateTexture1D(ID3D11Texture1D** ppTexture1D, const xiiGALTextureData* pInitialData);
  xiiResult CreateTexture2D(ID3D11Texture2D** ppTexture2D, const xiiGALTextureData* pInitialData);
  xiiResult CreateTexture3D(ID3D11Texture3D** ppTexture3D, const xiiGALTextureData* pInitialData);

  static void PrepareInitialData(const xiiGALTextureCreationDescription& description, const xiiGALTextureData* pInitialData, xiiHybridArray<D3D11_SUBRESOURCE_DATA, 16>& out_InitialData);

  void InitializeSparseTextureProperties();

protected:
  ID3D11Resource*               m_pTexture = nullptr;
  xiiGALSparseTextureProperties m_SparseTextureProperties;
};

#include <GraphicsD3D11/Resources/Implementation/TextureD3D11_inl.h>
