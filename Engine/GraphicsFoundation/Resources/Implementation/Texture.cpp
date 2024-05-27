#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/Device/Device.h>
#include <GraphicsFoundation/Resources/Texture.h>

// clang-format off

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALMiscTextureFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALMiscTextureFlags::None),
  XII_BITFLAGS_CONSTANT(xiiGALMiscTextureFlags::GenerateMips),
  XII_BITFLAGS_CONSTANT(xiiGALMiscTextureFlags::Memoryless),
  XII_BITFLAGS_CONSTANT(xiiGALMiscTextureFlags::SparseAlias),
  XII_BITFLAGS_CONSTANT(xiiGALMiscTextureFlags::Subsampled),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALTexture, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;

// clang-format on

xiiGALTexture::xiiGALTexture(xiiGALDevice* pDevice, const xiiGALTextureCreationDescription& creationDescription) :
  xiiGALResource(pDevice), m_Description(creationDescription)
{
}

xiiGALTexture::~xiiGALTexture() = default;

void xiiGALTexture::CreateDefaultResourceViews(xiiGALTextureHandle hTexture)
{
  xiiGALTextureViewCreationDescription viewDescription;
  viewDescription.m_hTexture                  = hTexture;
  viewDescription.m_uiMostDetailedMip         = 0U;
  viewDescription.m_uiFirstArrayOrDepthSlice  = 0U;
  viewDescription.m_uiMipLevelCount           = m_Description.m_uiMipLevels;
  viewDescription.m_uiArrayOrDepthSlicesCount = m_Description.GetArraySize();

  // Allow viewDescription.m_Format and viewDescription.m_ResourceDimension to be determined by the device.

  if (m_Description.m_BindFlags.IsSet(xiiGALBindFlags::ShaderResource))
  {
    auto shaderResourceViewDescription       = viewDescription;
    shaderResourceViewDescription.m_ViewType = xiiGALTextureViewType::ShaderResource;

    if (m_Description.m_MiscFlags.IsSet(xiiGALMiscTextureFlags::GenerateMips))
      shaderResourceViewDescription.m_Flags.Add(xiiGALTextureViewFlags::AllowMipGeneration);

    m_DefaultTextureViews[xiiGALTextureViewType::ShaderResource] = m_pDevice->CreateTextureView(shaderResourceViewDescription);
  }
  if (m_Description.m_BindFlags.IsSet(xiiGALBindFlags::RenderTarget))
  {
    viewDescription.m_ViewType                                 = xiiGALTextureViewType::RenderTarget;
    m_DefaultTextureViews[xiiGALTextureViewType::RenderTarget] = m_pDevice->CreateTextureView(viewDescription);
  }
  if (m_Description.m_BindFlags.IsSet(xiiGALBindFlags::DepthStencil))
  {
    viewDescription.m_ViewType                                 = xiiGALTextureViewType::DepthStencil;
    m_DefaultTextureViews[xiiGALTextureViewType::DepthStencil] = m_pDevice->CreateTextureView(viewDescription);

    viewDescription.m_ViewType                                         = xiiGALTextureViewType::ReadOnlyDepthStencil;
    m_DefaultTextureViews[xiiGALTextureViewType::ReadOnlyDepthStencil] = m_pDevice->CreateTextureView(viewDescription);
  }
  if (m_Description.m_BindFlags.IsSet(xiiGALBindFlags::UnorderedAccess))
  {
    viewDescription.m_ViewType                                    = xiiGALTextureViewType::UnorderedAccess;
    m_DefaultTextureViews[xiiGALTextureViewType::UnorderedAccess] = m_pDevice->CreateTextureView(viewDescription);
  }
  if (m_Description.m_BindFlags.IsSet(xiiGALBindFlags::ShadingRate))
  {
    viewDescription.m_ViewType                                = xiiGALTextureViewType::ShadingRate;
    m_DefaultTextureViews[xiiGALTextureViewType::ShadingRate] = m_pDevice->CreateTextureView(viewDescription);
  }
}

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_Resources_Implementation_Texture);
