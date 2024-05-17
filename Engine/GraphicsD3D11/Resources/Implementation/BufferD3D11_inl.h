
XII_ALWAYS_INLINE void xiiGALBufferD3D11::SetState(xiiBitflags<xiiGALResourceStateFlags> stateFlags)
{
}

XII_ALWAYS_INLINE xiiBitflags<xiiGALResourceStateFlags> xiiGALBufferD3D11::GetState() const
{
  return xiiGALResourceStateFlags::Undefined;
}

XII_ALWAYS_INLINE xiiGALMemoryProperties xiiGALBufferD3D11::GetMemoryProperties() const
{
  /// \todo GraphicsD3D11: Not yet implemented.

  XII_ASSERT_NOT_IMPLEMENTED;

  return xiiGALMemoryProperties();
}

XII_ALWAYS_INLINE void xiiGALBufferD3D11::FlushMappedRange(xiiUInt64 uiStartOffset, xiiUInt64 uiSize)
{
}

XII_ALWAYS_INLINE void xiiGALBufferD3D11::InvalidateMappedRange(xiiUInt64 uiStartOffset, xiiUInt64 uiSize)
{
}

XII_ALWAYS_INLINE ID3D11Buffer* xiiGALBufferD3D11::GetBuffer() const
{
  return m_pBuffer;
}

XII_ALWAYS_INLINE xiiEnum<xiiGALValueType> xiiGALBufferD3D11::GetIndexFormat() const
{
  return m_IndexFormat;
}
