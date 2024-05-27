#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <Core/System/Window.h>
#include <Foundation/Profiling/Profiling.h>
#include <GraphicsD3D11/CommandEncoder/CommandListD3D11.h>
#include <GraphicsD3D11/CommandEncoder/CommandQueueD3D11.h>
#include <GraphicsD3D11/Device/DeviceD3D11.h>
#include <GraphicsD3D11/Device/SwapChainD3D11.h>
#include <GraphicsD3D11/Resources/TextureD3D11.h>
#include <GraphicsD3D11/Resources/TextureViewD3D11.h>

#include <Foundation/Basics/Platform/Win/HResultUtils.h>

#include <VersionHelpers.h>
#include <dxgi1_4.h>

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALSwapChainD3D11, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALSwapChainD3D11::xiiGALSwapChainD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALSwapChainCreationDescription& creationDescription) :
  xiiGALSwapChain(pDeviceD3D11, creationDescription)
{
}

xiiGALSwapChainD3D11::~xiiGALSwapChainD3D11() = default;

xiiResult xiiGALSwapChainD3D11::InitPlatform()
{
  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);

  if (CreateDXGISwapChain().Failed())
    return XII_FAILURE;

  // We have created a surface on a window, the window must not be destroyed while the surface is still alive.
  m_Description.m_pWindow->AddReference();

  return CreateBackBufferInternal(pDeviceD3D11);
}

xiiResult xiiGALSwapChainD3D11::DeInitPlatform()
{
  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);

  // Reset command list swapchain references or ResizeBuffers will fail as the backbuffer is still referenced.
  pDeviceD3D11->ResetCommandQueuesSwapChainReferences();

  DestroyBackBufferInternal(pDeviceD3D11);

  // Call context flush to release resources.
  pDeviceD3D11->GetImmediateContext()->ClearState();
  pDeviceD3D11->GetImmediateContext()->Flush();

  if (m_pSwapChain)
  {
    // Full screen swap chains must be switched to windowed mode before destruction.
    // See: https://msdn.microsoft.com/en-us/library/windows/desktop/bb205075(v=vs.85).aspx#Destroying
    BOOL bIsFullScreen = FALSE;
    if (FAILED(m_pSwapChain->GetFullscreenState(&bIsFullScreen, nullptr)))
    {
      xiiLog::Error("Failed to query swap chain full screen state.");
    }
    if (bIsFullScreen == TRUE)
    {
      m_pSwapChain->SetFullscreenState(FALSE, nullptr);
    }

    XII_GAL_D3D11_RELEASE(m_pSwapChain);

    m_Description.m_pWindow->RemoveReference();
  }

  return XII_SUCCESS;
}

void xiiGALSwapChainD3D11::SetDebugNamePlatform(xiiStringView sName)
{
  if (m_pSwapChain != nullptr)
  {
    xiiStringBuilder sb;
    if (FAILED(m_pSwapChain->SetPrivateData(WKPDID_D3DDebugObjectName, sName.GetElementCount(), sName.GetData(sb))))
    {
      xiiLog::Error("Failed to set the Direct3D11 swap chain debug name.");
    }
  }
}

xiiResult xiiGALSwapChainD3D11::CreateDXGISwapChain()
{
  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);

  if (m_Description.m_PreTransform != xiiGALSurfaceTransform::Optimal && m_Description.m_PreTransform != xiiGALSurfaceTransform::Identity)
  {
    xiiLog::Warning("The current pre-transform is unsupported by Direct3D swap chains. Use xiiGALSurfaceTransform::Optimal (recommended) or xiiGALSurfaceTransform::Identity.");
  }
  m_DesiredSurfaceTransform    = xiiGALSurfaceTransform::Optimal;
  m_Description.m_PreTransform = xiiGALSurfaceTransform::Identity;

  m_uiMaximumFrameLatency = m_Description.m_uiBufferCount;

  HWND hNativeWindow = xiiMinWindows::ToNative(m_Description.m_pWindow->GetNativeWindowHandle());

#if XII_ENABLED(XII_PLATFORM_WINDOWS_DESKTOP)
  if (!m_Description.m_Resolution.HasNonZeroArea())
  {
    RECT rect;
    if (m_FullScreenMode.m_bIsFullScreen)
    {
      const HWND hDesktop = GetDesktopWindow();
      GetWindowRect(hDesktop, &rect);
    }
    else
    {
      GetClientRect(hNativeWindow, &rect);
    }
    m_Description.m_Resolution = xiiSizeU32(rect.right - rect.left, rect.bottom - rect.top);
  }
