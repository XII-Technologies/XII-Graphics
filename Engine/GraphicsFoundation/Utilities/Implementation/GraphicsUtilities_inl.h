
XII_ALWAYS_INLINE xiiEnum<xiiGALGraphicsAdapterVendor> xiiGALGraphicsUtilities::GetVendorFromID(xiiUInt32 uiID)
{
  switch (uiID)
  {
    case 0x01002: // AMD
      return xiiGALGraphicsAdapterVendor::AMD;
    case 0x010DE: // NVIDIA
      return xiiGALGraphicsAdapterVendor::Nvidia;
    case 0x08086: // Intel
      return xiiGALGraphicsAdapterVendor::Intel;
    case 0x013B5: // ARM
      return xiiGALGraphicsAdapterVendor::ARM;
    case 0x05143: // Qualcomm
      return xiiGALGraphicsAdapterVendor::Qualcomm;
    case 0x01010: // Imagination Technologies
      return xiiGALGraphicsAdapterVendor::ImaginationTechnologies;
    case 0x01414: // Microsoft
      return xiiGALGraphicsAdapterVendor::Microsoft;
    case 0x0106B: // Apple
      return xiiGALGraphicsAdapterVendor::Apple;
    case 0x10005: // Mesa
      return xiiGALGraphicsAdapterVendor::Mesa;
    case 0x014E4: // Broadcom
      return xiiGALGraphicsAdapterVendor::Broadcom;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }
  return xiiGALGraphicsAdapterVendor::Unknown;
}

XII_ALWAYS_INLINE bool xiiGALGraphicsUtilities::IsIdentityComponentMapping(const xiiGALTextureComponentMapping& mapping)
{
  return ((mapping.m_R == xiiGALTextureComponentSwizzle::Identity || mapping.m_R == xiiGALTextureComponentSwizzle::R) &&
          (mapping.m_G == xiiGALTextureComponentSwizzle::Identity || mapping.m_G == xiiGALTextureComponentSwizzle::G) &&
          (mapping.m_B == xiiGALTextureComponentSwizzle::Identity || mapping.m_B == xiiGALTextureComponentSwizzle::B) &&
          (mapping.m_A == xiiGALTextureComponentSwizzle::Identity || mapping.m_A == xiiGALTextureComponentSwizzle::A));
}

XII_ALWAYS_INLINE xiiBitflags<xiiGALPipelineResourceFlags> xiiGALGraphicsUtilities::GetValidPipelineResourceFlags(xiiEnum<xiiGALShaderResourceType> type)
{
  xiiBitflags<xiiGALPipelineResourceFlags> pipelineResourceFlags = xiiGALPipelineResourceFlags::None;

  switch (type)
  {
    case xiiGALShaderResourceType::ConstantBuffer:
      pipelineResourceFlags |= xiiGALPipelineResourceFlags::NoDynamicBuffers | xiiGALPipelineResourceFlags::RuntimeArray;
      break;
    case xiiGALShaderResourceType::TextureSRV:
      pipelineResourceFlags |= xiiGALPipelineResourceFlags::CombinedSampler | xiiGALPipelineResourceFlags::RuntimeArray;
      break;
    case xiiGALShaderResourceType::BufferSRV:
      pipelineResourceFlags |= xiiGALPipelineResourceFlags::NoDynamicBuffers | xiiGALPipelineResourceFlags::Formattedbuffer | xiiGALPipelineResourceFlags::RuntimeArray;
      break;
    case xiiGALShaderResourceType::TextureUAV:
      pipelineResourceFlags |= xiiGALPipelineResourceFlags::RuntimeArray;
      break;
    case xiiGALShaderResourceType::BufferUAV:
      pipelineResourceFlags |= xiiGALPipelineResourceFlags::NoDynamicBuffers | xiiGALPipelineResourceFlags::Formattedbuffer | xiiGALPipelineResourceFlags::RuntimeArray;
      break;
    case xiiGALShaderResourceType::Sampler:
      pipelineResourceFlags |= xiiGALPipelineResourceFlags::RuntimeArray;
      break;
    case xiiGALShaderResourceType::InputAttachment:
      pipelineResourceFlags |= xiiGALPipelineResourceFlags::GeneralInputAttachment;
      break;
    case xiiGALShaderResourceType::AccelerationStructure:
      pipelineResourceFlags |= xiiGALPipelineResourceFlags::RuntimeArray;
      break;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }
  return pipelineResourceFlags;
}

XII_ALWAYS_INLINE xiiVec3U32 xiiGALGraphicsUtilities::GetMipLevelSize(xiiUInt32 uiMipLevelSize, const xiiGALTextureCreationDescription& textureDescription)
{
  xiiVec3U32 size = {textureDescription.m_Size.width, textureDescription.m_Size.height, textureDescription.m_uiArraySizeOrDepth};
  size.x          = xiiMath::Max(1U, size.x >> uiMipLevelSize);
  size.y          = xiiMath::Max(1U, size.y >> uiMipLevelSize);
  size.z          = xiiMath::Max(1U, size.z >> uiMipLevelSize);
  return size;
}

XII_ALWAYS_INLINE xiiUInt32 xiiGALGraphicsUtilities::GetMipSize(xiiUInt32 uiSize, xiiUInt32 uiMipLevel)
{
  for (xiiUInt32 i = 0; i < uiMipLevel; ++i)
  {
    uiSize = uiSize / 2;
  }
  return xiiMath::Max(1U, uiSize);
}
