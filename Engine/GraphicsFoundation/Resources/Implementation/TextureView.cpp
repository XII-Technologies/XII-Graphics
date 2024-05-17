#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/Resources/TextureView.h>

// clang-format off

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALUnorderedAccessViewFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALUnorderedAccessViewFlags::Unspecified),
  XII_BITFLAGS_CONSTANT(xiiGALUnorderedAccessViewFlags::Read),
  XII_BITFLAGS_CONSTANT(xiiGALUnorderedAccessViewFlags::Write),
  XII_BITFLAGS_CONSTANT(xiiGALUnorderedAccessViewFlags::ReadWrite),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALTextureViewFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALTextureViewFlags::None),
  XII_BITFLAGS_CONSTANT(xiiGALTextureViewFlags::AllowMipGeneration),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALTextureComponentSwizzle, 1)
  XII_ENUM_CONSTANT(xiiGALTextureComponentSwizzle::Identity),
  XII_ENUM_CONSTANT(xiiGALTextureComponentSwizzle::Zero),
  XII_ENUM_CONSTANT(xiiGALTextureComponentSwizzle::One),
  XII_ENUM_CONSTANT(xiiGALTextureComponentSwizzle::R),
  XII_ENUM_CONSTANT(xiiGALTextureComponentSwizzle::G),
  XII_ENUM_CONSTANT(xiiGALTextureComponentSwizzle::B),
  XII_ENUM_CONSTANT(xiiGALTextureComponentSwizzle::A),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALTextureView, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;

// clang-format on

xiiGALTextureView::xiiGALTextureView(xiiGALDevice* pDevice, xiiGALTexture* pTexture, const xiiGALTextureViewCreationDescription& creationDescription) :
  xiiGALResourceView(pDevice), m_Description(creationDescription), m_pTexture(pTexture)
{
  XII_ASSERT_DEV(m_pTexture != nullptr, "The given texture must not be nullptr.");
}

xiiGALTextureView::~xiiGALTextureView() = default;

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_Resources_Implementation_TextureView);
