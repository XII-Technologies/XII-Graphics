
XII_ALWAYS_INLINE const xiiGALCommandListCreationDescription& xiiGALCommandList::GetDescription() const
{
  return m_Description;
}

XII_ALWAYS_INLINE void xiiGALCommandList::AssertRenderingThread() const
{
  XII_ASSERT_DEV(xiiThreadUtils::IsMainThread(), "This function may only be executed on the main thread.");
}

XII_ALWAYS_INLINE xiiGALCommandList::RecordingState xiiGALCommandList::GetRecordingState() const
{
  return m_RecordingState;
}

XII_ALWAYS_INLINE void xiiGALCommandList::CountDispatchCall()
{
  ++m_uiDispatchCalls;
}

XII_ALWAYS_INLINE void xiiGALCommandList::CountDrawCall()
{
  ++m_uiDrawCalls;
}

XII_ALWAYS_INLINE void xiiGALCommandList::ClearStatisticCounters()
{
  m_uiDrawCalls     = 0U;
  m_uiDispatchCalls = 0U;
}
