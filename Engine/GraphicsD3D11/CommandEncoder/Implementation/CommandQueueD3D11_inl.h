
XII_ALWAYS_INLINE xiiUInt64 xiiGALCommandQueueD3D11::GetNextFenceValue() const
{
  return m_NextFenceValue.load();
}
