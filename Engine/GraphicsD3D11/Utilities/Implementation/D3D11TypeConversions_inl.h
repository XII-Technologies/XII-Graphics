
XII_ALWAYS_INLINE D3D11_BLEND xiiD3D11TypeConversions::GetBlendFactor(xiiEnum<xiiGALBlendFactor> e)
{
  switch (e)
  {
    case xiiGALBlendFactor::Zero:
      return D3D11_BLEND::D3D11_BLEND_ZERO;
    case xiiGALBlendFactor::One:
      return D3D11_BLEND::D3D11_BLEND_ONE;
    case xiiGALBlendFactor::SourceColor:
      return D3D11_BLEND::D3D11_BLEND_SRC_COLOR;
    case xiiGALBlendFactor::InverseSourceColor:
      return D3D11_BLEND::D3D11_BLEND_INV_SRC_COLOR;
    case xiiGALBlendFactor::SourceAlpha:
      return D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
    case xiiGALBlendFactor::InverseSourceAlpha:
      return D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
    case xiiGALBlendFactor::DestinationAlpha:
      return D3D11_BLEND::D3D11_BLEND_DEST_ALPHA;
    case xiiGALBlendFactor::InverseDestinationAlpha:
      return D3D11_BLEND::D3D11_BLEND_INV_DEST_ALPHA;
    case xiiGALBlendFactor::DestinationColor:
      return D3D11_BLEND::D3D11_BLEND_DEST_COLOR;
    case xiiGALBlendFactor::InverseDestinationColor:
      return D3D11_BLEND::D3D11_BLEND_INV_DEST_COLOR;
    case xiiGALBlendFactor::SourceAlphaSaturate:
      return D3D11_BLEND::D3D11_BLEND_SRC_ALPHA_SAT;
    case xiiGALBlendFactor::BlendFactor:
      return D3D11_BLEND::D3D11_BLEND_BLEND_FACTOR;
    case xiiGALBlendFactor::InverseBlendFactor:
      return D3D11_BLEND::D3D11_BLEND_INV_BLEND_FACTOR;
    case xiiGALBlendFactor::SourceOneColor:
      return D3D11_BLEND::D3D11_BLEND_SRC1_COLOR;
    case xiiGALBlendFactor::InverseSourceOneColor:
      return D3D11_BLEND::D3D11_BLEND_INV_SRC1_COLOR;
    case xiiGALBlendFactor::SourceOneAlpha:
      return D3D11_BLEND::D3D11_BLEND_SRC1_ALPHA;
    case xiiGALBlendFactor::InverseSourceOneAlpha:
      return D3D11_BLEND::D3D11_BLEND_INV_SRC1_ALPHA;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }

  return D3D11_BLEND::D3D11_BLEND_ZERO;
}

XII_ALWAYS_INLINE D3D11_BLEND_OP xiiD3D11TypeConversions::GetBlendOp(xiiEnum<xiiGALBlendOperation> e)
{
  switch (e)
  {
    case xiiGALBlendOperation::Add:
      return D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
    case xiiGALBlendOperation::Subtract:
      return D3D11_BLEND_OP::D3D11_BLEND_OP_SUBTRACT;
    case xiiGALBlendOperation::ReverseSubtract:
      return D3D11_BLEND_OP::D3D11_BLEND_OP_REV_SUBTRACT;
    case xiiGALBlendOperation::Min:
      return D3D11_BLEND_OP::D3D11_BLEND_OP_MIN;
    case xiiGALBlendOperation::Max:
      return D3D11_BLEND_OP::D3D11_BLEND_OP_MAX;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }

  return D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
}

XII_ALWAYS_INLINE D3D11_COMPARISON_FUNC xiiD3D11TypeConversions::GetComparisonFunc(xiiEnum<xiiGALComparisonFunction> e)
{
  switch (e)
  {
    case xiiGALComparisonFunction::Never:
      return D3D11_COMPARISON_NEVER;
    case xiiGALComparisonFunction::Less:
      return D3D11_COMPARISON_LESS;
    case xiiGALComparisonFunction::Equal:
      return D3D11_COMPARISON_EQUAL;
    case xiiGALComparisonFunction::LessEqual:
      return D3D11_COMPARISON_LESS_EQUAL;
    case xiiGALComparisonFunction::Greater:
      return D3D11_COMPARISON_GREATER;
    case xiiGALComparisonFunction::NotEqual:
      return D3D11_COMPARISON_NOT_EQUAL;
    case xiiGALComparisonFunction::GreaterEqual:
      return D3D11_COMPARISON_GREATER_EQUAL;
    case xiiGALComparisonFunction::Always:
      return D3D11_COMPARISON_ALWAYS;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }

  return D3D11_COMPARISON_NEVER;
}

XII_ALWAYS_INLINE D3D11_STENCIL_OP xiiD3D11TypeConversions::GetStencilOp(xiiEnum<xiiGALStencilOperation> e)
{
  switch (e)
  {
    case xiiGALStencilOperation::Keep:
      return D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
    case xiiGALStencilOperation::Zero:
      return D3D11_STENCIL_OP::D3D11_STENCIL_OP_ZERO;
    case xiiGALStencilOperation::Replace:
      return D3D11_STENCIL_OP::D3D11_STENCIL_OP_REPLACE;
    case xiiGALStencilOperation::IncrementSaturate:
      return D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR_SAT;
    case xiiGALStencilOperation::DecrementSaturate:
      return D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR_SAT;
    case xiiGALStencilOperation::Invert:
      return D3D11_STENCIL_OP::D3D11_STENCIL_OP_INVERT;
    case xiiGALStencilOperation::IncrementWrap:
      return D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR;
    case xiiGALStencilOperation::DecrementWrap:
      return D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }

  return D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
}

XII_ALWAYS_INLINE D3D11_FILL_MODE xiiD3D11TypeConversions::GetFillMode(xiiEnum<xiiGALFillMode> e)
{
  switch (e)
  {
    case xiiGALFillMode::Wireframe:
      return D3D11_FILL_WIREFRAME;
    case xiiGALFillMode::Solid:
      return D3D11_FILL_SOLID;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }
  return D3D11_FILL_WIREFRAME;
}

XII_ALWAYS_INLINE D3D11_CULL_MODE xiiD3D11TypeConversions::GetCullMode(xiiEnum<xiiGALCullMode> e)
{
  switch (e)
  {
    case xiiGALCullMode::None:
      return D3D11_CULL_NONE;
    case xiiGALCullMode::Front:
      return D3D11_CULL_FRONT;
    case xiiGALCullMode::Back:
      return D3D11_CULL_BACK;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }

  return D3D11_CULL_NONE;
}

XII_ALWAYS_INLINE xiiUInt8 xiiD3D11TypeConversions::GetColorWriteMask(xiiBitflags<xiiGALColorMask> e)
{
  if (e.IsNoFlagSet())
    return 0U;

  xiiUInt8 uiColorMask = 0U;
  if (e.IsSet(xiiGALColorMask::Red))
    uiColorMask |= D3D11_COLOR_WRITE_ENABLE_RED;
  if (e.IsSet(xiiGALColorMask::Green))
    uiColorMask |= D3D11_COLOR_WRITE_ENABLE_GREEN;
  if (e.IsSet(xiiGALColorMask::Blue))
    uiColorMask |= D3D11_COLOR_WRITE_ENABLE_BLUE;
  if (e.IsSet(xiiGALColorMask::Alpha))
    uiColorMask |= D3D11_COLOR_WRITE_ENABLE_ALPHA;

  return uiColorMask;
}

