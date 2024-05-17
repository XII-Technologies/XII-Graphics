
XII_ALWAYS_INLINE void xiiGALBufferNull::SetState(xiiBitflags<xiiGALResourceStateFlags> stateFlags)
{
}

XII_ALWAYS_INLINE xiiBitflags<xiiGALResourceStateFlags> xiiGALBufferNull::GetState() const
{
  return xiiGALResourceStateFlags::Unknown;
}

XII_ALWAYS_INLINE xiiGALMemoryProperties xiiGALBufferNull::GetMemoryProperties() const
{
  return xiiGALMemoryProperties();
}

XII_ALWAYS_INLINE void xiiGALBufferNull::FlushMappedRange(xiiUInt64 uiStartOffset, xiiUInt64 uiSize)
{
}

XII_ALWAYS_INLINE void xiiGALBufferNull::InvalidateMappedRange(xiiUInt64 uiStartOffset, xiiUInt64 uiSize)
{
}

XII_ALWAYS_INLINE xiiGALSparseBufferProperties xiiGALBufferNull::GetSparseProperties() const
{
  return xiiGALSparseBufferProperties();
}
