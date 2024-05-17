
XII_ALWAYS_INLINE xiiUInt64 xiiGALCommandQueueNull::GetNextFenceValue() const
{
  return 0U;
}

XII_ALWAYS_INLINE xiiUInt64 xiiGALCommandQueueNull::GetCompletedFenceValue()
{
  return 0U;
}

XII_ALWAYS_INLINE xiiUInt64 xiiGALCommandQueueNull::WaitForIdle()
{
  return 0U;
}
