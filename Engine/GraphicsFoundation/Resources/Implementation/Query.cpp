#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/Resources/Query.h>

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALQuery, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;

xiiGALQuery::xiiGALQuery(xiiGALDevice* pDevice, const xiiGALQueryCreationDescription& creationDescription) :
  xiiGALDeviceObject(pDevice), m_Description(creationDescription)
{
}

xiiGALQuery::~xiiGALQuery() = default;

void xiiGALQuery::CheckQueryDataPtr(void* pData, xiiUInt32 uiDataSize)
{
  XII_ASSERT_DEV(m_QueryState == QueryState::Ended, "Attempting to get data of query '{0}' that has not been ended.");

  if (pData != nullptr)
  {
    switch (m_Description.m_Type)
    {
      case xiiGALQueryType::Occlusion:
      {
        XII_ASSERT_DEV(uiDataSize == sizeof(xiiGALQueryDataOcclusion), "The size of query data ({0}) is incorrect: ({1}) is expected.", sizeof(xiiGALQueryDataOcclusion));
      };
      break;
      case xiiGALQueryType::BinaryOcclusion:
      {
        XII_ASSERT_DEV(uiDataSize == sizeof(xiiGALQueryDataBinaryOcclusion), "The size of query data ({0}) is incorrect: ({1}) is expected.", sizeof(xiiGALQueryDataBinaryOcclusion));
      };
      break;
      case xiiGALQueryType::Timestamp:
      {
        XII_ASSERT_DEV(uiDataSize == sizeof(xiiGALQueryDataTimestamp), "The size of query data ({0}) is incorrect: ({1}) is expected.", sizeof(xiiGALQueryDataTimestamp));
      };
      break;
      case xiiGALQueryType::PipelineStatistics:
      {
        XII_ASSERT_DEV(uiDataSize == sizeof(xiiGALQueryDataPipelineStatistics), "The size of query data ({0}) is incorrect: ({1}) is expected.", sizeof(xiiGALQueryDataPipelineStatistics));
      };
      break;
      case xiiGALQueryType::Duration:
      {
        XII_ASSERT_DEV(uiDataSize == sizeof(xiiGALQueryDataDuration), "The size of query data ({0}) is incorrect: ({1}) is expected.", sizeof(xiiGALQueryDataDuration));
      };
      break;

        XII_DEFAULT_CASE_NOT_IMPLEMENTED;
    }
  }
}

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_Resources_Implementation_Query);
