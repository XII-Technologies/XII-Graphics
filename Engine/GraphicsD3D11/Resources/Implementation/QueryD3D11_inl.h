
XII_ALWAYS_INLINE void xiiGALQueryD3D11::Invalidate()
{
  m_DisjointQuery.Clear();

  m_QueryState = QueryState::Inactive;
}

XII_ALWAYS_INLINE ID3D11Query* xiiGALQueryD3D11::GetQuery(xiiUInt32 uiQueryID) const
{
  XII_ASSERT_DEV(uiQueryID == 0 || (m_Description.m_Type == xiiGALQueryType::Duration && uiQueryID == 1), "");

  return m_pQueryD3D11[uiQueryID];
}

XII_ALWAYS_INLINE void xiiGALQueryD3D11::SetDisjointQuery(xiiSharedPtr<xiiDisjointQueryPool::DisjointQueryWrapper> disjointQuery)
{
  m_DisjointQuery = disjointQuery;
}
