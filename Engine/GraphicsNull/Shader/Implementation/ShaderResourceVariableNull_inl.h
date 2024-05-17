
XII_ALWAYS_INLINE void xiiGALShaderResourceVariableNull::Set(xiiGALResource* pResource, xiiBitflags<xiiGALSetShaderResourceFlags> flags)
{
}

XII_ALWAYS_INLINE void xiiGALShaderResourceVariableNull::SetArray(xiiArrayPtr<xiiGALResource* const> ppResources, xiiBitflags<xiiGALSetShaderResourceFlags> flags)
{
}

XII_ALWAYS_INLINE void xiiGALShaderResourceVariableNull::SetBufferRange(xiiGALResource* pResource, xiiUInt64 uiOffset, xiiUInt64 uiSize, xiiUInt32 uiArrayIndex, xiiBitflags<xiiGALSetShaderResourceFlags> flags)
{
}

XII_ALWAYS_INLINE void xiiGALShaderResourceVariableNull::SetBufferOffset(xiiUInt32 uiOffset, xiiUInt32 uiArrayIndex)
{
}

XII_ALWAYS_INLINE xiiEnum<xiiGALShaderResourceVariableType> xiiGALShaderResourceVariableNull::GetType() const
{
  return xiiEnum<xiiGALShaderResourceVariableType>();
}

XII_ALWAYS_INLINE void xiiGALShaderResourceVariableNull::GetResourceDescription(xiiGALShaderResourceDescription& resourceDeccription)
{
}

XII_ALWAYS_INLINE xiiUInt32 xiiGALShaderResourceVariableNull::GetIndex() const
{
  return xiiUInt32();
}

XII_ALWAYS_INLINE xiiArrayPtr<xiiGALResource*> xiiGALShaderResourceVariableNull::Get(xiiUInt32 uiIndex) const
{
  return xiiArrayPtr<xiiGALResource*>();
}
