#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <GraphicsD3D11/Device/DeviceD3D11.h>
#include <GraphicsD3D11/Resources/QueryD3D11.h>

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALQueryD3D11, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALQueryD3D11::xiiGALQueryD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALQueryCreationDescription& creationDescription) :
  xiiGALQuery(pDeviceD3D11, creationDescription)
{
}

xiiGALQueryD3D11::~xiiGALQueryD3D11() = default;

xiiResult xiiGALQueryD3D11::InitPlatform()
{
  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);

  D3D11_QUERY_DESC queryDescription = {};
  queryDescription.Query            = xiiD3D11TypeConversions::GetQueryType(m_Description.m_Type);

  for (xiiUInt32 i = 0; i < (m_Description.m_Type == xiiGALQueryType::Duration ? 2U : 1U); ++i)
  {
    if (FAILED(pDeviceD3D11->GetD3D11Device()->CreateQuery(&queryDescription, &m_pQueryD3D11[i])))
    {
      xiiLog::Error("Failed to create D3D11 query object.");
      return XII_FAILURE;
    }
  }
  return XII_SUCCESS;
}

xiiResult xiiGALQueryD3D11::DeInitPlatform()
{
  m_DisjointQuery.Clear();

  XII_GAL_D3D11_RELEASE_ARRAY(m_pQueryD3D11);

  return XII_SUCCESS;
}

void xiiGALQueryD3D11::SetDebugNamePlatform(xiiStringView sName)
{
  for (auto pQuery : m_pQueryD3D11)
  {
    if (pQuery != nullptr)
    {
      xiiStringBuilder sb;
      if (FAILED(pQuery->SetPrivateData(WKPDID_D3DDebugObjectName, sName.GetElementCount(), sName.GetData(sb))))
      {
        xiiLog::Error("Failed to set the Direct3D11 query debug name.");
      }
    }
  }
}

