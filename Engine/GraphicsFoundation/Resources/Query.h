#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Declarations/DeviceObject.h>
#include <GraphicsFoundation/Declarations/GraphicsTypes.h>

/// \brief This describes the occlusion query data.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALQueryDataOcclusion : public xiiHashableStruct<xiiGALQueryDataOcclusion>
{
  XII_DECLARE_POD_TYPE();

  xiiEnum<xiiGALQueryType> m_Type         = xiiGALQueryType::Occlusion; ///< Query type.
  xiiUInt64                m_uiNumSamples = 0U;                         ///< The number of samples that passed the depth and stencil tests in between begin / end query.
};

/// \brief This describes the binary occlusion query data.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALQueryDataBinaryOcclusion : public xiiHashableStruct<xiiGALQueryDataBinaryOcclusion>
{
  XII_DECLARE_POD_TYPE();

  xiiEnum<xiiGALQueryType> m_Type              = xiiGALQueryType::BinaryOcclusion; ///< Query type.
  bool                     m_bAnySamplesPassed = false;                            ///< Indicates if at least one sample passed depth and stencil testing in between begin / end query.
};

/// \brief This describes the timestamp query data.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALQueryDataTimestamp : public xiiHashableStruct<xiiGALQueryDataTimestamp>
{
  XII_DECLARE_POD_TYPE();

  xiiEnum<xiiGALQueryType> m_Type        = xiiGALQueryType::Timestamp; ///< Query type.
  xiiUInt64                m_uiCounter   = 0U;                         ///< The value of a high-frequency counter.
  xiiUInt64                m_uiFrequency = 0U;                         ///< The counter frequency, in Hz (ticks/second). If there was an error while getting the timestamp, this value will be 0.
};

/// \brief This describes the pipeline statistics query data.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALQueryDataPipelineStatistics : public xiiHashableStruct<xiiGALQueryDataPipelineStatistics>
{
  XII_DECLARE_POD_TYPE();

  xiiEnum<xiiGALQueryType> m_Type                  = xiiGALQueryType::PipelineStatistics; ///< Query type.
  xiiUInt64                m_uiInputVertices       = 0U;                                  ///< Number of vertices processed by the input assembler stage.
  xiiUInt64                m_uiInputPrimitives     = 0U;                                  ///< Number of primitives processed by the input assembler stage.
  xiiUInt64                m_uiGSPrimitives        = 0U;                                  ///< Number of primitives output by a geometry shader.
  xiiUInt64                m_uiClippingInvocations = 0U;                                  ///< Number of primitives that were sent to the clipping stage.
  xiiUInt64                m_uiClippingPrimitives  = 0U;                                  ///< Number of primitives that were output by the clipping stage and were rendered. This may be larger or smaller than the clipping invocations because after a primitive is clipped sometimes it is either broken up into more than one primitive or completely culled.
  xiiUInt64                m_uiVSInvocations       = 0U;                                  ///< Number of times a vertex shader was invoked.
  xiiUInt64                m_uiGSInvocations       = 0U;                                  ///< Number of times a geometry shader was invoked.
  xiiUInt64                m_uiPSInvocations       = 0U;                                  ///< Number of times a pixel shader was invoked.
  xiiUInt64                m_uiHSInvocations       = 0U;                                  ///< Number of times a hull shader was invoked.
  xiiUInt64                m_uiDSInvocations       = 0U;                                  ///< Number of times a domain shader was invoked.
  xiiUInt64                m_uiCSInvocations       = 0U;                                  ///< Number of times a compute shader was invoked.
};

/// \brief This describes the duration query data.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALQueryDataDuration : public xiiHashableStruct<xiiGALQueryDataDuration>
{
  XII_DECLARE_POD_TYPE();

  xiiEnum<xiiGALQueryType> m_Type        = xiiGALQueryType::Duration; ///< Query type.
  xiiUInt64                m_uiDuration  = 0U;                        ///< The number of high-frequency counter ticks between begin / end query.
  xiiUInt64                m_uiFrequency = 0U;                        ///< The counter frequency, in Hz (ticks/second). If there was an error while getting the timestamp, this value will be 0.
};

/// \brief This describes the query creation description.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALQueryCreationDescription : public xiiHashableStruct<xiiGALQueryCreationDescription>
{
  XII_DECLARE_POD_TYPE();

  xiiEnum<xiiGALQueryType> m_Type = xiiGALQueryType::Undefined; ///< Query type.
};

/// \brief Interface that defines methods to manipulate a query object.
class XII_GRAPHICSFOUNDATION_DLL xiiGALQuery : public xiiGALDeviceObject
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALQuery, xiiGALDeviceObject);

public:
  /// \brief This returns the creation description for this object.
  XII_NODISCARD const xiiGALQueryCreationDescription& GetDescription() const;

  /// \brief This retrieves the query data.
  ///
  /// \param pData           - The pointer to the query data structure. This must be a pointer to one of Occlusion, BinaryOcclusion, Timestamp, PipelineStatistics, and Duration structures. An application may provide nullptr to only check the query status.
  /// \param uiDataSize      - The size of the data structure.
  /// \param bAutoInvalidate - Whether to invalidate the query if the results are available and release associated resources. An application should typically always invalidate completed queries unless it needs to retrieve the same data through GetData() multiple times. A query will not be invalidated if pData is nullptr.
  ///
  /// \return True if the query data is available, false otherwise.
  ///
  /// \note  In Direct3D11 backend timestamp queries will only be available after FinishFrame is called for the frame in which they were collected. If AutoInvalidate is set to true, and the data have been retrieved, an application must not call GetData() until it begins and ends the query again.
  XII_NODISCARD virtual bool GetData(void* pData, xiiUInt32 uiDataSize, bool bAutoInvalidate = true) = 0;

  /// \brief This invalidates the query and releases the associated resources.
  XII_NODISCARD virtual void Invalidate() = 0;

protected:
  friend class xiiGALDevice;
  friend class xiiGALCommandList;

  xiiGALQuery(xiiGALDevice* pDevice, const xiiGALQueryCreationDescription& creationDescription);

  virtual ~xiiGALQuery();

  virtual xiiResult InitPlatform() = 0;

  virtual xiiResult DeInitPlatform() = 0;

  void CheckQueryDataPtr(void* pData, xiiUInt32 uiDataSize);

protected:
  enum class QueryState
  {
    Inactive,
    Querying,
    Ended
  };

  xiiGALQueryCreationDescription m_Description;

  QueryState m_QueryState = QueryState::Inactive;
};

#include <GraphicsFoundation/Resources/Implementation/Query_inl.h>
