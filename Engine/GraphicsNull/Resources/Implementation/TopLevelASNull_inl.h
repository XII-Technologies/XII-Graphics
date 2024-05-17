
XII_ALWAYS_INLINE xiiGALTopLevelASInstanceDescription xiiGALTopLevelASNull::GetInstanceDescription(xiiStringView sName) const
{
  return xiiGALTopLevelASInstanceDescription();
}

XII_ALWAYS_INLINE xiiGALTopLevelASBuildDescription xiiGALTopLevelASNull::GetBuildDescription() const
{
  xiiGALTopLevelASBuildDescription description;
  description.m_uiInstanceCount                    = 0U;
  description.m_uiHitGroupStride                   = 0U;
  description.m_BindingMode                        = xiiGALHitGroupBindingMode::PerGeometry;
  description.m_uiFirstContributionToHitGroupIndex = 0;
  description.m_uiLastContributionToHitGroupIndex  = 0;

  return description;
}

XII_ALWAYS_INLINE xiiGALScratchBufferSizeDescription xiiGALTopLevelASNull::GetScratchBufferSizeDescription() const
{
  return xiiGALScratchBufferSizeDescription{.m_uiBuild = 0, .m_uiUpdate = 0};
}

XII_ALWAYS_INLINE void xiiGALTopLevelASNull::SetState(xiiBitflags<xiiGALResourceStateFlags> stateFlags)
{
}

XII_ALWAYS_INLINE xiiBitflags<xiiGALResourceStateFlags> xiiGALTopLevelASNull::GetState() const
{
  return xiiGALResourceStateFlags::Unknown;
}