bool xiiGALQueryD3D11::GetData(void* pData, xiiUInt32 uiDataSize, bool bAutoInvalidate)
{
  CheckQueryDataPtr(pData, uiDataSize);

  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);

  bool bIsDataAvailable = false;
  switch (m_Description.m_Type)
  {
    case xiiGALQueryType::Occlusion:
    {
      xiiUInt64 uiSampleCount;
      bIsDataAvailable = pDeviceD3D11->GetImmediateContext()->GetData(m_pQueryD3D11[0], &uiSampleCount, sizeof(uiSampleCount), 0) == S_OK;
      if (bIsDataAvailable && pData != nullptr)
      {
        xiiGALQueryDataOcclusion& occlusionData = *reinterpret_cast<xiiGALQueryDataOcclusion*>(pData);
        occlusionData.m_Type                    = xiiGALQueryType::Occlusion;
        occlusionData.m_uiNumSamples            = uiSampleCount;
      }
    }
    break;
    case xiiGALQueryType::BinaryOcclusion:
    {
      BOOL bAnySamplePassed;
      bIsDataAvailable = pDeviceD3D11->GetImmediateContext()->GetData(m_pQueryD3D11[0], &bAnySamplePassed, sizeof(bAnySamplePassed), 0) == S_OK;
      if (bIsDataAvailable && pData != nullptr)
      {
        xiiGALQueryDataBinaryOcclusion& binaryOcclusionData = *reinterpret_cast<xiiGALQueryDataBinaryOcclusion*>(pData);
        binaryOcclusionData.m_Type                          = xiiGALQueryType::BinaryOcclusion;
        binaryOcclusionData.m_bAnySamplesPassed             = bAnySamplePassed != FALSE;
      }
    }
    break;
    case xiiGALQueryType::Timestamp:
    {
      // Timestamp query is only useful if two timestamp queries are done in the middle of a D3D11_QUERY_TIMESTAMP_DISJOINT query.
      // Timestamp disjoint query is begun by the device context when the first timestamp query is begun and ended
      // by FinishFrame. Thus timestamp queries will only become available after FinishFrame.

      XII_ASSERT_DEV(m_DisjointQuery != nullptr, "");

      bIsDataAvailable = m_DisjointQuery->m_bIsEnded;
      if (bIsDataAvailable)
      {
        xiiUInt64 uiCounter = 0;
        bIsDataAvailable    = pDeviceD3D11->GetImmediateContext()->GetData(m_pQueryD3D11[0], &uiCounter, sizeof(uiCounter), 0) == S_OK;

        // Note: DataReady is a return value, so we query the counter first, and then check pData for null.
        if (bIsDataAvailable && pData != nullptr)
        {
          D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjointQueryData;
          bIsDataAvailable = pDeviceD3D11->GetImmediateContext()->GetData(m_DisjointQuery->m_pQueryD3D11, &disjointQueryData, sizeof(disjointQueryData), 0) == S_OK;

          if (bIsDataAvailable)
          {
            // The timestamp returned by ID3D11DeviceContext::GetData for a timestamp query is only reliable if Disjoint is FALSE.
            xiiGALQueryDataTimestamp& timestampData = *reinterpret_cast<xiiGALQueryDataTimestamp*>(pData);
            timestampData.m_Type                    = xiiGALQueryType::Timestamp;
            timestampData.m_uiCounter               = uiCounter;
            timestampData.m_uiFrequency             = disjointQueryData.Disjoint ? 0 : disjointQueryData.Frequency;
          }
        }
      }
    }
    break;
    case xiiGALQueryType::PipelineStatistics:
    {
      D3D11_QUERY_DATA_PIPELINE_STATISTICS pipelineStatisticQueryData;
      bIsDataAvailable = pDeviceD3D11->GetImmediateContext()->GetData(m_pQueryD3D11[0], &pipelineStatisticQueryData, sizeof(pipelineStatisticQueryData), 0) == S_OK;
      if (bIsDataAvailable && pData != nullptr)
      {
        xiiGALQueryDataPipelineStatistics& pipelineStatistics = *reinterpret_cast<xiiGALQueryDataPipelineStatistics*>(pData);
        pipelineStatistics.m_Type                             = xiiGALQueryType::PipelineStatistics;
        pipelineStatistics.m_uiInputVertices                  = pipelineStatisticQueryData.IAVertices;
        pipelineStatistics.m_uiInputPrimitives                = pipelineStatisticQueryData.IAPrimitives;
        pipelineStatistics.m_uiGSPrimitives                   = pipelineStatisticQueryData.GSPrimitives;
        pipelineStatistics.m_uiClippingInvocations            = pipelineStatisticQueryData.CInvocations;
        pipelineStatistics.m_uiClippingPrimitives             = pipelineStatisticQueryData.CPrimitives;
        pipelineStatistics.m_uiVSInvocations                  = pipelineStatisticQueryData.VSInvocations;
        pipelineStatistics.m_uiGSInvocations                  = pipelineStatisticQueryData.GSInvocations;
        pipelineStatistics.m_uiPSInvocations                  = pipelineStatisticQueryData.PSInvocations;
        pipelineStatistics.m_uiHSInvocations                  = pipelineStatisticQueryData.HSInvocations;
        pipelineStatistics.m_uiDSInvocations                  = pipelineStatisticQueryData.DSInvocations;
        pipelineStatistics.m_uiCSInvocations                  = pipelineStatisticQueryData.CSInvocations;
      }
    }
    break;
    case xiiGALQueryType::Duration:
    {
      // Timestamp query is only useful if two timestamp queries are done in the middle of a D3D11_QUERY_TIMESTAMP_DISJOINT query.
      // Timestamp disjoint query is begun by the device context when the first timestamp query is begun and ended
      // by FinishFrame. Thus timestamp queries will only become available after FinishFrame.

      XII_ASSERT_DEV(m_DisjointQuery != nullptr, "");

      bIsDataAvailable = m_DisjointQuery->m_bIsEnded;
      if (bIsDataAvailable)
      {
        xiiUInt64 uiStartCounter = 0;
        xiiUInt64 uiEndCounter   = 0;
        bIsDataAvailable         = pDeviceD3D11->GetImmediateContext()->GetData(m_pQueryD3D11[0], &uiStartCounter, sizeof(uiStartCounter), 0) == S_OK;

        if (bIsDataAvailable)
        {
          bIsDataAvailable = pDeviceD3D11->GetImmediateContext()->GetData(m_pQueryD3D11[1], &uiEndCounter, sizeof(uiEndCounter), 0) == S_OK;

          // Note: DataReady is a return value, so we query the counter first, and then check pData for null.
          if (bIsDataAvailable && pData != nullptr)
          {
            D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjointQueryData;
            bIsDataAvailable = pDeviceD3D11->GetImmediateContext()->GetData(m_DisjointQuery->m_pQueryD3D11, &disjointQueryData, sizeof(disjointQueryData), 0) == S_OK;

            if (bIsDataAvailable)
            {
              // The timestamp returned by ID3D11DeviceContext::GetData for a timestamp query is only reliable if Disjoint is FALSE.

              XII_ASSERT_DEV(uiEndCounter >= uiStartCounter, "");

              xiiGALQueryDataDuration& durationData = *reinterpret_cast<xiiGALQueryDataDuration*>(pData);
              durationData.m_Type                   = xiiGALQueryType::Duration;
              durationData.m_uiDuration             = uiEndCounter - uiStartCounter;
              durationData.m_uiFrequency            = disjointQueryData.Disjoint ? 0 : disjointQueryData.Frequency;
            }
          }
        }
      }
    }
    break;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }

  if (bIsDataAvailable && pData != nullptr && bAutoInvalidate)
  {
    Invalidate();
  }
  return bIsDataAvailable;
}

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_Resources_Implementation_QueryD3D11);