XII_ALWAYS_INLINE DXGI_FORMAT xiiD3D11TypeConversions::GetFormat(xiiEnum<xiiGALTextureFormat> e)
{
  switch (e)
  {
    case xiiGALTextureFormat::Unknown:
      return DXGI_FORMAT_UNKNOWN;
    case xiiGALTextureFormat::RGBA32Typeless:
      return DXGI_FORMAT_R32G32B32A32_TYPELESS;
    case xiiGALTextureFormat::RGBA32Float:
      return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case xiiGALTextureFormat::RGBA32UInt:
      return DXGI_FORMAT_R32G32B32A32_UINT;
    case xiiGALTextureFormat::RGBA32SInt:
      return DXGI_FORMAT_R32G32B32A32_SINT;
    case xiiGALTextureFormat::RGB32Typeless:
      return DXGI_FORMAT_R32G32B32_TYPELESS;
    case xiiGALTextureFormat::RGB32Float:
      return DXGI_FORMAT_R32G32B32_FLOAT;
    case xiiGALTextureFormat::RGB32UInt:
      return DXGI_FORMAT_R32G32B32_UINT;
    case xiiGALTextureFormat::RGB32SInt:
      return DXGI_FORMAT_R32G32B32_SINT;
    case xiiGALTextureFormat::RGBA16Typeless:
      return DXGI_FORMAT_R16G16B16A16_TYPELESS;
    case xiiGALTextureFormat::RGBA16Float:
      return DXGI_FORMAT_R16G16B16A16_FLOAT;
    case xiiGALTextureFormat::RGBA16UNormalized:
      return DXGI_FORMAT_R16G16B16A16_UNORM;
    case xiiGALTextureFormat::RGBA16UInt:
      return DXGI_FORMAT_R16G16B16A16_UINT;
    case xiiGALTextureFormat::RGBA16SNormalized:
      return DXGI_FORMAT_R16G16B16A16_SNORM;
    case xiiGALTextureFormat::RGBA16SInt:
      return DXGI_FORMAT_R16G16B16A16_SINT;
    case xiiGALTextureFormat::RG32Typeless:
      return DXGI_FORMAT_R32G32_TYPELESS;
    case xiiGALTextureFormat::RG32Float:
      return DXGI_FORMAT_R32G32_FLOAT;
    case xiiGALTextureFormat::RG32UInt:
      return DXGI_FORMAT_R32G32_UINT;
    case xiiGALTextureFormat::RG32SInt:
      return DXGI_FORMAT_R32G32_SINT;
    case xiiGALTextureFormat::R32G8X24Typeless:
      return DXGI_FORMAT_R32G8X24_TYPELESS;
    case xiiGALTextureFormat::D32FloatS8X24UInt:
      return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
    case xiiGALTextureFormat::R32FloatX8X24Typeless:
      return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
    case xiiGALTextureFormat::X32TypelessG8X24UInt:
      return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
    case xiiGALTextureFormat::RGB10A2Typeless:
      return DXGI_FORMAT_R10G10B10A2_TYPELESS;
    case xiiGALTextureFormat::RGB10A2UNormalized:
      return DXGI_FORMAT_R10G10B10A2_UNORM;
    case xiiGALTextureFormat::RGB10A2UInt:
      return DXGI_FORMAT_R10G10B10A2_UINT;
    case xiiGALTextureFormat::RG11B10Float:
      return DXGI_FORMAT_R11G11B10_FLOAT;
    case xiiGALTextureFormat::RGBA8Typeless:
      return DXGI_FORMAT_R8G8B8A8_TYPELESS;
    case xiiGALTextureFormat::RGBA8UNormalized:
      return DXGI_FORMAT_R8G8B8A8_UNORM;
    case xiiGALTextureFormat::RGBA8UNormalizedSRGB:
      return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    case xiiGALTextureFormat::RGBA8UInt:
      return DXGI_FORMAT_R8G8B8A8_UINT;
    case xiiGALTextureFormat::RGBA8SNormalized:
      return DXGI_FORMAT_R8G8B8A8_SNORM;
    case xiiGALTextureFormat::RGBA8SInt:
      return DXGI_FORMAT_R8G8B8A8_SINT;
    case xiiGALTextureFormat::RG16Typeless:
      return DXGI_FORMAT_R16G16_TYPELESS;
    case xiiGALTextureFormat::RG16Float:
      return DXGI_FORMAT_R16G16_FLOAT;
    case xiiGALTextureFormat::RG16UNormalized:
      return DXGI_FORMAT_R16G16_UNORM;
    case xiiGALTextureFormat::RG16UInt:
      return DXGI_FORMAT_R16G16_UINT;
    case xiiGALTextureFormat::RG16SNormalized:
      return DXGI_FORMAT_R16G16_SNORM;
    case xiiGALTextureFormat::RG16SInt:
      return DXGI_FORMAT_R16G16_SINT;
    case xiiGALTextureFormat::R32Typeless:
      return DXGI_FORMAT_R32_TYPELESS;
    case xiiGALTextureFormat::D32Float:
      return DXGI_FORMAT_D32_FLOAT;
    case xiiGALTextureFormat::R32Float:
      return DXGI_FORMAT_R32_FLOAT;
    case xiiGALTextureFormat::R32UInt:
      return DXGI_FORMAT_R32_UINT;
    case xiiGALTextureFormat::R32SInt:
      return DXGI_FORMAT_R32_SINT;
    case xiiGALTextureFormat::R24G8Typeless:
      return DXGI_FORMAT_R24G8_TYPELESS;
    case xiiGALTextureFormat::D24UNormalizedS8UInt:
      return DXGI_FORMAT_D24_UNORM_S8_UINT;
    case xiiGALTextureFormat::R24UNormalizedX8Typeless:
      return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    case xiiGALTextureFormat::X24TypelessG8UInt:
      return DXGI_FORMAT_X24_TYPELESS_G8_UINT;
    case xiiGALTextureFormat::RG8Typeless:
      return DXGI_FORMAT_R8G8_TYPELESS;
    case xiiGALTextureFormat::RG8UNormalized:
      return DXGI_FORMAT_R8G8_UNORM;
    case xiiGALTextureFormat::RG8UInt:
      return DXGI_FORMAT_R8G8_UINT;
    case xiiGALTextureFormat::RG8SNormalized:
      return DXGI_FORMAT_R8G8_SNORM;
    case xiiGALTextureFormat::RG8SInt:
      return DXGI_FORMAT_R8G8_SINT;
    case xiiGALTextureFormat::R16Typeless:
      return DXGI_FORMAT_R16_TYPELESS;
    case xiiGALTextureFormat::R16Float:
      return DXGI_FORMAT_R16_FLOAT;
    case xiiGALTextureFormat::D16UNormalized:
      return DXGI_FORMAT_D16_UNORM;
    case xiiGALTextureFormat::R16UNormalized:
      return DXGI_FORMAT_R16_UNORM;
    case xiiGALTextureFormat::R16UInt:
      return DXGI_FORMAT_R16_UINT;
    case xiiGALTextureFormat::R16SNormalized:
      return DXGI_FORMAT_R16_SNORM;
    case xiiGALTextureFormat::R16SInt:
      return DXGI_FORMAT_R16_SINT;
    case xiiGALTextureFormat::R8Typeless:
      return DXGI_FORMAT_R8_TYPELESS;
    case xiiGALTextureFormat::R8UNormalized:
      return DXGI_FORMAT_R8_UNORM;
    case xiiGALTextureFormat::R8UInt:
      return DXGI_FORMAT_R8_UINT;
    case xiiGALTextureFormat::R8SNormalized:
      return DXGI_FORMAT_R8_SNORM;
    case xiiGALTextureFormat::R8SInt:
      return DXGI_FORMAT_R8_SINT;
    case xiiGALTextureFormat::A8UNormalized:
      return DXGI_FORMAT_A8_UNORM;
    case xiiGALTextureFormat::R1UNormalized:
      return DXGI_FORMAT_R1_UNORM;
    case xiiGALTextureFormat::RGB9E5SharedExponent:
      return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
    case xiiGALTextureFormat::RG8BG8UNormalized:
      return DXGI_FORMAT_R8G8_B8G8_UNORM;
    case xiiGALTextureFormat::GR8GB8UNormalized:
      return DXGI_FORMAT_G8R8_G8B8_UNORM;
    case xiiGALTextureFormat::BC1Typeless:
      return DXGI_FORMAT_BC1_TYPELESS;
    case xiiGALTextureFormat::BC1UNormalized:
      return DXGI_FORMAT_BC1_UNORM;
    case xiiGALTextureFormat::BC1UNormalizedSRGB:
      return DXGI_FORMAT_BC1_UNORM_SRGB;
    case xiiGALTextureFormat::BC2Typeless:
      return DXGI_FORMAT_BC2_TYPELESS;
    case xiiGALTextureFormat::BC2UNormalized:
      return DXGI_FORMAT_BC2_UNORM;
    case xiiGALTextureFormat::BC2UNormalizedSRGB:
      return DXGI_FORMAT_BC2_UNORM_SRGB;
    case xiiGALTextureFormat::BC3Typeless:
      return DXGI_FORMAT_BC3_TYPELESS;
    case xiiGALTextureFormat::BC3UNormalized:
      return DXGI_FORMAT_BC3_UNORM;
    case xiiGALTextureFormat::BC3UNormalizedSRGB:
      return DXGI_FORMAT_BC3_UNORM_SRGB;
    case xiiGALTextureFormat::BC4Typeless:
      return DXGI_FORMAT_BC4_TYPELESS;
    case xiiGALTextureFormat::BC4UNormalized:
      return DXGI_FORMAT_BC4_UNORM;
    case xiiGALTextureFormat::BC4SNormalized:
      return DXGI_FORMAT_BC4_SNORM;
    case xiiGALTextureFormat::BC5Typeless:
      return DXGI_FORMAT_BC5_TYPELESS;
    case xiiGALTextureFormat::BC5UNormalized:
      return DXGI_FORMAT_BC5_UNORM;
    case xiiGALTextureFormat::BC5SNormalized:
      return DXGI_FORMAT_BC5_SNORM;
    case xiiGALTextureFormat::B5G6R5UNormalized:
      return DXGI_FORMAT_B5G6R5_UNORM;
    case xiiGALTextureFormat::B5G5R5A1UNormalized:
      return DXGI_FORMAT_B5G5R5A1_UNORM;
    case xiiGALTextureFormat::BGRA8UNormalized:
      return DXGI_FORMAT_B8G8R8A8_UNORM;
    case xiiGALTextureFormat::BGRX8UNormalized:
      return DXGI_FORMAT_B8G8R8X8_UNORM;
    case xiiGALTextureFormat::R10G10B10XRBiasA2UNormalized:
      return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
    case xiiGALTextureFormat::BGRA8Typeless:
      return DXGI_FORMAT_B8G8R8A8_TYPELESS;
    case xiiGALTextureFormat::BGRA8UNormalizedSRGB:
      return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
    case xiiGALTextureFormat::BGRX8Typeless:
      return DXGI_FORMAT_B8G8R8X8_TYPELESS;
    case xiiGALTextureFormat::BGRX8UNormalizedSRGB:
      return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
    case xiiGALTextureFormat::BC6HTypeless:
      return DXGI_FORMAT_BC6H_TYPELESS;
    case xiiGALTextureFormat::BC6HUF16:
      return DXGI_FORMAT_BC6H_UF16;
    case xiiGALTextureFormat::BC6HSF16:
      return DXGI_FORMAT_BC6H_SF16;
    case xiiGALTextureFormat::BC7Typeless:
      return DXGI_FORMAT_BC7_TYPELESS;
    case xiiGALTextureFormat::BC7UNormalized:
      return DXGI_FORMAT_BC7_UNORM;
    case xiiGALTextureFormat::BC7UNormalizedSRGB:
      return DXGI_FORMAT_BC7_UNORM_SRGB;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }
  return DXGI_FORMAT_UNKNOWN;
}

