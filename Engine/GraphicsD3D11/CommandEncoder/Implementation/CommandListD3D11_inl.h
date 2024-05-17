
XII_ALWAYS_INLINE ID3D11CommandList* xiiGALCommandListD3D11::GetD3D11CommandList() const
{
  return m_pSubmittedCommandList;
}

XII_ALWAYS_INLINE ID3D11DeviceContext1* xiiGALCommandListD3D11::GetD3D11DeferredContext() const
{
  return m_pCommandList;
}