#endif

  DXGI_FORMAT dxgiColorBufferFormat = xiiD3D11TypeConversions::GetFormat(m_Description.m_ColorBufferFormat);

  DXGI_SWAP_CHAIN_DESC1 swapChainDescription = {};
  swapChainDescription.Width                 = m_Description.m_Resolution.width;
  swapChainDescription.Height                = m_Description.m_Resolution.height;
  swapChainDescription.Stereo                = FALSE;

  // Multi-sampled swap chains are not supported anymore. CreateSwapChainForHwnd() fails when sample count is not 1 for any swap effect.
  swapChainDescription.SampleDesc = {.Count = 1U, .Quality = 0U};

  // Flip model swapchains (DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL and DXGI_SWAP_EFFECT_FLIP_DISCARD) are more efficient, and only support the following Formats:
  // - DXGI_FORMAT_R16G16B16A16_FLOAT
  // - DXGI_FORMAT_B8G8R8A8_UNORM
  // - DXGI_FORMAT_R8G8B8A8_UNORM
  // - DXGI_FORMAT_R10G10B10A2_UNORM
  // So, we can't have sRGB swap chain formats (we can have non-srgb swap chain with srgb texture view).
  switch (dxgiColorBufferFormat)
  {
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
      swapChainDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
      break;

    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
      swapChainDescription.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      break;

    default:
      swapChainDescription.Format = dxgiColorBufferFormat;
      break;
  }

  XII_ASSERT_DEV(!m_Description.m_Usage.IsNoFlagSet(), "No swap chain usage flags are set!");

  if (m_Description.m_Usage.IsSet(xiiGALSwapChainUsageFlags::RenderTarget))
  {
    swapChainDescription.BufferUsage |= DXGI_USAGE_RENDER_TARGET_OUTPUT;
  }
  if (m_Description.m_Usage.IsAnySet(xiiGALSwapChainUsageFlags::ShaderResource | xiiGALSwapChainUsageFlags::InputAttachment))
  {
    swapChainDescription.BufferUsage |= DXGI_USAGE_SHADER_INPUT;
  }

  swapChainDescription.BufferCount = m_Description.m_uiBufferCount;
  swapChainDescription.Scaling     = DXGI_SCALING_NONE;

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
  // DXGI_SCALING_NONE is supported starting with Windows 8
  if (!IsWindows8OrGreater())
    swapChainDescription.Scaling = DXGI_SCALING_STRETCH;
#endif

  // DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL is the flip presentation model, where the contents of the back
  // buffer is preserved after the call to Present. This flag cannot be used with multisampling.
  // The only swap effect that supports multisampling is DXGI_SWAP_EFFECT_DISCARD.
  // Windows Store apps must use DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL or DXGI_SWAP_EFFECT_FLIP_DISCARD.
  swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

  swapChainDescription.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED; //  Transparency behavior is not specified

  // DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH enables an application to switch modes by calling
  // IDXGISwapChain::ResizeTarget(). When switching from windowed to fullscreen mode, the display
  // mode (or monitor resolution) will be changed to match the dimensions of the application window.
  swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

  // Create DXGI Factory.
  IDXGIFactory4* pDXGIFactory = pDeviceD3D11->GetDXGIFactory();

  // DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT enables querying a waitable object that can be
  // used to synchronize presentation with CPU timeline.
  // The flag is not supported in D3D11 fullscreen mode.
  if (!m_FullScreenMode.m_bIsFullScreen)
  {
    swapChainDescription.Flags |= DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
  }

