#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/Resources/Texture.h>
#include <GraphicsFoundation/Utilities/GraphicsUtilities.h>

const xiiGALTextureFormatDescription& xiiGALGraphicsUtilities::GetTextureFormatProperties(xiiEnum<xiiGALTextureFormat> format)
{
  static xiiGALTextureFormatDescription formatDescriptions[xiiGALTextureFormat::ENUM_COUNT];
  static bool                           bIsInitialized = false;

  // Note that this implementation is thread safe. Even if multiple threads call the function, the data may be initialized multiple times but the result will be the same.
  if (!bIsInitialized)
  {
#define FILL_TEXTURE_FORMAT_INFO(format, componentSize, componentCount, componentType, isTypeless, blockWidth, blockHeight) \
  formatDescriptions[format].m_Format           = format;                                                                   \
  formatDescriptions[format].m_uiComponentSize  = componentSize;                                                            \
  formatDescriptions[format].m_uiComponentCount = componentCount;                                                           \
  formatDescriptions[format].m_ComponentType    = componentType;                                                            \
  formatDescriptions[format].m_bIsTypeless      = isTypeless;                                                               \
  formatDescriptions[format].m_uiBlockWidth     = blockWidth;                                                               \
  formatDescriptions[format].m_uiBlockHeight    = blockHeight

    // clang-format off

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGBA32Typeless, 4, 4, xiiGALTextureFormatComponentType::Undefined,        true,  1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGBA32Float,    4, 4, xiiGALTextureFormatComponentType::Float,            false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGBA32UInt,     4, 4, xiiGALTextureFormatComponentType::UnsignedInteger,  false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGBA32SInt,     4, 4, xiiGALTextureFormatComponentType::SignedInteger,    false, 1, 1);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGB32Typeless, 4, 3, xiiGALTextureFormatComponentType::Undefined,        true,  1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGB32Float,    4, 3, xiiGALTextureFormatComponentType::Float,            false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGB32UInt,     4, 3, xiiGALTextureFormatComponentType::UnsignedInteger,  false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGB32SInt,     4, 3, xiiGALTextureFormatComponentType::SignedInteger,    false, 1, 1);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGBA16Typeless,    2, 4, xiiGALTextureFormatComponentType::Undefined,          true,  1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGBA16Float,       2, 4, xiiGALTextureFormatComponentType::Float,              false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGBA16UNormalized, 2, 4, xiiGALTextureFormatComponentType::UnsignedNormalized, false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGBA16UInt,        2, 4, xiiGALTextureFormatComponentType::UnsignedInteger,    false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGBA16SNormalized, 2, 4, xiiGALTextureFormatComponentType::SignedNormalized,   false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGBA16SInt,        2, 4, xiiGALTextureFormatComponentType::SignedInteger,      false, 1, 1);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RG32Typeless, 4, 2, xiiGALTextureFormatComponentType::Undefined,        true,  1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RG32Float,    4, 2, xiiGALTextureFormatComponentType::Float,            false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RG32UInt,     4, 2, xiiGALTextureFormatComponentType::UnsignedInteger,  false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RG32SInt,     4, 2, xiiGALTextureFormatComponentType::SignedInteger,    false, 1, 1);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R32G8X24Typeless,      4, 2, xiiGALTextureFormatComponentType::DepthStencil, true,  1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::D32FloatS8X24UInt,     4, 2, xiiGALTextureFormatComponentType::DepthStencil, false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R32FloatX8X24Typeless, 4, 2, xiiGALTextureFormatComponentType::DepthStencil, false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::X32TypelessG8X24UInt,  4, 2, xiiGALTextureFormatComponentType::DepthStencil, false, 1, 1);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGB10A2Typeless,    4, 1, xiiGALTextureFormatComponentType::Compound, true,  1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGB10A2UNormalized, 4, 1, xiiGALTextureFormatComponentType::Compound, false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGB10A2UInt,        4, 1, xiiGALTextureFormatComponentType::Compound, false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RG11B10Float,       4, 1, xiiGALTextureFormatComponentType::Compound, false, 1, 1);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGBA8Typeless,        1, 4, xiiGALTextureFormatComponentType::Undefined,              true,  1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGBA8UNormalized,     1, 4, xiiGALTextureFormatComponentType::UnsignedNormalized,     false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGBA8UNormalizedSRGB, 1, 4, xiiGALTextureFormatComponentType::UnsignedNormalizedSRGB, false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGBA8UInt,            1, 4, xiiGALTextureFormatComponentType::UnsignedInteger,        false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGBA8SNormalized,     1, 4, xiiGALTextureFormatComponentType::SignedNormalized,       false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGBA8SInt,            1, 4, xiiGALTextureFormatComponentType::SignedInteger,          false, 1, 1);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RG16Typeless,    2, 2, xiiGALTextureFormatComponentType::Undefined,          true,  1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RG16Float,       2, 2, xiiGALTextureFormatComponentType::Float,              false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RG16UNormalized, 2, 2, xiiGALTextureFormatComponentType::UnsignedNormalized, false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RG16UInt,        2, 2, xiiGALTextureFormatComponentType::UnsignedInteger,    false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RG16SNormalized, 2, 2, xiiGALTextureFormatComponentType::SignedNormalized,   false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RG16SInt,        2, 2, xiiGALTextureFormatComponentType::SignedInteger,      false, 1, 1);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R32Typeless,     4, 1, xiiGALTextureFormatComponentType::Undefined,          true,  1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::D32Float,        4, 1, xiiGALTextureFormatComponentType::Depth,              false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R32Float,        4, 1, xiiGALTextureFormatComponentType::Float,              false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R32UInt,         4, 1, xiiGALTextureFormatComponentType::UnsignedInteger,    false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R32SInt,         4, 1, xiiGALTextureFormatComponentType::SignedInteger,      false, 1, 1);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R24G8Typeless,            4, 1, xiiGALTextureFormatComponentType::DepthStencil, true,  1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::D24UNormalizedS8UInt,     4, 1, xiiGALTextureFormatComponentType::DepthStencil, false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R24UNormalizedX8Typeless, 4, 1, xiiGALTextureFormatComponentType::DepthStencil, false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::X24TypelessG8UInt,        4, 1, xiiGALTextureFormatComponentType::DepthStencil, false, 1, 1);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RG8Typeless,    1, 2, xiiGALTextureFormatComponentType::Undefined,          true,  1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RG8UNormalized, 1, 2, xiiGALTextureFormatComponentType::UnsignedNormalized, false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RG8UInt,        1, 2, xiiGALTextureFormatComponentType::UnsignedInteger,    false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RG8SNormalized, 1, 2, xiiGALTextureFormatComponentType::SignedNormalized,   false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RG8SInt,        1, 2, xiiGALTextureFormatComponentType::SignedInteger,      false, 1, 1);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R16Typeless,    2, 1, xiiGALTextureFormatComponentType::Undefined,          true,  1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R16Float,       2, 1, xiiGALTextureFormatComponentType::Float,              false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::D16UNormalized, 2, 1, xiiGALTextureFormatComponentType::Depth,              false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R16UNormalized, 2, 1, xiiGALTextureFormatComponentType::UnsignedNormalized, false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R16UInt,        2, 1, xiiGALTextureFormatComponentType::UnsignedInteger,    false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R16SNormalized, 2, 1, xiiGALTextureFormatComponentType::SignedNormalized,   false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R16SInt,        2, 1, xiiGALTextureFormatComponentType::SignedInteger,      false, 1, 1);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R8Typeless,    1, 1, xiiGALTextureFormatComponentType::Undefined,           true,  1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R8UNormalized, 1, 1, xiiGALTextureFormatComponentType::UnsignedNormalized,  false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R8UInt,        1, 1, xiiGALTextureFormatComponentType::UnsignedInteger,     false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R8SNormalized, 1, 1, xiiGALTextureFormatComponentType::SignedNormalized,    false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R8SInt,        1, 1, xiiGALTextureFormatComponentType::SignedInteger,       false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::A8UNormalized, 1, 1, xiiGALTextureFormatComponentType::UnsignedNormalized,  false, 1, 1);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R1UNormalized, 1, 1, xiiGALTextureFormatComponentType::UnsignedNormalized,  false, 1, 1);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RGB9E5SharedExponent, 4, 1, xiiGALTextureFormatComponentType::Compound,            false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::RG8BG8UNormalized,    1, 4, xiiGALTextureFormatComponentType::UnsignedNormalized,  false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::GR8GB8UNormalized,    1, 4, xiiGALTextureFormatComponentType::UnsignedNormalized,  false, 1, 1);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC1Typeless,        8, 3, xiiGALTextureFormatComponentType::Compressed,  true,  4, 4);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC1UNormalized,     8, 3, xiiGALTextureFormatComponentType::Compressed,  false, 4, 4);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC1UNormalizedSRGB, 8, 3, xiiGALTextureFormatComponentType::Compressed,  false, 4, 4);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC2Typeless,        16, 4, xiiGALTextureFormatComponentType::Compressed,  true,  4, 4);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC2UNormalized,     16, 4, xiiGALTextureFormatComponentType::Compressed,  false, 4, 4);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC2UNormalizedSRGB, 16, 4, xiiGALTextureFormatComponentType::Compressed,  false, 4, 4);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC3Typeless,        16, 4, xiiGALTextureFormatComponentType::Compressed,  true,  4, 4);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC3UNormalized,     16, 4, xiiGALTextureFormatComponentType::Compressed,  false, 4, 4);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC3UNormalizedSRGB, 16, 4, xiiGALTextureFormatComponentType::Compressed,  false, 4, 4);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC4Typeless,    8, 1, xiiGALTextureFormatComponentType::Compressed,  true,  4, 4);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC4UNormalized, 8, 1, xiiGALTextureFormatComponentType::Compressed,  false, 4, 4);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC4SNormalized, 8, 1, xiiGALTextureFormatComponentType::Compressed,  false, 4, 4);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC5Typeless,    16, 2, xiiGALTextureFormatComponentType::Compressed,  true,  4, 4);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC5UNormalized, 16, 2, xiiGALTextureFormatComponentType::Compressed,  false, 4, 4);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC5SNormalized, 16, 2, xiiGALTextureFormatComponentType::Compressed,  false, 4, 4);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::B5G6R5UNormalized,            2, 1, xiiGALTextureFormatComponentType::Compound,               false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::B5G5R5A1UNormalized,          2, 1, xiiGALTextureFormatComponentType::Compound,               false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BGRA8UNormalized,             1, 4, xiiGALTextureFormatComponentType::UnsignedNormalized,     false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BGRX8UNormalized,             1, 4, xiiGALTextureFormatComponentType::UnsignedNormalized,     false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::R10G10B10XRBiasA2UNormalized, 4, 1, xiiGALTextureFormatComponentType::Compound,               false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BGRA8Typeless,                1, 4, xiiGALTextureFormatComponentType::Undefined,              true,  1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BGRA8UNormalizedSRGB,         1, 4, xiiGALTextureFormatComponentType::UnsignedNormalizedSRGB, false, 1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BGRX8Typeless,                1, 4, xiiGALTextureFormatComponentType::Undefined,              true,  1, 1);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BGRX8UNormalizedSRGB,         1, 4, xiiGALTextureFormatComponentType::UnsignedNormalizedSRGB, false, 1, 1);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC6HTypeless, 16, 3, xiiGALTextureFormatComponentType::Compressed,  true,  4, 4);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC6HUF16,     16, 3, xiiGALTextureFormatComponentType::Compressed,  false, 4, 4);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC6HSF16,     16, 3, xiiGALTextureFormatComponentType::Compressed,  false, 4, 4);

    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC7Typeless,        16, 4, xiiGALTextureFormatComponentType::Compressed,  true,  4, 4);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC7UNormalized,     16, 4, xiiGALTextureFormatComponentType::Compressed,  false, 4, 4);
    FILL_TEXTURE_FORMAT_INFO(xiiGALTextureFormat::BC7UNormalizedSRGB, 16, 4, xiiGALTextureFormatComponentType::Compressed,  false, 4, 4);

    // clang-format on

