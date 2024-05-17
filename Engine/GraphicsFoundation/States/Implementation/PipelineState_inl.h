
XII_ALWAYS_INLINE bool xiiGALPipelineStateCreationDescription::IsAnyGraphicsPipeline() const
{
  return m_PipelineType == xiiGALPipelineType::Graphics || m_PipelineType == xiiGALPipelineType::Mesh;
}

XII_ALWAYS_INLINE bool xiiGALPipelineStateCreationDescription::IsComputePipeline() const
{
  return m_PipelineType == xiiGALPipelineType::Compute;
}

XII_ALWAYS_INLINE bool xiiGALPipelineStateCreationDescription::IsRayTracingPipeline() const
{
  return m_PipelineType == xiiGALPipelineType::RayTracing;
}

XII_ALWAYS_INLINE bool xiiGALPipelineStateCreationDescription::IsTilePipeline() const
{
  return m_PipelineType == xiiGALPipelineType::Tile;
}

XII_ALWAYS_INLINE const xiiGALPipelineStateCreationDescription& xiiGALPipelineState::GetDescription() const
{
  return m_Description;
}