#if XII_ENABLED(XII_PLATFORM_WINDOWS_DESKTOP)
  DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDescription = {};

  fullScreenDescription.Windowed                = D3D11_BOOL(!m_FullScreenMode.m_bIsFullScreen);
  fullScreenDescription.RefreshRate.Numerator   = m_FullScreenMode.m_uiRefreshRateNumerator;
  fullScreenDescription.RefreshRate.Denominator = m_FullScreenMode.m_uiRefreshRateDenominator;
  fullScreenDescription.Scaling                 = xiiD3D11TypeConversions::GetScalingMode(m_FullScreenMode.m_ScalingMode);
  fullScreenDescription.ScanlineOrdering        = xiiD3D11TypeConversions::GetScanLineOrder(m_FullScreenMode.m_ScanLineOrder);

  IDXGISwapChain1* pSwapChain1 = nullptr;
  XII_SCOPE_EXIT(XII_GAL_D3D11_RELEASE(pSwapChain1));

  HRESULT hResult = pDXGIFactory->CreateSwapChainForHwnd(pDeviceD3D11->GetD3D11Device(), hNativeWindow, &swapChainDescription, &fullScreenDescription, nullptr, &pSwapChain1);
  if (FAILED(hResult))
  {
    if (hResult == E_ACCESSDENIED)
    {
      xiiLog::Error("Failed to create the DXGI Swap Chain: {}. This may occur when the old swap chain is still in use. Ensure that all resources referencing the swap chain were destroyed, keeping in mind the deferred destruction that applies with FLIP model swap chains.", xiiHRESULTtoString(hResult));
    }
    else
    {
      xiiLog::Error("Failed to create the DXGI Swap Chain: {}", xiiHRESULTtoString(hResult));
    }
    return XII_FAILURE;
  }

  {
    // The IDXGIFactory used for MakeWindowAssociation must be retrieved via calling IDXGISwapchain::GetParent first, otherwise it won't work.
    // https://www.gamedev.net/forums/topic/634235-dxgidisabling-altenter/?do=findComment&comment=4999990
    IDXGIFactory1* pFactoryFromSC = nullptr;
    XII_SCOPE_EXIT(XII_GAL_D3D11_RELEASE(pFactoryFromSC));

    if (SUCCEEDED(pSwapChain1->GetParent(__uuidof(pFactoryFromSC), (void**)&pFactoryFromSC)))
    {
      // Do not allow the swap chain to handle Alt+Enter.
      pFactoryFromSC->MakeWindowAssociation(hNativeWindow, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
    }
  }
#elif XII_ENABLED(XII_PLATFORM_WINDOWS_UWP)
  if (m_FullScreenMode.m_bIsFullScreen)
  {
    xiiLog::Warning("UWP applications do not support full screen mode.");
  }

  if (FAILED(pDXGIFactory->CreateSwapChainForCoreWindow(pDeviceD3D11->GetD3D11Device(), reinterpret_cast<IUnknown*>(m_Description.m_pWindow->GetNativeWindowHandle()), &swapChainDescription, nullptr, &pSwapChain1)))
  {
    xiiLog::Error("Failed to create the DXGI Swap Chain.");
    return XII_FAILURE;
  }
#endif

  if (FAILED(pSwapChain1->QueryInterface(__uuidof(m_pSwapChain), reinterpret_cast<void**>(static_cast<IDXGISwapChain4**>(&m_pSwapChain)))))
  {
    xiiLog::Error("Failed to retrieve IDXGISwapChain4 from DXGI interface.");
    return XII_FAILURE;
  }

  if ((swapChainDescription.Flags & DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT) != 0)
  {
    // IMPORTANT: SetMaximumFrameLatency must be called BEFORE GetFrameLatencyWaitableObject!
    m_pSwapChain->SetMaximumFrameLatency(m_uiMaximumFrameLatency);

    m_FrameLatencyWaitableObject = m_pSwapChain->GetFrameLatencyWaitableObject();

    XII_ASSERT_DEV(m_FrameLatencyWaitableObject != NULL, "The DXGI Swapchain waitable object must not be null.");
  }
  else
  {
    m_FrameLatencyWaitableObject = NULL;
    m_uiMaximumFrameLatency      = 0U;
  }

  return XII_SUCCESS;
}

