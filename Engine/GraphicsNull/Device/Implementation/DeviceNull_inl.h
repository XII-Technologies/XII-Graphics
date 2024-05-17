
XII_ALWAYS_INLINE xiiGALCommandQueue* xiiGALDeviceNull::GetDefaultCommandQueue(xiiBitflags<xiiGALCommandQueueType> queueType) const
{
  return m_pDefaultQueue.Borrow();
}