XII_ALWAYS_INLINE xiiEnum<xiiGALTextureFormat> xiiD3D11TypeConversions::GetGALFormat(DXGI_FORMAT e)
{
  switch (e)
  {
    case DXGI_FORMAT_UNKNOWN:
      return xiiGALTextureFormat::Unknown;
    case DXGI_FORMAT_R32G32B32A32_TYPELESS:
      return xiiGALTextureFormat::RGBA32Typeless;
    case DXGI_FORMAT_R32G32B32A32_FLOAT:
      return xiiGALTextureFormat::RGBA32Float;
    case DXGI_FORMAT_R32G32B32A32_UINT:
      return xiiGALTextureFormat::RGBA32UInt;
    case DXGI_FORMAT_R32G32B32A32_SINT:
      return xiiGALTextureFormat::RGBA32SInt;
    case DXGI_FORMAT_R32G32B32_TYPELESS:
      return xiiGALTextureFormat::RGB32Typeless;
    case DXGI_FORMAT_R32G32B32_FLOAT:
      return xiiGALTextureFormat::RGB32Float;
    case DXGI_FORMAT_R32G32B32_UINT:
      return xiiGALTextureFormat::RGB32UInt;
    case DXGI_FORMAT_R32G32B32_SINT:
      return xiiGALTextureFormat::RGB32SInt;
    case DXGI_FORMAT_R16G16B16A16_TYPELESS:
      return xiiGALTextureFormat::RGBA16Typeless;
    case DXGI_FORMAT_R16G16B16A16_FLOAT:
      return xiiGALTextureFormat::RGBA16Float;
    case DXGI_FORMAT_R16G16B16A16_UNORM:
      return xiiGALTextureFormat::RGBA16UNormalized;
    case DXGI_FORMAT_R16G16B16A16_UINT:
      return xiiGALTextureFormat::RGBA16UInt;
    case DXGI_FORMAT_R16G16B16A16_SNORM:
      return xiiGALTextureFormat::RGBA16SNormalized;
    case DXGI_FORMAT_R16G16B16A16_SINT:
      return xiiGALTextureFormat::RGBA16SInt;
    case DXGI_FORMAT_R32G32_TYPELESS:
      return xiiGALTextureFormat::RG32Typeless;
    case DXGI_FORMAT_R32G32_FLOAT:
      return xiiGALTextureFormat::RG32Float;
    case DXGI_FORMAT_R32G32_UINT:
      return xiiGALTextureFormat::RG32UInt;
    case DXGI_FORMAT_R32G32_SINT:
      return xiiGALTextureFormat::RG32SInt;
    case DXGI_FORMAT_R32G8X24_TYPELESS:
      return xiiGALTextureFormat::R32G8X24Typeless;
    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
      return xiiGALTextureFormat::D32FloatS8X24UInt;
    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
      return xiiGALTextureFormat::R32FloatX8X24Typeless;
    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
      return xiiGALTextureFormat::X32TypelessG8X24UInt;
    case DXGI_FORMAT_R10G10B10A2_TYPELESS:
      return xiiGALTextureFormat::RGB10A2Typeless;
    case DXGI_FORMAT_R10G10B10A2_UNORM:
      return xiiGALTextureFormat::RGB10A2UNormalized;
    case DXGI_FORMAT_R10G10B10A2_UINT:
      return xiiGALTextureFormat::RGB10A2UInt;
    case DXGI_FORMAT_R11G11B10_FLOAT:
      return xiiGALTextureFormat::RG11B10Float;
    case DXGI_FORMAT_R8G8B8A8_TYPELESS:
      return xiiGALTextureFormat::RGBA8Typeless;
    case DXGI_FORMAT_R8G8B8A8_UNORM:
      return xiiGALTextureFormat::RGBA8UNormalized;
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
      return xiiGALTextureFormat::RGBA8UNormalizedSRGB;
    case DXGI_FORMAT_R8G8B8A8_UINT:
      return xiiGALTextureFormat::RGBA8UInt;
    case DXGI_FORMAT_R8G8B8A8_SNORM:
      return xiiGALTextureFormat::RGBA8SNormalized;
    case DXGI_FORMAT_R8G8B8A8_SINT:
      return xiiGALTextureFormat::RGBA8SInt;
    case DXGI_FORMAT_R16G16_TYPELESS:
      return xiiGALTextureFormat::RG16Typeless;
    case DXGI_FORMAT_R16G16_FLOAT:
      return xiiGALTextureFormat::RG16Float;
    case DXGI_FORMAT_R16G16_UNORM:
      return xiiGALTextureFormat::RG16UNormalized;
    case DXGI_FORMAT_R16G16_UINT:
      return xiiGALTextureFormat::RG16UInt;
    case DXGI_FORMAT_R16G16_SNORM:
      return xiiGALTextureFormat::RG16SNormalized;
    case DXGI_FORMAT_R16G16_SINT:
      return xiiGALTextureFormat::RG16SInt;
    case DXGI_FORMAT_R32_TYPELESS:
      return xiiGALTextureFormat::R32Typeless;
    case DXGI_FORMAT_D32_FLOAT:
      return xiiGALTextureFormat::D32Float;
    case DXGI_FORMAT_R32_FLOAT:
      return xiiGALTextureFormat::R32Float;
    case DXGI_FORMAT_R32_UINT:
      return xiiGALTextureFormat::R32UInt;
    case DXGI_FORMAT_R32_SINT:
      return xiiGALTextureFormat::R32SInt;
    case DXGI_FORMAT_R24G8_TYPELESS:
      return xiiGALTextureFormat::R24G8Typeless;
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
      return xiiGALTextureFormat::D24UNormalizedS8UInt;
    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
      return xiiGALTextureFormat::R24UNormalizedX8Typeless;
    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
      return xiiGALTextureFormat::X24TypelessG8UInt;
    case DXGI_FORMAT_R8G8_TYPELESS:
      return xiiGALTextureFormat::RG8Typeless;
    case DXGI_FORMAT_R8G8_UNORM:
      return xiiGALTextureFormat::RG8UNormalized;
    case DXGI_FORMAT_R8G8_UINT:
      return xiiGALTextureFormat::RG8UInt;
    case DXGI_FORMAT_R8G8_SNORM:
      return xiiGALTextureFormat::RG8SNormalized;
    case DXGI_FORMAT_R8G8_SINT:
      return xiiGALTextureFormat::RG8SInt;
    case DXGI_FORMAT_R16_TYPELESS:
      return xiiGALTextureFormat::R16Typeless;
    case DXGI_FORMAT_R16_FLOAT:
      return xiiGALTextureFormat::R16Float;
    case DXGI_FORMAT_D16_UNORM:
      return xiiGALTextureFormat::D16UNormalized;
    case DXGI_FORMAT_R16_UNORM:
      return xiiGALTextureFormat::R16UNormalized;
    case DXGI_FORMAT_R16_UINT:
      return xiiGALTextureFormat::R16UInt;
    case DXGI_FORMAT_R16_SNORM:
      return xiiGALTextureFormat::R16SNormalized;
    case DXGI_FORMAT_R16_SINT:
      return xiiGALTextureFormat::R16SInt;
    case DXGI_FORMAT_R8_TYPELESS:
      return xiiGALTextureFormat::R8Typeless;
    case DXGI_FORMAT_R8_UNORM:
      return xiiGALTextureFormat::R8UNormalized;
    case DXGI_FORMAT_R8_UINT:
      return xiiGALTextureFormat::R8UInt;
    case DXGI_FORMAT_R8_SNORM:
      return xiiGALTextureFormat::R8SNormalized;
    case DXGI_FORMAT_R8_SINT:
      return xiiGALTextureFormat::R8SInt;
    case DXGI_FORMAT_A8_UNORM:
      return xiiGALTextureFormat::A8UNormalized;
    case DXGI_FORMAT_R1_UNORM:
      return xiiGALTextureFormat::R1UNormalized;
    case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
      return xiiGALTextureFormat::RGB9E5SharedExponent;
    case DXGI_FORMAT_R8G8_B8G8_UNORM:
      return xiiGALTextureFormat::RG8BG8UNormalized;
    case DXGI_FORMAT_G8R8_G8B8_UNORM:
      return xiiGALTextureFormat::GR8GB8UNormalized;
    case DXGI_FORMAT_BC1_TYPELESS:
      return xiiGALTextureFormat::BC1Typeless;
    case DXGI_FORMAT_BC1_UNORM:
      return xiiGALTextureFormat::BC1UNormalized;
    case DXGI_FORMAT_BC1_UNORM_SRGB:
      return xiiGALTextureFormat::BC1UNormalizedSRGB;
    case DXGI_FORMAT_BC2_TYPELESS:
      return xiiGALTextureFormat::BC2Typeless;
    case DXGI_FORMAT_BC2_UNORM:
      return xiiGALTextureFormat::BC2UNormalized;
    case DXGI_FORMAT_BC2_UNORM_SRGB:
      return xiiGALTextureFormat::BC2UNormalizedSRGB;
    case DXGI_FORMAT_BC3_TYPELESS:
      return xiiGALTextureFormat::BC3Typeless;
    case DXGI_FORMAT_BC3_UNORM:
      return xiiGALTextureFormat::BC3UNormalized;
    case DXGI_FORMAT_BC3_UNORM_SRGB:
      return xiiGALTextureFormat::BC3UNormalizedSRGB;
    case DXGI_FORMAT_BC4_TYPELESS:
      return xiiGALTextureFormat::BC4Typeless;
    case DXGI_FORMAT_BC4_UNORM:
      return xiiGALTextureFormat::BC4UNormalized;
    case DXGI_FORMAT_BC4_SNORM:
      return xiiGALTextureFormat::BC4SNormalized;
    case DXGI_FORMAT_BC5_TYPELESS:
      return xiiGALTextureFormat::BC5Typeless;
    case DXGI_FORMAT_BC5_UNORM:
      return xiiGALTextureFormat::BC5UNormalized;
    case DXGI_FORMAT_BC5_SNORM:
      return xiiGALTextureFormat::BC5SNormalized;
    case DXGI_FORMAT_B5G6R5_UNORM:
      return xiiGALTextureFormat::B5G6R5UNormalized;
    case DXGI_FORMAT_B5G5R5A1_UNORM:
      return xiiGALTextureFormat::B5G5R5A1UNormalized;
    case DXGI_FORMAT_B8G8R8A8_UNORM:
      return xiiGALTextureFormat::BGRA8UNormalized;
    case DXGI_FORMAT_B8G8R8X8_UNORM:
      return xiiGALTextureFormat::BGRX8UNormalized;
    case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
      return xiiGALTextureFormat::R10G10B10XRBiasA2UNormalized;
    case DXGI_FORMAT_B8G8R8A8_TYPELESS:
      return xiiGALTextureFormat::BGRA8Typeless;
    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
      return xiiGALTextureFormat::BGRA8UNormalizedSRGB;
    case DXGI_FORMAT_B8G8R8X8_TYPELESS:
      return xiiGALTextureFormat::BGRX8Typeless;
    case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
      return xiiGALTextureFormat::BGRX8UNormalizedSRGB;
    case DXGI_FORMAT_BC6H_TYPELESS:
      return xiiGALTextureFormat::BC6HTypeless;
    case DXGI_FORMAT_BC6H_UF16:
      return xiiGALTextureFormat::BC6HUF16;
    case DXGI_FORMAT_BC6H_SF16:
      return xiiGALTextureFormat::BC6HSF16;
    case DXGI_FORMAT_BC7_TYPELESS:
      return xiiGALTextureFormat::BC7Typeless;
    case DXGI_FORMAT_BC7_UNORM:
      return xiiGALTextureFormat::BC7UNormalized;
    case DXGI_FORMAT_BC7_UNORM_SRGB:
      return xiiGALTextureFormat::BC7UNormalizedSRGB;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }
  return xiiGALTextureFormat::Unknown;
}