#undef FILL_TEXTURE_FORMAT_INFO

#if XII_ENABLED(XII_COMPILE_FOR_DEBUG)
    for (xiiUInt32 i = xiiGALTextureFormat::Unknown; i < xiiGALTextureFormat::ENUM_COUNT; ++i)
    {
      XII_ASSERT_DEV(formatDescriptions[i].m_Format == static_cast<xiiGALTextureFormat::Enum>(i), "Encountered an uninitialized format.");
    }
#endif

    bIsInitialized = true;
  }

  if (format >= xiiGALTextureFormat::Unknown && format < xiiGALTextureFormat::ENUM_COUNT)
  {
    const auto& description = formatDescriptions[format];
    XII_ASSERT_DEV(description.m_Format == format, "Encountered an unexpected format.");
    return description;
  }

  XII_ASSERT_DEV(false, "Texture format {0} is not in the allowed rage [0, {1}].", format.GetValue(), 0, xiiGALTextureFormat::ENUM_COUNT - 1);
  return formatDescriptions[xiiGALTextureFormat::Unknown];
}

class TextureFormatToViewFormatConverter
{
public:
  TextureFormatToViewFormatConverter()
  {
    m_ViewFormats.SetCount(xiiGALTextureFormat::ENUM_COUNT);

    // clang-format off
#define INIT_TEX_VIEW_FORMAT_INFO(textureFormat, SRVFormat, RTVFormat, DSVFormat, UAVFormat) \
    {\
      m_ViewFormats[textureFormat].SetCount(xiiGALTextureViewType::ENUM_COUNT);                                     \
      m_ViewFormats[textureFormat][xiiGALTextureViewType::ShaderResource]       = xiiGALTextureFormat::##SRVFormat; \
      m_ViewFormats[textureFormat][xiiGALTextureViewType::RenderTarget]         = xiiGALTextureFormat::##RTVFormat; \
      m_ViewFormats[textureFormat][xiiGALTextureViewType::DepthStencil]         = xiiGALTextureFormat::##DSVFormat; \
      m_ViewFormats[textureFormat][xiiGALTextureViewType::ReadOnlyDepthStencil] = xiiGALTextureFormat::##DSVFormat; \
      m_ViewFormats[textureFormat][xiiGALTextureViewType::UnorderedAccess]      = xiiGALTextureFormat::##UAVFormat; \
    }
    static_assert(xiiGALTextureViewType::ENUM_COUNT == 6, "Please handle the new view type above, if necessary");

    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::Unknown,                  Unknown, Unknown, Unknown, Unknown);

    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGBA32Typeless,           RGBA32Float, RGBA32Float, Unknown, RGBA32Float);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGBA32Float,              RGBA32Float, RGBA32Float, Unknown, RGBA32Float);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGBA32UInt,               RGBA32UInt,  RGBA32UInt,  Unknown, RGBA32UInt);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGBA32SInt,               RGBA32SInt,  RGBA32SInt,  Unknown, RGBA32SInt);

    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGB32Typeless,            RGB32Float, RGB32Float, Unknown, RGB32Float);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGB32Float,               RGB32Float, RGB32Float, Unknown, RGB32Float);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGB32UInt,                RGB32UInt,  RGB32UInt,  Unknown, RGB32UInt);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGB32SInt,                RGB32SInt,  RGB32SInt,  Unknown, RGB32SInt);

    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGBA16Typeless,           RGBA16Float,       RGBA16Float,       Unknown, RGBA16Float);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGBA16Float,              RGBA16Float,       RGBA16Float,       Unknown, RGBA16Float);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGBA16UNormalized,        RGBA16UNormalized, RGBA16UNormalized, Unknown, RGBA16UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGBA16UInt,               RGBA16UInt,        RGBA16UInt,        Unknown, RGBA16UInt);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGBA16SNormalized,        RGBA16SNormalized, RGBA16SNormalized, Unknown, RGBA16SNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGBA16SInt,               RGBA16SInt,        RGBA16SInt,        Unknown, RGBA16SInt);

    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RG32Typeless,             RG32Float, RG32Float, Unknown, RG32Float);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RG32Float,                RG32Float, RG32Float, Unknown, RG32Float);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RG32UInt,                 RG32UInt,  RG32UInt,  Unknown, RG32UInt);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RG32SInt,                 RG32SInt,  RG32SInt,  Unknown, RG32SInt);

    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R32G8X24Typeless,         R32FloatX8X24Typeless, Unknown, D32FloatS8X24UInt, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::D32FloatS8X24UInt,        R32FloatX8X24Typeless, Unknown, D32FloatS8X24UInt, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R32FloatX8X24Typeless,    R32FloatX8X24Typeless, Unknown, D32FloatS8X24UInt, R32FloatX8X24Typeless);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::X32TypelessG8X24UInt,     X32TypelessG8X24UInt,  Unknown, D32FloatS8X24UInt, X32TypelessG8X24UInt);

    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGB10A2Typeless,          RGB10A2UNormalized, RGB10A2UNormalized, Unknown, RGB10A2UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGB10A2UNormalized,       RGB10A2UNormalized, RGB10A2UNormalized, Unknown, RGB10A2UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGB10A2UInt,              RGB10A2UInt,        RGB10A2UInt,        Unknown, RGB10A2UInt);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RG11B10Float,             RG11B10Float,       RG11B10Float,       Unknown, RG11B10Float);

    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGBA8Typeless,            RGBA8UNormalizedSRGB, RGBA8UNormalizedSRGB, Unknown, RGBA8UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGBA8UNormalized,         RGBA8UNormalized,     RGBA8UNormalized,     Unknown, RGBA8UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGBA8UNormalizedSRGB,     RGBA8UNormalizedSRGB, RGBA8UNormalizedSRGB, Unknown, RGBA8UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGBA8UInt,                RGBA8UInt,            RGBA8UInt,            Unknown, RGBA8UInt);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGBA8SNormalized,         RGBA8SNormalized,     RGBA8SNormalized,     Unknown, RGBA8SNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGBA8SInt,                RGBA8SInt,            RGBA8SInt,            Unknown, RGBA8SInt);

    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RG16Typeless,             RG16Float,       RG16Float,       Unknown, RG16Float);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RG16Float,                RG16Float,       RG16Float,       Unknown, RG16Float);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RG16UNormalized,          RG16UNormalized, RG16UNormalized, Unknown, RG16UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RG16UInt,                 RG16UInt,        RG16UInt,        Unknown, RG16UInt);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RG16SNormalized,          RG16SNormalized, RG16SNormalized, Unknown, RG16SNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RG16SInt,                 RG16SInt,        RG16SInt,        Unknown, RG16SInt);

    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R32Typeless,              R32Float, R32Float, D32Float, R32Float);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::D32Float,                 R32Float, R32Float, D32Float, R32Float);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R32Float,                 R32Float, R32Float, D32Float, R32Float);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R32UInt,                  R32UInt,  R32UInt,  Unknown,  R32UInt);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R32SInt,                  R32SInt,  R32SInt,  Unknown,  R32SInt);

    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R24G8Typeless,            R24UNormalizedX8Typeless, Unknown, D24UNormalizedS8UInt, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::D24UNormalizedS8UInt,     R24UNormalizedX8Typeless, Unknown, D24UNormalizedS8UInt, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R24UNormalizedX8Typeless, R24UNormalizedX8Typeless, Unknown, D24UNormalizedS8UInt, R24UNormalizedX8Typeless);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::X24TypelessG8UInt,        X24TypelessG8UInt,        Unknown, D24UNormalizedS8UInt, X24TypelessG8UInt);

    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RG8Typeless,              RG8UNormalized, RG8UNormalized, Unknown, RG8UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RG8UNormalized,           RG8UNormalized, RG8UNormalized, Unknown, RG8UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RG8UInt,                  RG8UInt,        RG8UInt,        Unknown, RG8UInt);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RG8SNormalized,           RG8SNormalized, RG8SNormalized, Unknown, RG8SNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RG8SInt,                  RG8SInt,        RG8SInt,        Unknown, RG8SInt);

    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R16Typeless,              R16Float,       R16Float,       Unknown,        R16Float);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R16Float,                 R16Float,       R16Float,       Unknown,        R16Float);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::D16UNormalized,           R16UNormalized, R16UNormalized, D16UNormalized, R16UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R16UNormalized,           R16UNormalized, R16UNormalized, D16UNormalized, R16UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R16UInt,                  R16UInt,        R16UInt,        Unknown,        R16UInt);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R16SNormalized,           R16SNormalized, R16SNormalized, Unknown,        R16SNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R16SInt,                  R16SInt,        R16SInt,        Unknown,        R16SInt);

    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R8Typeless,               R8UNormalized, R8UNormalized, Unknown, R8UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R8UNormalized,            R8UNormalized, R8UNormalized, Unknown, R8UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R8UInt,                   R8UInt,        R8UInt,        Unknown, R8UInt);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R8SNormalized,            R8SNormalized, R8SNormalized, Unknown, R8SNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R8SInt,                   R8SInt,        R8SInt,        Unknown, R8SInt);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::A8UNormalized,            A8UNormalized, A8UNormalized, Unknown, A8UNormalized);

    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R1UNormalized,            R1UNormalized, R1UNormalized, Unknown, R1UNormalized);

    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RGB9E5SharedExponent,     RGB9E5SharedExponent, RGB9E5SharedExponent, Unknown, RGB9E5SharedExponent);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::RG8BG8UNormalized,        RG8BG8UNormalized,    RG8BG8UNormalized,    Unknown, RG8BG8UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::GR8GB8UNormalized,        GR8GB8UNormalized,    GR8GB8UNormalized,    Unknown, GR8GB8UNormalized);

    // http://www.g-truc.net/post-0335.html
    // http://renderingpipeline.com/2012/07/texture-compression/
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC1Typeless,              BC1UNormalizedSRGB, Unknown, Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC1UNormalized,           BC1UNormalized,     Unknown, Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC1UNormalizedSRGB,       BC1UNormalizedSRGB, Unknown, Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC2Typeless,              BC2UNormalizedSRGB, Unknown, Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC2UNormalized,           BC2UNormalized,     Unknown, Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC2UNormalizedSRGB,       BC2UNormalizedSRGB, Unknown, Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC3Typeless,              BC3UNormalizedSRGB, Unknown, Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC3UNormalized,           BC3UNormalized,     Unknown, Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC3UNormalizedSRGB,       BC3UNormalizedSRGB, Unknown, Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC4Typeless,              BC4UNormalized,     Unknown, Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC4UNormalized,           BC4UNormalized,     Unknown, Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC4SNormalized,           BC4SNormalized,     Unknown, Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC5Typeless,              BC5UNormalized,     Unknown, Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC5UNormalized,           BC5UNormalized,     Unknown, Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC5SNormalized,           BC5SNormalized,     Unknown, Unknown, Unknown);

    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::B5G6R5UNormalized,        B5G6R5UNormalized,   B5G6R5UNormalized,     Unknown, B5G6R5UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::B5G5R5A1UNormalized,      B5G5R5A1UNormalized, B5G5R5A1UNormalized,   Unknown, B5G5R5A1UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BGRA8UNormalized,         BGRA8UNormalized,    BGRA8UNormalized,      Unknown, BGRA8UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BGRX8UNormalized,         BGRX8UNormalized,    BGRX8UNormalized,      Unknown, BGRX8UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::R10G10B10XRBiasA2UNormalized, R10G10B10XRBiasA2UNormalized, Unknown,  Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BGRA8Typeless,            BGRA8UNormalizedSRGB, BGRA8UNormalizedSRGB, Unknown, BGRA8UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BGRA8UNormalizedSRGB,     BGRA8UNormalizedSRGB, BGRA8UNormalizedSRGB, Unknown, BGRA8UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BGRX8Typeless,            BGRX8UNormalizedSRGB, BGRX8UNormalizedSRGB, Unknown, BGRX8UNormalized);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BGRX8UNormalizedSRGB,     BGRX8UNormalizedSRGB, BGRX8UNormalizedSRGB, Unknown, BGRX8UNormalized);

    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC6HTypeless,             BC6HUF16,           Unknown, Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC6HUF16,                 BC6HUF16,           Unknown, Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC6HSF16,                 BC6HSF16,           Unknown, Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC7Typeless,              BC7UNormalizedSRGB, Unknown, Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC7UNormalized,           BC7UNormalized,     Unknown, Unknown, Unknown);
    INIT_TEX_VIEW_FORMAT_INFO(xiiGALTextureFormat::BC7UNormalizedSRGB,       BC7UNormalizedSRGB, Unknown, Unknown, Unknown);
