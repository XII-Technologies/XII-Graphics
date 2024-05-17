
XII_ALWAYS_INLINE void xiiGALTextureNull::SetState(xiiBitflags<xiiGALResourceStateFlags> stateFlags)
{
}

XII_ALWAYS_INLINE xiiBitflags<xiiGALResourceStateFlags> xiiGALTextureNull::GetState() const
{
  return xiiGALResourceStateFlags::Unknown;
}

XII_ALWAYS_INLINE const xiiGALSparseTextureProperties& xiiGALTextureNull::GetSparseProperties() const
{
  static xiiGALSparseTextureProperties temporary;

  return temporary;
}