XII_ALWAYS_INLINE DXGI_MODE_SCALING xiiD3D11TypeConversions::GetScalingMode(xiiEnum<xiiGALScalingMode> e)
{
  switch (e)
  {
    case xiiGALScalingMode::Unspecified:
      return DXGI_MODE_SCALING_UNSPECIFIED;
    case xiiGALScalingMode::Centered:
      return DXGI_MODE_SCALING_CENTERED;
    case xiiGALScalingMode::Stretched:
      return DXGI_MODE_SCALING_STRETCHED;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }
  return DXGI_MODE_SCALING_UNSPECIFIED;
}

XII_ALWAYS_INLINE xiiEnum<xiiGALScalingMode> xiiD3D11TypeConversions::GetGALScalingMode(DXGI_MODE_SCALING e)
{
  switch (e)
  {
    case DXGI_MODE_SCALING_UNSPECIFIED:
      return xiiGALScalingMode::Unspecified;
    case DXGI_MODE_SCALING_CENTERED:
      return xiiGALScalingMode::Centered;
    case DXGI_MODE_SCALING_STRETCHED:
      return xiiGALScalingMode::Stretched;
  }
  return xiiEnum<xiiGALScalingMode>();
}

XII_ALWAYS_INLINE DXGI_MODE_SCANLINE_ORDER xiiD3D11TypeConversions::GetScanLineOrder(xiiEnum<xiiGALScanLineOrder> e)
{
  switch (e)
  {
    case xiiGALScanLineOrder::Unspecified:
      return DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    case xiiGALScanLineOrder::Progressive:
      return DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
    case xiiGALScanLineOrder::UpperFieldFirst:
      return DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST;
    case xiiGALScanLineOrder::LowerFieldFirst:
      return DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }
  return DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
}

