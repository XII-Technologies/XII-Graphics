
XII_ALWAYS_INLINE xiiUInt32 xiiGALBottomLevelASNull::GetGeometryDescriptionIndex(xiiStringView sName) const
{
  return xiiInvalidIndex;
}

XII_ALWAYS_INLINE xiiUInt32 xiiGALBottomLevelASNull::GetGeometryIndex(xiiStringView sName) const
{
  return xiiInvalidIndex;
}

XII_ALWAYS_INLINE xiiUInt32 xiiGALBottomLevelASNull::GetActualGeometryCount() const
{
  return 0U;
}

XII_ALWAYS_INLINE xiiGALScratchBufferSizeDescription xiiGALBottomLevelASNull::GetScratchBufferSizeDescription() const
{
  return xiiGALScratchBufferSizeDescription{.m_uiBuild = 0U, .m_uiUpdate = 0U};
}

XII_ALWAYS_INLINE void xiiGALBottomLevelASNull::SetState(xiiBitflags<xiiGALResourceStateFlags> stateFlags)
{
}

XII_ALWAYS_INLINE xiiBitflags<xiiGALResourceStateFlags> xiiGALBottomLevelASNull::GetState() const
{
  return xiiGALResourceStateFlags::Unknown;
}