xiiResult xiiGALSwapChainD3D11::UpdateSwapChain(bool bCreateNew)
{
  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);

  // When switching to full screen mode, WM_SIZE is sent to the window and Resize() is called before the new swap chain is created.
  if (!m_pSwapChain)
    return XII_SUCCESS;

  if (ID3D11DeviceContext* pContextD3D11 = pDeviceD3D11->GetImmediateContext())
  {
    // Reset command list swapchain references or ResizeBuffers will fail as the backbuffer is still referenced.
    pDeviceD3D11->ResetCommandQueuesSwapChainReferences();

    DestroyBackBufferInternal(pDeviceD3D11);

    // Need to flush pending deletion or ResizeBuffers will fail as the backbuffer is still referenced.
    pDeviceD3D11->FlushPendingObjects();

    if (bCreateNew)
    {
      XII_GAL_D3D11_RELEASE(m_pSwapChain);

      // Only one flip presentation model swap chain can be associated with an HWND.
      // We must make sure that the swap chain is actually released by D3D11 before creating a new one.
      // To force the destruction, we need to ensure no views are bound to pipeline state, and then call Flush
      // on the immediate context. Destruction must be forced before calling IDXGIFactory2::CreateSwapChainForHwnd(), or
      // IDXGIFactory2::CreateSwapChainForCoreWindow() again to create a new swap chain.
      // https://msdn.microsoft.com/en-us/library/windows/desktop/ff476425(v=vs.85).aspx#Defer_Issues_with_Flip
      pContextD3D11->Flush();

      CreateDXGISwapChain().AssertSuccess();
    }
    else
    {
      DXGI_SWAP_CHAIN_DESC swapChainDescription;
      memset(&swapChainDescription, 0, sizeof(swapChainDescription));
      m_pSwapChain->GetDesc(&swapChainDescription);

      if (FAILED(m_pSwapChain->ResizeBuffers(swapChainDescription.BufferCount, m_Description.m_Resolution.width, m_Description.m_Resolution.height, swapChainDescription.BufferDesc.Format, swapChainDescription.Flags)))
      {
        xiiLog::Error("Failed to resize the DXGI swap chain.");
        return XII_FAILURE;
      }

      // Call flush to release resources.
      pContextD3D11->Flush();
    }
  }
  return CreateBackBufferInternal(pDeviceD3D11);
}

xiiResult xiiGALSwapChainD3D11::CreateBackBufferInternal(xiiGALDeviceD3D11* pDeviceD3D11)
{
  DestroyBackBufferInternal(pDeviceD3D11);

  // Retrieve the texture of the swap chain.
  ID3D11Texture2D* pNativeBackBuffer = nullptr;
  HRESULT          hResult           = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pNativeBackBuffer));
  if (FAILED(hResult))
  {
    xiiLog::Error("Failed to access backbuffer texture of swapchain: {0}", xiiHRESULTtoString(hResult));

    XII_GAL_D3D11_RELEASE(m_pSwapChain);
    return XII_FAILURE;
  }

  D3D11_TEXTURE2D_DESC d3d11BackbufferTextureDescription;
  pNativeBackBuffer->GetDesc(&d3d11BackbufferTextureDescription);

  xiiGALTextureCreationDescription textureDescription;
  textureDescription.m_Type                   = d3d11BackbufferTextureDescription.ArraySize > 1 ? xiiGALResourceDimension::Texture2DArray : xiiGALResourceDimension::Texture2D;
  textureDescription.m_Size.width             = d3d11BackbufferTextureDescription.Width;
  textureDescription.m_Size.height            = d3d11BackbufferTextureDescription.Height;
  textureDescription.m_Format                 = xiiD3D11TypeConversions::GetGALFormat(d3d11BackbufferTextureDescription.Format);
  textureDescription.m_uiArraySizeOrDepth     = d3d11BackbufferTextureDescription.ArraySize;
  textureDescription.m_uiMipLevels            = d3d11BackbufferTextureDescription.MipLevels;
  textureDescription.m_uiSampleCount          = d3d11BackbufferTextureDescription.SampleDesc.Count;
  textureDescription.m_BindFlags              = xiiD3D11TypeConversions::GetGALBindFlags(d3d11BackbufferTextureDescription.BindFlags);
  textureDescription.m_Usage                  = xiiD3D11TypeConversions::GetGALUsage(d3d11BackbufferTextureDescription.Usage);
  textureDescription.m_CPUAccessFlags         = xiiD3D11TypeConversions::GetGALCPUAccessFlags(d3d11BackbufferTextureDescription.CPUAccessFlags);
  textureDescription.m_MiscFlags              = xiiD3D11TypeConversions::GetGALMiscTextureFlags(d3d11BackbufferTextureDescription.MiscFlags);
  textureDescription.m_pExisitingNativeObject = pNativeBackBuffer;

  if (d3d11BackbufferTextureDescription.MiscFlags & D3D11_RESOURCE_MISC_TILED)
  {
    XII_ASSERT_DEV(textureDescription.m_Usage == xiiGALResourceUsage::Immutable, "");

    textureDescription.m_Usage = xiiGALResourceUsage::Sparse;

    // In Direct3D11 sparse resources are always aliased.
    textureDescription.m_MiscFlags.Add(xiiGALMiscTextureFlags::SparseAlias);
  }

  xiiGALTextureHandle hBackbufferTexture = pDeviceD3D11->CreateTexture(textureDescription);
  XII_ASSERT_RELEASE(!hBackbufferTexture.IsInvalidated(), "Failed to create native backbuffer texture object!");

  m_hBackBufferTexture = hBackbufferTexture;

  // If we sRGB backbuffer was requested, we create a "practical backbuffer".
  if (m_Description.m_ColorBufferFormat == xiiGALTextureFormat::RGBA8UNormalizedSRGB || m_Description.m_ColorBufferFormat == xiiGALTextureFormat::BGRA8UNormalizedSRGB)
  {
    textureDescription.m_pExisitingNativeObject = nullptr;
    textureDescription.m_Format                 = m_Description.m_ColorBufferFormat;

    m_hActualBackBufferTexture = m_hBackBufferTexture;
    m_hBackBufferTexture       = pDeviceD3D11->CreateTexture(textureDescription);
    XII_ASSERT_RELEASE(!hBackbufferTexture.IsInvalidated(), "Failed to create practical backbuffer texture object!");
  }

  m_Description.m_Resolution = textureDescription.m_Size;

  if (!m_hActualBackBufferTexture.IsInvalidated())
  {
    m_pDevice->GetTexture(m_hActualBackBufferTexture)->SetDebugName("Internal Backbuffer");
  }
  m_pDevice->GetTexture(m_hBackBufferTexture)->SetDebugName("Main Backbuffer");

  return XII_SUCCESS;
}

