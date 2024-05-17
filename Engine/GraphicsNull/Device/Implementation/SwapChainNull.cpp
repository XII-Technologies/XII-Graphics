#include <GraphicsNull/GraphicsNullPCH.h>

#include <Core/System/Window.h>
#include <Foundation/Profiling/Profiling.h>
#include <GraphicsNull/Device/DeviceNull.h>
#include <GraphicsNull/Device/SwapChainNull.h>
#include <GraphicsNull/Resources/TextureNull.h>

xiiGALSwapChainNull::xiiGALSwapChainNull(xiiGALDeviceNull* pDeviceNull, const xiiGALSwapChainCreationDescription& creationDescription) :
  xiiGALSwapChain(pDeviceNull, creationDescription)
{
}

xiiGALSwapChainNull::~xiiGALSwapChainNull() = default;

xiiResult xiiGALSwapChainNull::InitPlatform()
{
  xiiGALDeviceNull* pDeviceNull = static_cast<xiiGALDeviceNull*>(m_pDevice);

  // We have created a surface on a window, the window must not be destroyed while the surface is still alive.
  m_Description.m_pWindow->AddReference();

  return CreateBackBufferInternal(pDeviceNull);
}

xiiResult xiiGALSwapChainNull::DeInitPlatform()
{
  xiiGALDeviceNull* pDeviceNull = static_cast<xiiGALDeviceNull*>(m_pDevice);

  DestroyBackBufferInternal(pDeviceNull);

  m_Description.m_pWindow->RemoveReference();

  return XII_SUCCESS;
}

xiiResult xiiGALSwapChainNull::CreateBackBufferInternal(xiiGALDeviceNull* pDeviceNull)
{
  xiiEnum<xiiGALTextureFormat> textureFormat = pDeviceNull->GetDescription().m_GraphicsDeviceType != xiiGALGraphicsDeviceType::Vulkan ? xiiGALTextureFormat::RGBA8UNormalizedSRGB : xiiGALTextureFormat::BGRA8UNormalizedSRGB;

  xiiGALTextureCreationDescription textureDescription;
  textureDescription.m_sName              = "SwapChain Null Render Target.";
  textureDescription.m_Type               = xiiGALResourceDimension::Texture2D;
  textureDescription.m_Size               = m_Description.m_pWindow->GetClientAreaSize();
  textureDescription.m_uiArraySizeOrDepth = 1U;
  textureDescription.m_Format             = textureFormat;
  textureDescription.m_uiMipLevels        = 1U;
  textureDescription.m_uiSampleCount      = xiiGALSampleCount::OneSample;
  textureDescription.m_BindFlags          = xiiGALBindFlags::RenderTarget | xiiGALBindFlags::ShaderResource;
  textureDescription.m_Usage              = xiiGALResourceUsage::Default;
  textureDescription.m_CPUAccessFlags     = xiiGALCPUAccessFlag::Read;
  textureDescription.m_MiscFlags          = xiiGALMiscTextureFlags::None;

  textureDescription.m_ClearValue.m_TextureFormat            = textureFormat;
  textureDescription.m_ClearValue.m_ClearColor               = xiiColor::Black;
  textureDescription.m_ClearValue.m_DepthStencil.m_fDepth    = 1.0f;
  textureDescription.m_ClearValue.m_DepthStencil.m_uiStencil = 0U;
  textureDescription.m_uiImmediateContextMask                = XII_BIT(0);

  textureDescription.m_pExisitingNativeObject = nullptr;

  m_hBackBufferTexture = pDeviceNull->CreateTexture(textureDescription);
  XII_ASSERT_RELEASE(!m_hBackBufferTexture.IsInvalidated(), "Couldn't create native backbuffer texture object!");

  m_Description.m_Resolution = textureDescription.m_Size;

  return XII_SUCCESS;
}

void xiiGALSwapChainNull::DestroyBackBufferInternal(xiiGALDeviceNull* pDeviceNull)
{
  pDeviceNull->DestroyTexture(m_hBackBufferTexture);
  m_hBackBufferTexture.Invalidate();
}

void xiiGALSwapChainNull::AcquireNextRenderTarget()
{
}

void xiiGALSwapChainNull::Present()
{
}

xiiResult xiiGALSwapChainNull::Resize(xiiSizeU32 newSize, xiiEnum<xiiGALSurfaceTransform> newTransform)
{
  xiiGALDeviceNull* pDeviceNull = static_cast<xiiGALDeviceNull*>(m_pDevice);

  DestroyBackBufferInternal(pDeviceNull);

  // Need to flush dead objects or ResizeBuffers will fail as the backbuffer is still referenced.
  pDeviceNull->FlushPendingObjects();

  return CreateBackBufferInternal(pDeviceNull);
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_Device_Implementation_SwapChainNull);
