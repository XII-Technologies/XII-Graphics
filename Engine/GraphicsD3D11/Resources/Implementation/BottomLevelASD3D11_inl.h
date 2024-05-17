
XII_ALWAYS_INLINE xiiUInt32 xiiGALBottomLevelASD3D11::GetGeometryDescriptionIndex(xiiStringView sName) const
{
  XII_ASSERT_NOT_IMPLEMENTED;
  return xiiInvalidIndex;
}

XII_ALWAYS_INLINE xiiUInt32 xiiGALBottomLevelASD3D11::GetGeometryIndex(xiiStringView sName) const
{
  XII_ASSERT_NOT_IMPLEMENTED;
  return xiiInvalidIndex;
}

XII_ALWAYS_INLINE xiiUInt32 xiiGALBottomLevelASD3D11::GetActualGeometryCount() const
{
  return 0U;
}

XII_ALWAYS_INLINE xiiGALScratchBufferSizeDescription xiiGALBottomLevelASD3D11::GetScratchBufferSizeDescription() const
{
  XII_ASSERT_NOT_IMPLEMENTED;
  return xiiGALScratchBufferSizeDescription{.m_uiBuild = 0, .m_uiUpdate = 0};
}

XII_ALWAYS_INLINE void xiiGALBottomLevelASD3D11::SetState(xiiBitflags<xiiGALResourceStateFlags> stateFlags)
{
}

XII_ALWAYS_INLINE xiiBitflags<xiiGALResourceStateFlags> xiiGALBottomLevelASD3D11::GetState() const
{
  return xiiGALResourceStateFlags::Undefined;
}
