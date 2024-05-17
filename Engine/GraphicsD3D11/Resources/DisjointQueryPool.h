#pragma once

#include <GraphicsD3D11/GraphicsD3D11DLL.h>

#include <Foundation/Types/SharedPtr.h>

struct ID3D11Query;

class XII_GRAPHICSD3D11_DLL xiiDisjointQueryPool final
{
public:
  struct DisjointQueryWrapper : public xiiRefCounted
  {
    DisjointQueryWrapper(xiiDisjointQueryPool& pool, ID3D11Query* pQueryD3D11) :
      m_Pool(pool), m_pQueryD3D11(pQueryD3D11)
    {
    }

    ~DisjointQueryWrapper()
    {
      m_Pool.m_AvailableQueries.PushBack(m_pQueryD3D11);
    }

    DisjointQueryWrapper(DisjointQueryWrapper&&) = default;

    DisjointQueryWrapper(const DisjointQueryWrapper&) = delete;
    DisjointQueryWrapper& operator=(const DisjointQueryWrapper&) = delete;
    DisjointQueryWrapper& operator=(DisjointQueryWrapper&&) = delete;

    xiiDisjointQueryPool& m_Pool;
    ID3D11Query*          m_pQueryD3D11 = nullptr;
    bool                  m_bIsEnded    = false;
  };

  xiiSharedPtr<DisjointQueryWrapper> GetDisjointQuery(ID3D11Device* pDeviceD3D11)
  {
    ID3D11Query* pQueryD3D11 = nullptr;

    if (!m_AvailableQueries.IsEmpty())
    {
      pQueryD3D11 = m_AvailableQueries.PeekBack();
      m_AvailableQueries.PopBack();
    }
    else
    {
      pQueryD3D11 = CreateQuery(pDeviceD3D11);
    }
    return xiiSharedPtr<DisjointQueryWrapper>(XII_DEFAULT_NEW(DisjointQueryWrapper, *this, pQueryD3D11));
  }

  ~xiiDisjointQueryPool()
  {
    XII_GAL_D3D11_RELEASE_ARRAY(m_AvailableQueries);

    xiiLog::Info("GraphicsD3D11: Disjoint query pool: created {0} {1}.", m_uiCreatedQueriesCount, (m_uiCreatedQueriesCount == 1 ? "query" : "queries"));
  }

private:
  ID3D11Query* CreateQuery(ID3D11Device* pDeviceD3D11)
  {
    D3D11_QUERY_DESC queryDescription = {D3D11_QUERY_TIMESTAMP_DISJOINT, 0};
    ID3D11Query*     pQueryD3D11      = nullptr;

    XII_ASSERT_DEV(SUCCEEDED(pDeviceD3D11->CreateQuery(&queryDescription, &pQueryD3D11)), "Failed to create D3D11 disjoint query.");
    ++m_uiCreatedQueriesCount;

    return pQueryD3D11;
  }

  xiiDynamicArray<ID3D11Query*> m_AvailableQueries;

  xiiUInt32 m_uiCreatedQueriesCount = 0;
};