#undef INIT_TVIEW_FORMAT_INFO
    // clang-format on

    m_ViewFormats[xiiGALTextureFormat::R8UInt][xiiGALTextureViewType::ShadingRate]         = xiiGALTextureFormat::R8UInt;
    m_ViewFormats[xiiGALTextureFormat::RG8UNormalized][xiiGALTextureViewType::ShadingRate] = xiiGALTextureFormat::RG8UNormalized;
  }

  xiiGALTextureFormat::Enum GetViewFormat(xiiGALTextureFormat::Enum format, xiiGALTextureViewType::Enum viewType, xiiUInt32 uiBindFlags)
  {
    XII_ASSERT_DEV(viewType > xiiGALTextureViewType::Undefined && viewType < xiiGALTextureViewType::ENUM_COUNT, "Unexpected texture view type.");
    XII_ASSERT_DEV(format >= xiiGALTextureFormat::Unknown && format < xiiGALTextureFormat::ENUM_COUNT, "Unknown texture format.");

    switch (format)
    {
      case xiiGALTextureFormat::R16Typeless:
      {
        if (uiBindFlags & xiiGALBindFlags::DepthStencil)
        {
          switch (viewType)
          {
            case xiiGALTextureViewType::ShaderResource:
            case xiiGALTextureViewType::RenderTarget:
            case xiiGALTextureViewType::UnorderedAccess:
              return xiiGALTextureFormat::R16UNormalized;

            case xiiGALTextureViewType::DepthStencil:
            case xiiGALTextureViewType::ReadOnlyDepthStencil:
              return xiiGALTextureFormat::D16UNormalized;

            case xiiGALTextureViewType::ShadingRate:
              return xiiGALTextureFormat::Unknown;

            default:
              XII_REPORT_FAILURE("Unexpected texture view type");
              return xiiGALTextureFormat::Unknown;
          }
          static_assert(xiiGALTextureViewType::ENUM_COUNT == 6, "Please handle the new view type in the switch above, if necessary.");
        }
        [[fallthrough]];
      }

      default: // Nothing to do.
        break;
    }

    return m_ViewFormats[format][viewType];
  }

private:
  xiiStaticArray<xiiStaticArray<xiiGALTextureFormat::Enum, xiiGALTextureViewType::ENUM_COUNT>, xiiGALTextureFormat::ENUM_COUNT> m_ViewFormats;
};

xiiEnum<xiiGALTextureFormat> xiiGALGraphicsUtilities::GetDefaultTextureViewFormat(xiiEnum<xiiGALTextureFormat> format, xiiEnum<xiiGALTextureViewType> viewType, xiiBitflags<xiiGALBindFlags> bindFlags)
{
  static TextureFormatToViewFormatConverter formatConverter;
  return formatConverter.GetViewFormat(format, viewType, bindFlags.GetValue());
}

const xiiGALSparseTextureProperties xiiGALGraphicsUtilities::GetSparseTextureProperties(xiiEnum<xiiGALTextureFormat> format, xiiEnum<xiiGALResourceDimension> dimension, xiiUInt32 uiSampleCount)
{
  /// \todo GraphicsFoundation: To be implemented.

  XII_ASSERT_NOT_IMPLEMENTED;

  return xiiGALSparseTextureProperties();
}

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_Utilities_Implementation_GraphicsUtilities);
