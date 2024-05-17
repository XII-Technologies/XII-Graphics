
XII_ALWAYS_INLINE bool xiiGALQueryNull::GetData(void* pData, xiiUInt32 uiDataSize, bool bAutoInvalidate)
{
  switch (m_Description.m_Type)
  {
    case xiiGALQueryType::Occlusion:
    {
      xiiGALQueryDataOcclusion& occlusionData = *reinterpret_cast<xiiGALQueryDataOcclusion*>(pData);
      occlusionData.m_Type                    = xiiGALQueryType::Occlusion;
      occlusionData.m_uiNumSamples            = 0U;

      return true;
    }
    case xiiGALQueryType::BinaryOcclusion:
    {
      xiiGALQueryDataBinaryOcclusion& binaryOcclusionData = *reinterpret_cast<xiiGALQueryDataBinaryOcclusion*>(pData);
      binaryOcclusionData.m_Type                          = xiiGALQueryType::BinaryOcclusion;
      binaryOcclusionData.m_bAnySamplesPassed             = false;

      return true;
    }
    case xiiGALQueryType::Timestamp:
    {
      xiiGALQueryDataTimestamp& timestampData = *reinterpret_cast<xiiGALQueryDataTimestamp*>(pData);
      timestampData.m_Type                    = xiiGALQueryType::Timestamp;
      timestampData.m_uiCounter               = 0U;
      timestampData.m_uiFrequency             = 0U;

      return true;
    }
    case xiiGALQueryType::PipelineStatistics:
    {
      xiiGALQueryDataPipelineStatistics& pipelineStatistics = *reinterpret_cast<xiiGALQueryDataPipelineStatistics*>(pData);
      pipelineStatistics.m_Type                             = xiiGALQueryType::PipelineStatistics;
      pipelineStatistics.m_uiInputVertices                  = 0U;
      pipelineStatistics.m_uiInputPrimitives                = 0U;
      pipelineStatistics.m_uiGSPrimitives                   = 0U;
      pipelineStatistics.m_uiClippingInvocations            = 0U;
      pipelineStatistics.m_uiClippingPrimitives             = 0U;
      pipelineStatistics.m_uiVSInvocations                  = 0U;
      pipelineStatistics.m_uiGSInvocations                  = 0U;
      pipelineStatistics.m_uiPSInvocations                  = 0U;
      pipelineStatistics.m_uiHSInvocations                  = 0U;
      pipelineStatistics.m_uiDSInvocations                  = 0U;
      pipelineStatistics.m_uiCSInvocations                  = 0U;

      return true;
    }
    case xiiGALQueryType::Duration:
    {
      xiiGALQueryDataDuration& durationData = *reinterpret_cast<xiiGALQueryDataDuration*>(pData);
      durationData.m_Type                   = xiiGALQueryType::Duration;
      durationData.m_uiDuration             = 0U;
      durationData.m_uiFrequency            = 0U;

      return true;
    }

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }
  return false;
}

XII_ALWAYS_INLINE void xiiGALQueryNull::Invalidate()
{
}
