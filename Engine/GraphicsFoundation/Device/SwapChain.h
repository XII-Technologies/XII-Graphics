#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Declarations/Descriptors.h>
#include <GraphicsFoundation/Declarations/DeviceObject.h>

/// \brief Interface that defines methods to manipulate a swap chain object.
class XII_GRAPHICSFOUNDATION_DLL xiiGALSwapChain : public xiiGALDeviceObject
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALSwapChain, xiiGALDeviceObject);

public:
  /// \brief This returns the creation description for this object.
  XII_NODISCARD const xiiGALSwapChainCreationDescription& GetDescription() const;

  /// \brief Acquires the next render target for presenting.
  virtual void AcquireNextRenderTarget() = 0;

  /// \brief This presents a rendered image to the screen.
  virtual void Present() = 0;

  /// \brief This changes the swap chain size.
  ///
  /// \param newSize      - The new logical swap chain width and height (not accounting for the pre-transform), in pixels.
  /// \param newTransform - newTransform The new surface transform.
  ///
  /// \note When resizing non-primary swap chains, the engine unbinds the swap chain buffers from the output.
  virtual xiiResult Resize(xiiSizeU32 newSize, xiiEnum<xiiGALSurfaceTransform> newTransform = xiiGALSurfaceTransform::Optimal) = 0;

  /// \brief This sets the swap chain to full screen mode. Note that this is only supported on the Windows platform.
  virtual void SetFullScreenMode(const xiiGALDisplayModeDescription& displayMode) = 0;

  /// \brief This sets the swap chain to windowed mode. Note that this is only supported on the Windows platform.
  virtual void SetWindowedMode() = 0;

  /// \brief This sets the maximum number of frames that the swap chain is allowed to queue for rendering.
  ///
  /// This value is only relevant for DirectX11 and DirectX12 backends and ignored for others. By default it matches the number of buffers in the swap chain. For example, for a 2-buffer
  /// swap chain, the CPU can enqueue frames 0 and 1, but Present command of frame 2 will block until frame 0 is presented. If in the example above the maximum frame latency is set
  /// to 1, then Present command of frame 1 will block until Present of frame 0 is complete.
  virtual void SetMaximumFrameLatency(xiiUInt32 uiMaxLatency);

  void SetPresentMode(xiiEnum<xiiGALPresentMode> presentMode);

  XII_NODISCARD xiiGALTextureHandle GetBackBufferTexture() const;

  XII_NODISCARD xiiSizeU32 GetCurrentSize() const;

  XII_NODISCARD xiiEnum<xiiGALPresentMode> GetPresentMode() const;

protected:
  friend class xiiGALDevice;
  friend class xiiMemoryUtils;

  xiiGALSwapChain(xiiGALDevice* pDevice, const xiiGALSwapChainCreationDescription& creationDescription);

  virtual ~xiiGALSwapChain();

  virtual xiiResult InitPlatform() = 0;

  virtual xiiResult DeInitPlatform() = 0;

  xiiGALTextureHandle m_hBackBufferTexture;

  xiiEnum<xiiGALPresentMode> m_PresentMode = xiiGALPresentMode::VSync;

  xiiGALSwapChainCreationDescription m_Description;

  xiiEnum<xiiGALSurfaceTransform> m_DesiredSurfaceTransform = xiiGALSurfaceTransform::Optimal;
};

#include <GraphicsFoundation/Device/Implementation/SwapChain_inl.h>
