
XII_ALWAYS_INLINE float xiiGALNormalizedDeviceCoordinates::GetZtoDepthBias() const
{
  return -m_fMinZ * m_fZToDepthScale;
}

XII_ALWAYS_INLINE xiiUInt32 xiiGALTextureFormatDescription::GetElementSize() const
{
  return m_uiComponentSize * (m_ComponentType != xiiGALTextureFormatComponentType::Compressed ? m_uiComponentCount : 1);
}
