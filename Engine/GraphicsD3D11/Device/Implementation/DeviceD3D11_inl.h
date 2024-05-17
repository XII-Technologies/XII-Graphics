
XII_ALWAYS_INLINE xiiGALCommandQueue* xiiGALDeviceD3D11::GetDefaultCommandQueue(xiiBitflags<xiiGALCommandQueueType> queueType) const
{
  return m_CommandQueues[GetCommandQueueIndex(queueType)].Borrow();
}

XII_ALWAYS_INLINE ID3D11Device5* xiiGALDeviceD3D11::GetD3D11Device() const
{
  return m_pDeviceD3D11;
}

XII_ALWAYS_INLINE IDXGIAdapter4* xiiGALDeviceD3D11::GetDXGIAdapter() const
{
  return m_pDXGIAdapter;
}

XII_ALWAYS_INLINE IDXGIFactory5* xiiGALDeviceD3D11::GetDXGIFactory() const
{
  return m_pDXGIFactory;
}

XII_ALWAYS_INLINE ID3D11DeviceContext4* xiiGALDeviceD3D11::GetImmediateContext()
{
  return m_pDeviceContext;
}

XII_ALWAYS_INLINE xiiUInt32 xiiGALDeviceD3D11::GetCommandQueueIndex(xiiBitflags<xiiGALCommandQueueType> queueType) const
{
  if (queueType.IsSet(xiiGALCommandQueueType::Graphics))
    return 0U;

  if (queueType.IsSet(xiiGALCommandQueueType::Compute))
    return 1U;

  if (queueType.IsSet(xiiGALCommandQueueType::Transfer))
    return 2U;

  if (queueType.IsSet(xiiGALCommandQueueType::SparseBinding))
    return 3U;

  XII_ASSERT_DEV(false, "Unknown command queue type.");

  return 0U;
}