XII_ALWAYS_INLINE xiiEnum<xiiGALScanLineOrder> xiiD3D11TypeConversions::GetGALScanLineOrder(DXGI_MODE_SCANLINE_ORDER e)
{
  switch (e)
  {
    case DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED:
      return xiiGALScanLineOrder::Unspecified;
    case DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE:
      return xiiGALScanLineOrder::Progressive;
    case DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST:
      return xiiGALScanLineOrder::UpperFieldFirst;
    case DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST:
      return xiiGALScanLineOrder::LowerFieldFirst;
  }
  return xiiEnum<xiiGALScanLineOrder>();
}

XII_ALWAYS_INLINE D3D11_FILTER xiiD3D11TypeConversions::GetFilter(xiiEnum<xiiGALFilterType> minFilter, xiiEnum<xiiGALFilterType> magFilter, xiiEnum<xiiGALFilterType> mipFilter)
{
  switch (minFilter)
  {
    case xiiGALFilterType::Unknown:
    {
      xiiLog::Error("Filter type is not defined.");
    }
    break;

    // Regular filters
    case xiiGALFilterType::Point:
    {
      if (magFilter == xiiGALFilterType::Point)
      {
        if (mipFilter == xiiGALFilterType::Point)
          return D3D11_FILTER_MIN_MAG_MIP_POINT;
        else if (mipFilter == xiiGALFilterType::Linear)
          return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
      }
      else if (magFilter == xiiGALFilterType::Linear)
      {
        if (mipFilter == xiiGALFilterType::Point)
          return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
        else if (mipFilter == xiiGALFilterType::Linear)
          return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
      }
    }
    break;

    case xiiGALFilterType::Linear:
    {
      if (magFilter == xiiGALFilterType::Point)
      {
        if (mipFilter == xiiGALFilterType::Point)
          return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
        else if (mipFilter == xiiGALFilterType::Linear)
          return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
      }
      else if (magFilter == xiiGALFilterType::Linear)
      {
        if (mipFilter == xiiGALFilterType::Point)
          return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
        else if (mipFilter == xiiGALFilterType::Linear)
          return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
      }
    }
    break;

    case xiiGALFilterType::Anisotropic:
    {
      XII_ASSERT_DEV(magFilter == xiiGALFilterType::Anisotropic && mipFilter == xiiGALFilterType::Anisotropic, "For anisotropic filtering, all filters must be anisotropic.");
      return D3D11_FILTER_ANISOTROPIC;
    }

    // Comparison filters
    case xiiGALFilterType::ComparisonPoint:
    {
      if (magFilter == xiiGALFilterType::ComparisonPoint)
      {
        if (mipFilter == xiiGALFilterType::ComparisonPoint)
          return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
        else if (mipFilter == xiiGALFilterType::ComparisonLinear)
          return D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
      }
      else if (magFilter == xiiGALFilterType::ComparisonLinear)
      {
        if (mipFilter == xiiGALFilterType::ComparisonPoint)
          return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
        else if (mipFilter == xiiGALFilterType::ComparisonLinear)
          return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
      }
    }
    break;

    case xiiGALFilterType::ComparisonLinear:
    {
      if (magFilter == xiiGALFilterType::ComparisonPoint)
      {
        if (mipFilter == xiiGALFilterType::ComparisonPoint)
          return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
        else if (mipFilter == xiiGALFilterType::ComparisonLinear)
          return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
      }
      else if (magFilter == xiiGALFilterType::ComparisonLinear)
      {
        if (mipFilter == xiiGALFilterType::ComparisonPoint)
          return D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
        else if (mipFilter == xiiGALFilterType::ComparisonLinear)
          return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
      }
    }
    break;

    case xiiGALFilterType::ComparisonAnisotropic:
    {
      XII_ASSERT_DEV(magFilter == xiiGALFilterType::ComparisonAnisotropic && mipFilter == xiiGALFilterType::ComparisonAnisotropic, "For comparison anisotropic filtering, all filters must be anisotropic");
      return D3D11_FILTER_COMPARISON_ANISOTROPIC;
    }

    // Minimum filters
    case xiiGALFilterType::MinimumPoint:
    {
      if (magFilter == xiiGALFilterType::MinimumPoint)
      {
        if (mipFilter == xiiGALFilterType::MinimumPoint)
          return D3D11_FILTER_MINIMUM_MIN_MAG_MIP_POINT;
        else if (mipFilter == xiiGALFilterType::MinimumLinear)
          return D3D11_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR;
      }
      else if (magFilter == xiiGALFilterType::MinimumLinear)
      {
        if (mipFilter == xiiGALFilterType::MinimumPoint)
          return D3D11_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
        else if (mipFilter == xiiGALFilterType::MinimumLinear)
          return D3D11_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR;
      }
    }
    break;

    case xiiGALFilterType::MinimumLinear:
    {
      if (magFilter == xiiGALFilterType::MinimumPoint)
      {
        if (mipFilter == xiiGALFilterType::MinimumPoint)
          return D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT;
        else if (mipFilter == xiiGALFilterType::MinimumLinear)
          return D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
      }
      else if (magFilter == xiiGALFilterType::MinimumLinear)
      {
        if (mipFilter == xiiGALFilterType::MinimumPoint)
          return D3D11_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT;
        else if (mipFilter == xiiGALFilterType::MinimumLinear)
          return D3D11_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR;
      }
    }
    break;

    case xiiGALFilterType::MinimumAnisotropic:
    {
      XII_ASSERT_DEV(magFilter == xiiGALFilterType::MinimumAnisotropic && mipFilter == xiiGALFilterType::MinimumAnisotropic, "For minimum anisotropic filtering, all filters must be anisotropic");
      return D3D11_FILTER_MINIMUM_ANISOTROPIC;
    }

    // Maximum filters
    case xiiGALFilterType::MaximumPoint:
    {
      if (magFilter == xiiGALFilterType::MaximumPoint)
      {
        if (mipFilter == xiiGALFilterType::MaximumPoint)
          return D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT;
        else if (mipFilter == xiiGALFilterType::MaximumLinear)
          return D3D11_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR;
      }
      else if (magFilter == xiiGALFilterType::MaximumLinear)
      {
        if (mipFilter == xiiGALFilterType::MaximumPoint)
          return D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
        else if (mipFilter == xiiGALFilterType::MaximumLinear)
          return D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR;
      }
    }
    break;

    case xiiGALFilterType::MaximumLinear:
    {
      if (magFilter == xiiGALFilterType::MaximumPoint)
      {
        if (mipFilter == xiiGALFilterType::MaximumPoint)
          return D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT;
        else if (mipFilter == xiiGALFilterType::MaximumLinear)
          return D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
      }
      else if (magFilter == xiiGALFilterType::MaximumLinear)
      {
        if (mipFilter == xiiGALFilterType::MaximumPoint)
          return D3D11_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT;
        else if (mipFilter == xiiGALFilterType::MaximumLinear)
          return D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR;
      }
    }
    break;

    case xiiGALFilterType::MaximumAnisotropic:
    {
      XII_ASSERT_DEV(magFilter == xiiGALFilterType::MaximumAnisotropic && mipFilter == xiiGALFilterType::MaximumAnisotropic, "For maximum anisotropic filtering, all filters must be anisotropic");
      return D3D11_FILTER_MAXIMUM_ANISOTROPIC;
    }

    case xiiGALFilterType::ENUM_COUNT:
    {
      xiiLog::Error("This value does not define a valid filter type");
    }
    break;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }

  XII_ASSERT_DEV(false, "Unsupported filter combination");
  return D3D11_FILTER_MIN_MAG_MIP_POINT;
}

