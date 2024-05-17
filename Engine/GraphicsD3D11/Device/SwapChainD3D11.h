#pragma once

#include <GraphicsD3D11/GraphicsD3D11DLL.h>

#include <GraphicsFoundation/Device/SwapChain.h>

struct IDXGISwapChain4;

class XII_GRAPHICSD3D11_DLL xiiGALSwapChainD3D11 final : public xiiGALSwapChain
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALSwapChainD3D11, xiiGALSwapChain);

public:
  virtual void AcquireNextRenderTarget() override final;

  virtual void Present() override final;

  virtual xiiResult Resize(xiiSizeU32 newSize, xiiEnum<xiiGALSurfaceTransform> newTransform = xiiGALSurfaceTransform::Optimal) override final;

  virtual void SetFullScreenMode(const xiiGALDisplayModeDescription& displayMode) override final;

  virtual void SetWindowedMode() override final;

  virtual void SetMaximumFrameLatency(xiiUInt32 uiMaxLatency) override final;

  IDXGISwapChain4* GetSwapChain() const;

protected:
  friend class xiiGALDeviceD3D11;
  friend class xiiMemoryUtils;

  xiiGALSwapChainD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALSwapChainCreationDescription& creationDescription);

  virtual ~xiiGALSwapChainD3D11();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;

  virtual void SetDebugNamePlatform(xiiStringView sName) override final;

  xiiResult CreateDXGISwapChain();

  xiiResult UpdateSwapChain(bool bCreateNew);

  xiiResult CreateBackBufferInternal(xiiGALDeviceD3D11* pDeviceD3D11);

  void DestroyBackBufferInternal(xiiGALDeviceD3D11* pDeviceD3D11);

protected:
  IDXGISwapChain4* m_pSwapChain = nullptr;

  xiiGALTextureHandle m_hActualBackBufferTexture;

  xiiGALFullScreenModeDescription m_FullScreenMode;

  xiiUInt32 m_uiMaximumFrameLatency = 0U;
};

#include <GraphicsD3D11/Device/Implementation/SwapChainD3D11_inl.h>
