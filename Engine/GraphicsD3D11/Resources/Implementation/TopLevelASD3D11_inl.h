
XII_ALWAYS_INLINE xiiGALTopLevelASInstanceDescription xiiGALTopLevelASD3D11::GetInstanceDescription(xiiStringView sName) const
{
  XII_ASSERT_NOT_IMPLEMENTED;

  return xiiGALTopLevelASInstanceDescription();
}

XII_ALWAYS_INLINE xiiGALTopLevelASBuildDescription xiiGALTopLevelASD3D11::GetBuildDescription() const
{
  XII_ASSERT_NOT_IMPLEMENTED;

  return xiiGALTopLevelASBuildDescription();
}

XII_ALWAYS_INLINE xiiGALScratchBufferSizeDescription xiiGALTopLevelASD3D11::GetScratchBufferSizeDescription() const
{
  XII_ASSERT_NOT_IMPLEMENTED;

  return xiiGALScratchBufferSizeDescription{.m_uiBuild = 0, .m_uiUpdate = 0};
}

XII_ALWAYS_INLINE void xiiGALTopLevelASD3D11::SetState(xiiBitflags<xiiGALResourceStateFlags> stateFlags)
{
}

XII_ALWAYS_INLINE xiiBitflags<xiiGALResourceStateFlags> xiiGALTopLevelASD3D11::GetState() const
{
  return xiiGALResourceStateFlags::Undefined;
}
