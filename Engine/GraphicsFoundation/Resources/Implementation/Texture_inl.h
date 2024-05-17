
XII_ALWAYS_INLINE const xiiGALTextureCreationDescription& xiiGALTexture::GetDescription() const
{
  return m_Description;
}

XII_ALWAYS_INLINE xiiGALTextureViewHandle xiiGALTexture::GetDefaultView(xiiEnum<xiiGALTextureViewType> viewType)
{
  XII_ASSERT_DEV(viewType > xiiGALTextureViewType::Undefined && viewType < xiiGALTextureViewType::ENUM_COUNT, "Invalid view type.");

  XII_ASSERT_DEV(!m_DefaultTextureViews[viewType.GetValue()].IsInvalidated(), "Texture view handle is invalid!");

  return m_DefaultTextureViews[viewType.GetValue()];
}

XII_FORCE_INLINE bool xiiGALTextureCreationDescription::IsArray() const
{
  return m_Type == xiiGALResourceDimension::Texture1DArray || m_Type == xiiGALResourceDimension::Texture2DArray || m_Type == xiiGALResourceDimension::TextureCube || m_Type == xiiGALResourceDimension::TextureCubeArray;
}

XII_FORCE_INLINE bool xiiGALTextureCreationDescription::Is1D() const
{
  return m_Type == xiiGALResourceDimension::Texture1D || m_Type == xiiGALResourceDimension::Texture1DArray;
}

XII_FORCE_INLINE bool xiiGALTextureCreationDescription::Is2D() const
{
  return m_Type == xiiGALResourceDimension::Texture2D || m_Type == xiiGALResourceDimension::Texture2DArray || m_Type == xiiGALResourceDimension::TextureCube || m_Type == xiiGALResourceDimension::TextureCubeArray;
}

XII_FORCE_INLINE bool xiiGALTextureCreationDescription::Is3D() const
{
  return m_Type == xiiGALResourceDimension::Texture3D;
}

XII_FORCE_INLINE bool xiiGALTextureCreationDescription::IsCube() const
{
  return m_Type == xiiGALResourceDimension::TextureCube || m_Type == xiiGALResourceDimension::TextureCubeArray;
};

XII_FORCE_INLINE xiiUInt32 xiiGALTextureCreationDescription::GetArraySize() const
{
  return IsArray() ? m_uiArraySizeOrDepth : 1U;
};