XII_ALWAYS_INLINE D3D11_TEXTURE_ADDRESS_MODE xiiD3D11TypeConversions::GetTextureAddressMode(xiiEnum<xiiGALTextureAddressMode> e)
{
  switch (e)
  {
    case xiiGALTextureAddressMode::Wrap:
      return D3D11_TEXTURE_ADDRESS_WRAP;
    case xiiGALTextureAddressMode::Mirror:
      return D3D11_TEXTURE_ADDRESS_MIRROR;
    case xiiGALTextureAddressMode::Clamp:
      return D3D11_TEXTURE_ADDRESS_CLAMP;
    case xiiGALTextureAddressMode::Border:
      return D3D11_TEXTURE_ADDRESS_BORDER;
    case xiiGALTextureAddressMode::MirrorOnce:
      return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }
  return D3D11_TEXTURE_ADDRESS_WRAP;
}

XII_ALWAYS_INLINE D3D11_QUERY xiiD3D11TypeConversions::GetQueryType(xiiEnum<xiiGALQueryType> e)
{
  switch (e)
  {
    case xiiGALQueryType::Occlusion:
      return D3D11_QUERY_OCCLUSION;
    case xiiGALQueryType::BinaryOcclusion:
      return D3D11_QUERY_OCCLUSION_PREDICATE;
    case xiiGALQueryType::Duration:
    case xiiGALQueryType::Timestamp:
      return D3D11_QUERY_TIMESTAMP;
    case xiiGALQueryType::PipelineStatistics:
      return D3D11_QUERY_PIPELINE_STATISTICS;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }
  return D3D11_QUERY_TIMESTAMP;
}

XII_ALWAYS_INLINE xiiUInt32 xiiD3D11TypeConversions::GetBindFlags(xiiBitflags<xiiGALBindFlags> e)
{
  xiiUInt32 uiBindFlags = {};

  if (e.IsSet(xiiGALBindFlags::VertexBuffer))
    uiBindFlags |= D3D11_BIND_VERTEX_BUFFER;
  if (e.IsSet(xiiGALBindFlags::IndexBuffer))
    uiBindFlags |= D3D11_BIND_INDEX_BUFFER;
  if (e.IsSet(xiiGALBindFlags::UniformBuffer))
    uiBindFlags |= D3D11_BIND_CONSTANT_BUFFER;
  if (e.IsSet(xiiGALBindFlags::ShaderResource))
    uiBindFlags |= D3D11_BIND_SHADER_RESOURCE;
  if (e.IsSet(xiiGALBindFlags::StreamOutput))
    uiBindFlags |= D3D11_BIND_STREAM_OUTPUT;
  if (e.IsSet(xiiGALBindFlags::RenderTarget))
    uiBindFlags |= D3D11_BIND_RENDER_TARGET;
  if (e.IsSet(xiiGALBindFlags::DepthStencil))
    uiBindFlags |= D3D11_BIND_DEPTH_STENCIL;
  if (e.IsSet(xiiGALBindFlags::UnorderedAccess))
    uiBindFlags |= D3D11_BIND_UNORDERED_ACCESS;

  return uiBindFlags;
}