void xiiGALSwapChainD3D11::DestroyBackBufferInternal(xiiGALDeviceD3D11* pDeviceD3D11)
{
  if (!m_hBackBufferTexture.IsInvalidated())
  {
    pDeviceD3D11->DestroyTexture(m_hBackBufferTexture);
    m_hBackBufferTexture.Invalidate();
  }

  if (!m_hActualBackBufferTexture.IsInvalidated())
  {
    pDeviceD3D11->DestroyTexture(m_hActualBackBufferTexture);
    m_hActualBackBufferTexture.Invalidate();
  }
}

void xiiGALSwapChainD3D11::AcquireNextRenderTarget()
{
}

void xiiGALSwapChainD3D11::Present()
{
  XII_PROFILE_SCOPE("PresentRenderTarget");

  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);

  if (!m_hActualBackBufferTexture.IsInvalidated())
  {
    if (auto pQueue = pDeviceD3D11->GetDefaultCommandQueue(xiiGALCommandQueueType::Graphics))
    {
      auto pCommandList = pQueue->BeginCommandList();

      pCommandList->CopyTexture(m_hBackBufferTexture, m_hActualBackBufferTexture);

      pQueue->Submit(pCommandList);
      pQueue->WaitForIdle();
    }
  }

  xiiUInt32 uiSyncInterval = 1U;

#if XII_DISABLED(XII_PLATFORM_WINDOWS_UWP)
  switch (m_PresentMode)
  {
    case xiiGALPresentMode::Immediate:
      uiSyncInterval = 0U;
      break;
    case xiiGALPresentMode::VSync:
      uiSyncInterval = 1U;
      break;
  }
#endif

  // We wait for the frame as late as possible - right before presenting.
  // https://docs.microsoft.com/en-us/windows/uwp/gaming/reduce-latency-with-dxgi-1-3-swap-chains#step-4-wait-before-rendering-each-frame
  WaitForFrame();

  HRESULT hResult = m_pSwapChain->Present(uiSyncInterval, 0);
  if (FAILED(hResult))
  {
    xiiLog::Error("Failed to present to swap chain: {}", xiiHRESULTtoString(hResult));
  }
}

