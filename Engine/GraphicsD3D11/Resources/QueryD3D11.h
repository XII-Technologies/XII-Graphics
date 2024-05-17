#pragma once

#include <GraphicsD3D11/GraphicsD3D11DLL.h>

#include <GraphicsFoundation/Resources/Query.h>

#include <GraphicsD3D11/Resources/DisjointQueryPool.h>

class XII_GRAPHICSD3D11_DLL xiiGALQueryD3D11 final : public xiiGALQuery
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALQueryD3D11, xiiGALQuery);

public:
  virtual bool GetData(void* pData, xiiUInt32 uiDataSize, bool bAutoInvalidate) override final;

  virtual void Invalidate() override final;

  ID3D11Query* GetQuery(xiiUInt32 uiQueryID) const;

  void SetDisjointQuery(xiiSharedPtr<xiiDisjointQueryPool::DisjointQueryWrapper> disjointQuery);

protected:
  friend class xiiGALDeviceD3D11;
  friend class xiiMemoryUtils;

  xiiGALQueryD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALQueryCreationDescription& creationDescription);

  virtual ~xiiGALQueryD3D11();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;

  virtual void SetDebugNamePlatform(xiiStringView sName) override final;

protected:
  ID3D11Query* m_pQueryD3D11[2] = {nullptr, nullptr};

  xiiSharedPtr<xiiDisjointQueryPool::DisjointQueryWrapper> m_DisjointQuery;
};

#include <GraphicsD3D11/Resources/Implementation/QueryD3D11_inl.h>