XII_ALWAYS_INLINE xiiUInt32 xiiD3D11TypeConversions::GetCPUAccessFlags(xiiBitflags<xiiGALCPUAccessFlag> e)
{
  xiiUInt32 uiAccessFlags = {};

  if (e.IsSet(xiiGALCPUAccessFlag::Read))
    uiAccessFlags |= D3D11_CPU_ACCESS_READ;
  if (e.IsSet(xiiGALCPUAccessFlag::Write))
    uiAccessFlags |= D3D11_CPU_ACCESS_WRITE;

  return uiAccessFlags;
}

XII_ALWAYS_INLINE D3D11_USAGE xiiD3D11TypeConversions::GetUsage(xiiEnum<xiiGALResourceUsage> e)
{
  switch (e)
  {
    case xiiGALResourceUsage::Immutable:
      return D3D11_USAGE_IMMUTABLE;
    case xiiGALResourceUsage::Default:
      return D3D11_USAGE_DEFAULT;
    case xiiGALResourceUsage::Dynamic:
      return D3D11_USAGE_DYNAMIC;
    case xiiGALResourceUsage::Staging:
      return D3D11_USAGE_STAGING;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }
  return D3D11_USAGE_IMMUTABLE;
}

XII_ALWAYS_INLINE DXGI_FORMAT xiiD3D11TypeConversions::GetDXGIFormatFromType(xiiEnum<xiiGALValueType> e, xiiUInt32 uiComponentCount, bool bIsNormalized)
{
  switch (e)
  {
    case xiiGALValueType::Int8:
    {
      if (bIsNormalized)
      {
        switch (uiComponentCount)
        {
          case 1:
            return DXGI_FORMAT_R8_SNORM;
          case 2:
            return DXGI_FORMAT_R8G8_SNORM;
          case 4:
            return DXGI_FORMAT_R8G8B8A8_SNORM;

            XII_DEFAULT_CASE_NOT_IMPLEMENTED;
        }
      }
      else
      {
        switch (uiComponentCount)
        {
          case 1:
            return DXGI_FORMAT_R8_SINT;
          case 2:
            return DXGI_FORMAT_R8G8_SINT;
          case 4:
            return DXGI_FORMAT_R8G8B8A8_SINT;

            XII_DEFAULT_CASE_NOT_IMPLEMENTED;
        }
      }
    }
    break;
    case xiiGALValueType::Int16:
    {
      if (bIsNormalized)
      {
        switch (uiComponentCount)
        {
          case 1:
            return DXGI_FORMAT_R16_SNORM;
          case 2:
            return DXGI_FORMAT_R16G16_SNORM;
          case 4:
            return DXGI_FORMAT_R16G16B16A16_SNORM;

            XII_DEFAULT_CASE_NOT_IMPLEMENTED;
        }
      }
      else
      {
        switch (uiComponentCount)
        {
          case 1:
            return DXGI_FORMAT_R16_SINT;
          case 2:
            return DXGI_FORMAT_R16G16_SINT;
          case 4:
            return DXGI_FORMAT_R16G16B16A16_SINT;

            XII_DEFAULT_CASE_NOT_IMPLEMENTED;
        }
      }
    }
    break;
    case xiiGALValueType::Int32:
    {
      XII_ASSERT_DEV(bIsNormalized, "32-bit signed normalized formats are unsupported. Use xiiGALTextureFormat::R32Float instead.");

      switch (uiComponentCount)
      {
        case 1:
          return DXGI_FORMAT_R32_SINT;
        case 2:
          return DXGI_FORMAT_R32G32_SINT;
        case 3:
          return DXGI_FORMAT_R32G32B32_SINT;
        case 4:
          return DXGI_FORMAT_R32G32B32A32_SINT;

          XII_DEFAULT_CASE_NOT_IMPLEMENTED;
      }
    }
    break;
    case xiiGALValueType::UInt8:
    {
      if (bIsNormalized)
      {
        switch (uiComponentCount)
        {
          case 1:
            return DXGI_FORMAT_R8_UNORM;
          case 2:
            return DXGI_FORMAT_R8G8_UNORM;
          case 4:
            return DXGI_FORMAT_R8G8B8A8_UNORM;

            XII_DEFAULT_CASE_NOT_IMPLEMENTED;
        }
      }
      else
      {
        switch (uiComponentCount)
        {
          case 1:
            return DXGI_FORMAT_R8_UINT;
          case 2:
            return DXGI_FORMAT_R8G8_UINT;
          case 4:
            return DXGI_FORMAT_R8G8B8A8_UINT;

            XII_DEFAULT_CASE_NOT_IMPLEMENTED;
        }
      }
    }
    break;
    case xiiGALValueType::UInt16:
    {
      if (bIsNormalized)
      {
        switch (uiComponentCount)
        {
          case 1:
            return DXGI_FORMAT_R16_UNORM;
          case 2:
            return DXGI_FORMAT_R16G16_UNORM;
          case 4:
            return DXGI_FORMAT_R16G16B16A16_UNORM;

            XII_DEFAULT_CASE_NOT_IMPLEMENTED;
        }
      }
      else
      {
        switch (uiComponentCount)
        {
          case 1:
            return DXGI_FORMAT_R16_UINT;
          case 2:
            return DXGI_FORMAT_R16G16_UINT;
          case 4:
            return DXGI_FORMAT_R16G16B16A16_UINT;

            XII_DEFAULT_CASE_NOT_IMPLEMENTED;
        }
      }
    }
    break;
    case xiiGALValueType::UInt32:
    {
      XII_ASSERT_DEV(bIsNormalized, "32-bit unsigned normalized formats are unsupported. Use xiiGALTextureFormat::R32Float instead.");

      switch (uiComponentCount)
      {
        case 1:
          return DXGI_FORMAT_R32_UINT;
        case 2:
          return DXGI_FORMAT_R32G32_UINT;
        case 3:
          return DXGI_FORMAT_R32G32B32_UINT;
        case 4:
          return DXGI_FORMAT_R32G32B32A32_UINT;

          XII_DEFAULT_CASE_NOT_IMPLEMENTED;
      }
    }
    break;
    case xiiGALValueType::Float16:
    {
      XII_ASSERT_DEV(!bIsNormalized, "Floating point formats can not be normalized.");

      switch (uiComponentCount)
      {
        case 1:
          return DXGI_FORMAT_R16_FLOAT;
        case 2:
          return DXGI_FORMAT_R16G16_FLOAT;
        case 4:
          return DXGI_FORMAT_R16G16B16A16_FLOAT;

          XII_DEFAULT_CASE_NOT_IMPLEMENTED;
      }
    }
    break;
    case xiiGALValueType::Float32:
    {
      XII_ASSERT_DEV(!bIsNormalized, "Floating point formats can not be normalized.");

      switch (uiComponentCount)
      {
        case 1:
          return DXGI_FORMAT_R32_FLOAT;
        case 2:
          return DXGI_FORMAT_R32G32_FLOAT;
        case 3:
          return DXGI_FORMAT_R32G32B32_FLOAT;
        case 4:
          return DXGI_FORMAT_R32G32B32A32_FLOAT;

          XII_DEFAULT_CASE_NOT_IMPLEMENTED;
      }
    }
    break;
    case xiiGALValueType::Float64:
    {
      XII_ASSERT_NOT_IMPLEMENTED;
    }
    break;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }
  return DXGI_FORMAT_UNKNOWN;
}

