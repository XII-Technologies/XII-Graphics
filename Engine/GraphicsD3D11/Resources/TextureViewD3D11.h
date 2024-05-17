#pragma once

#include <GraphicsD3D11/GraphicsD3D11DLL.h>

#include <GraphicsFoundation/Resources/TextureView.h>

struct ID3D11View;

class XII_GRAPHICSD3D11_DLL xiiGALTextureViewD3D11 final : public xiiGALTextureView
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALTextureViewD3D11, xiiGALTextureView);

public:
  ID3D11View* GetTextureView() const;

protected:
  friend class xiiGALDeviceD3D11;
  friend class xiiMemoryUtils;

  xiiGALTextureViewD3D11(xiiGALDeviceD3D11* pDeviceD3D11, xiiGALTexture* pTexture, const xiiGALTextureViewCreationDescription& creationDescription);

  virtual ~xiiGALTextureViewD3D11();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;

  virtual void SetDebugNamePlatform(xiiStringView sName) override final;

  xiiResult CreateSRV(ID3D11ShaderResourceView** ppShaderResourceView);
  xiiResult CreateRTV(ID3D11RenderTargetView** ppRenderTargetView);
  xiiResult CreateDSV(ID3D11DepthStencilView** ppDepthStencilView);
  xiiResult CreateUAV(ID3D11UnorderedAccessView** ppUnorderedAccessView);

protected:
  ID3D11View* m_pTextureView = nullptr;
};

#include <GraphicsD3D11/Resources/Implementation/TextureViewD3D11_inl.h>