xiiResult xiiGALSwapChainD3D11::Resize(xiiSizeU32 newSize, xiiEnum<xiiGALSurfaceTransform> newTransform)
{
  if (newTransform != xiiGALSurfaceTransform::Optimal && newTransform != xiiGALSurfaceTransform::Identity)
  {
    xiiLog::Warning("The current pre-transform is unsupported by Direct3D swap chains. Use xiiGALSurfaceTransform::Optimal (recommended) or xiiGALSurfaceTransform::Identity.");
  }
  newTransform = xiiGALSurfaceTransform::Optimal;

  if (newSize.HasNonZeroArea() && (newSize.width != m_Description.m_Resolution.width || newSize.height != m_Description.m_Resolution.height || m_DesiredSurfaceTransform != newTransform))
  {
    m_Description.m_Resolution = newSize;

    if (UpdateSwapChain(false).Succeeded())
    {
      xiiLog::Info("Resized swapchain to {}x{}.", m_Description.m_Resolution.width, m_Description.m_Resolution.height);
    }
  }
  return XII_SUCCESS;
}

void xiiGALSwapChainD3D11::SetFullScreenMode(const xiiGALDisplayModeDescription& displayMode)
{
  if (m_pSwapChain)
  {
    // If we are already in fullscreen mode, we need to switch to windowed mode first,
    // because a swap chain must be in windowed mode when it is released.
    // https://msdn.microsoft.com/en-us/library/windows/desktop/bb205075(v=vs.85).aspx#Destroying
    if (m_FullScreenMode.m_bIsFullScreen)
    {
      m_pSwapChain->SetFullscreenState(FALSE, nullptr);
    }

    m_FullScreenMode.m_bIsFullScreen            = true;
    m_FullScreenMode.m_uiRefreshRateNumerator   = displayMode.m_uiRefreshRateNumerator;
    m_FullScreenMode.m_uiRefreshRateDenominator = displayMode.m_uiRefreshRateDenominator;
    m_FullScreenMode.m_ScalingMode              = displayMode.m_ScalingMode;
    m_FullScreenMode.m_ScanLineOrder            = displayMode.m_ScanLineOrder;

    m_Description.m_Resolution = displayMode.m_Resolution;
    if (displayMode.m_TextureFormat != xiiGALTextureFormat::Unknown)
    {
      m_Description.m_ColorBufferFormat = displayMode.m_TextureFormat;
    }

    UpdateSwapChain(true).AssertSuccess();
  }
}

void xiiGALSwapChainD3D11::SetWindowedMode()
{
  if (m_FullScreenMode.m_bIsFullScreen)
  {
    m_FullScreenMode.m_bIsFullScreen = false;

    m_pSwapChain->SetFullscreenState(FALSE, nullptr);
  }
}

void xiiGALSwapChainD3D11::SetMaximumFrameLatency(xiiUInt32 uiMaxLatency)
{
  if (m_uiMaximumFrameLatency == uiMaxLatency)
    return;

  m_uiMaximumFrameLatency = uiMaxLatency;

  if (m_FrameLatencyWaitableObject != NULL)
  {
    // A swap chain must be in windowed mode when it is released.
    // https://msdn.microsoft.com/en-us/library/windows/desktop/bb205075(v=vs.85).aspx#Destroying
    if (m_FullScreenMode.m_bIsFullScreen)
    {
      m_pSwapChain->SetFullscreenState(FALSE, nullptr);
    }

    // Destroying the swap chain and creating a new one is the only reliable way to change the maximum frame latency of a waitable swap chain.
    UpdateSwapChain(true).AssertSuccess();
  }
}

void xiiGALSwapChainD3D11::WaitForFrame()
{
  // https://docs.microsoft.com/en-us/windows/uwp/gaming/reduce-latency-with-dxgi-1-3-swap-chains#step-4-wait-before-rendering-each-frame
  if (m_FrameLatencyWaitableObject != NULL)
  {
    // 0.5 second timeout (shouldn't ever occur)
    DWORD uiResult = WaitForSingleObjectEx(m_FrameLatencyWaitableObject, 500, true);

    if (uiResult != WAIT_OBJECT_0)
    {
      if (uiResult == WAIT_TIMEOUT)
      {
        xiiLog::Error("Timeout elapsed while waiting for the frame waitable object. This is a strong indication of a synchronization error.");
      }
      else
      {
        xiiLog::Error("Waiting for the frame waitable object failed. This is a strong indication of a synchronization error.");
      }
    }
  }
}

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_Device_Implementation_SwapChainD3D11);