XII_ALWAYS_INLINE xiiUInt32 xiiD3D11TypeConversions::GetMiscTextureFlags(xiiBitflags<xiiGALMiscTextureFlags> e)
{
  xiiUInt32 uiD3D11MiscFlags = 0;

  if (e.IsSet(xiiGALMiscTextureFlags::GenerateMips))
    uiD3D11MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;

  return uiD3D11MiscFlags;
}

XII_ALWAYS_INLINE D3D11_INPUT_CLASSIFICATION xiiD3D11TypeConversions::GetElementFrequency(xiiEnum<xiiGALInputElementFrequency> e)
{
  switch (e)
  {
    case xiiGALInputElementFrequency::PerVertex:
      return D3D11_INPUT_PER_VERTEX_DATA;
    case xiiGALInputElementFrequency::PerInstance:
      return D3D11_INPUT_PER_INSTANCE_DATA;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }
  return D3D11_INPUT_PER_VERTEX_DATA;
}

XII_ALWAYS_INLINE xiiEnum<xiiGALResourceUsage> xiiD3D11TypeConversions::GetGALUsage(D3D11_USAGE e)
{
  switch (e)
  {
    case D3D11_USAGE_DEFAULT:
      return xiiGALResourceUsage::Default;
    case D3D11_USAGE_IMMUTABLE:
      return xiiGALResourceUsage::Immutable;
    case D3D11_USAGE_DYNAMIC:
      return xiiGALResourceUsage::Dynamic;
    case D3D11_USAGE_STAGING:
      return xiiGALResourceUsage::Staging;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }
  return xiiEnum<xiiGALResourceUsage>();
}

XII_ALWAYS_INLINE xiiBitflags<xiiGALBindFlags> xiiD3D11TypeConversions::GetGALBindFlags(xiiUInt32 e)
{
  xiiBitflags<xiiGALBindFlags> bindFlags;

  if (e & D3D11_BIND_VERTEX_BUFFER)
    bindFlags |= xiiGALBindFlags::VertexBuffer;
  if (e & D3D11_BIND_INDEX_BUFFER)
    bindFlags |= xiiGALBindFlags::IndexBuffer;
  if (e & D3D11_BIND_CONSTANT_BUFFER)
    bindFlags |= xiiGALBindFlags::UniformBuffer;
  if (e & D3D11_BIND_SHADER_RESOURCE)
    bindFlags |= xiiGALBindFlags::ShaderResource;
  if (e & D3D11_BIND_STREAM_OUTPUT)
    bindFlags |= xiiGALBindFlags::StreamOutput;
  if (e & D3D11_BIND_RENDER_TARGET)
    bindFlags |= xiiGALBindFlags::RenderTarget;
  if (e & D3D11_BIND_DEPTH_STENCIL)
    bindFlags |= xiiGALBindFlags::DepthStencil;
  if (e & D3D11_BIND_UNORDERED_ACCESS)
    bindFlags |= xiiGALBindFlags::UnorderedAccess;

  return bindFlags;
}

XII_ALWAYS_INLINE xiiBitflags<xiiGALCPUAccessFlag> xiiD3D11TypeConversions::GetGALCPUAccessFlags(xiiUInt32 e)
{
  xiiBitflags<xiiGALCPUAccessFlag> cpuAccessFlags;

  if (e & D3D11_CPU_ACCESS_WRITE)
    cpuAccessFlags |= xiiGALCPUAccessFlag::Write;
  if (e & D3D11_CPU_ACCESS_READ)
    cpuAccessFlags |= xiiGALCPUAccessFlag::Read;

  return cpuAccessFlags;
}

XII_ALWAYS_INLINE xiiBitflags<xiiGALMiscTextureFlags> xiiD3D11TypeConversions::GetGALMiscTextureFlags(xiiUInt32 e)
{
  xiiBitflags<xiiGALMiscTextureFlags> miscTextureFlags;

  if (e & D3D11_RESOURCE_MISC_GENERATE_MIPS)
    miscTextureFlags |= xiiGALMiscTextureFlags::GenerateMips;

  return miscTextureFlags;
}

XII_ALWAYS_INLINE void xiiD3D11TypeConversions::GetMapTypeAndFlags(xiiEnum<xiiGALMapType> type, xiiBitflags<xiiGALMapFlags> flags, D3D11_MAP& out_mapType, xiiUInt32& out_mapFlags)
{
  out_mapType = static_cast<D3D11_MAP>(0);

  switch (type)
  {
    case xiiGALMapType::Read:
    {
      out_mapType = D3D11_MAP_READ;
    }
    break;
    case xiiGALMapType::Write:
    {
      if (flags.IsSet(xiiGALMapFlags::Discard))
      {
        out_mapType = D3D11_MAP_WRITE_DISCARD;
      }
      else if (flags.IsSet(xiiGALMapFlags::NoOverWrite))
      {
        out_mapType = D3D11_MAP_WRITE_NO_OVERWRITE;
      }
      else
      {
        out_mapType = D3D11_MAP_WRITE;
      }
    }
    break;
    case xiiGALMapType::ReadWrite:
    {
      out_mapType = D3D11_MAP_READ_WRITE;
    }
    break;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }

  out_mapFlags = 0U;
  if (flags.IsSet(xiiGALMapFlags::DoNotWait))
    out_mapFlags |= D3D11_MAP_FLAG_DO_NOT_WAIT;
}

XII_ALWAYS_INLINE D3D_PRIMITIVE_TOPOLOGY xiiD3D11TypeConversions::GetPrimitiveTopology(xiiEnum<xiiGALPrimitiveTopology> e)
{
  switch (e)
  {
    case xiiGALPrimitiveTopology::PointList:
      return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
    case xiiGALPrimitiveTopology::LineList:
      return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
    case xiiGALPrimitiveTopology::TriangleList:
      return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    case xiiGALPrimitiveTopology::TriangleStrip:
      return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
    case xiiGALPrimitiveTopology::LineStrip:
      return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
    case xiiGALPrimitiveTopology::TriangleListAdjacent:
      return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
    case xiiGALPrimitiveTopology::TriangleStripAdjacent:
      return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
    case xiiGALPrimitiveTopology::LineListAdjacent:
      return D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
    case xiiGALPrimitiveTopology::LineStripAdjacent:
      return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
    case xiiGALPrimitiveTopology::ControlPointPatchList1:
      return D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList2:
      return D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList3:
      return D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList4:
      return D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList5:
      return D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList6:
      return D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList7:
      return D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList8:
      return D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList9:
      return D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList10:
      return D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList11:
      return D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList12:
      return D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList13:
      return D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList14:
      return D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList15:
      return D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList16:
      return D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList17:
      return D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList18:
      return D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList19:
      return D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList20:
      return D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList21:
      return D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList22:
      return D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList23:
      return D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList24:
      return D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList25:
      return D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList26:
      return D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList27:
      return D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList28:
      return D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList29:
      return D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList30:
      return D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList31:
      return D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST;
    case xiiGALPrimitiveTopology::ControlPointPatchList32:
      return D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }

  return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
}
