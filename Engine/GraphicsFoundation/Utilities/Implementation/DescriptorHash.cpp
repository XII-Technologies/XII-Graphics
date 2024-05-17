#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <Foundation/Algorithm/HashStream.h>
#include <GraphicsFoundation/States/PipelineResourceSignature.h>
#include <GraphicsFoundation/States/PipelineState.h>
#include <GraphicsFoundation/Utilities/DescriptorHash.h>

namespace
{
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALSwapChainHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALBottomLevelASHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALBufferHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALBufferViewHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALFenceHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALFramebufferHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALQueryHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALRenderPassHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALSamplerHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALTextureHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALTextureViewHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALTopLevelASHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALInputLayoutHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALShaderHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALShaderBindingTableHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALShaderByteCodeHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALShaderResourceBindingHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALBlendStateHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALDepthStencilStateHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALPipelineResourceSignatureHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALPipelineStateHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
  xiiStreamWriter& operator<<(xiiStreamWriter& ref_stream, const xiiGALRasterizerStateHandle& value)
  {
    ref_stream << reinterpret_cast<const xiiUInt32&>(value);
    return ref_stream;
  }
} // namespace

xiiUInt32 xiiGALDescriptorHash::Hash(const xiiGALPipelineStateCreationDescription& description)
{
  xiiHashStreamWriter32 writer;

  writer << description.m_PipelineType;
  writer << description.m_hShader;
  writer << description.m_uiNodeMask;
  writer << description.m_uiImmediateContextMask;

  // Graphics pipeline state.
  switch (description.m_PipelineType)
  {
    case xiiGALPipelineType::Graphics:
    case xiiGALPipelineType::Mesh:
    {
      const auto& graphicsPipeline = description.m_GraphicsPipeline;

      writer << graphicsPipeline.m_hBlendState;
      writer << graphicsPipeline.m_uiSampleMask;
      writer << graphicsPipeline.m_hRasterizerState;
      writer << graphicsPipeline.m_hDepthStencilState;
      writer << graphicsPipeline.m_hInputLayout;
      writer << graphicsPipeline.m_PrimitiveTopology;
      writer << graphicsPipeline.m_uiViewportCount;
      writer << graphicsPipeline.m_uiSubpassIndex;
      writer << graphicsPipeline.m_ShadingRateFlags;
      writer << graphicsPipeline.m_hRenderPass;
    }
    break;
    case xiiGALPipelineType::Compute:
    {
    }
    break;
    case xiiGALPipelineType::RayTracing:
    {
      const auto& rayTracingPipeline = description.m_RayTracingPipeline;

      writer << rayTracingPipeline.m_uiShaderRecordSize;
      writer << rayTracingPipeline.m_uiMaxRecursionDepth;
    }
    break;
    case xiiGALPipelineType::Tile:
    {
      const auto& tilePipeline = description.m_TilePipeline;

      writer << tilePipeline.m_SampleCount;
      writer << tilePipeline.m_RenderTargetFormats.GetCount();

      for (xiiUInt32 i = 0; i < tilePipeline.m_RenderTargetFormats.GetCount(); ++i)
      {
        writer << tilePipeline.m_RenderTargetFormats[i];
      }
    }
    break;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }

  return writer.GetHashValue();
}

bool xiiGALDescriptorHash::Equal(const xiiGALPipelineStateCreationDescription& a, const xiiGALPipelineStateCreationDescription& b)
{
  return false;
}

xiiUInt32 xiiGALDescriptorHash::Hash(const xiiGALPipelineResourceSignatureCreationDescription& description)
{
  xiiHashStreamWriter32 writer;

  writer << description.m_uiBindingIndex;
  writer << description.m_bUseCombinedTextureSamplers;
  writer << description.m_sCombinedSamplerSuffix;

  writer << description.m_Resources.GetCount();
  for (xiiUInt32 i = 0; i < description.m_Resources.GetCount(); ++i)
  {
    const auto& resource = description.m_Resources[i];

    writer << i;
    writer << resource.m_sName;
    writer << resource.m_ShaderStages;
    writer << resource.m_uiArraySize;
    writer << resource.m_ResourceType;
    writer << resource.m_ResourceVariableType;
    writer << resource.m_PipelineResourceFlags;
  }

  writer << description.m_ImmutableSamplers.GetCount();
  for (xiiUInt32 i = 0; i < description.m_ImmutableSamplers.GetCount(); ++i)
  {
    const auto& sampler = description.m_ImmutableSamplers[i];

    writer << i;
    writer << sampler.m_SamplerOrTextureName;
    writer << sampler.m_ShaderStages;
    writer << sampler.m_SamplerDescription.CalculateHash();
  }

  return writer.GetHashValue();
}

bool xiiGALDescriptorHash::Equal(const xiiGALPipelineResourceSignatureCreationDescription& a, const xiiGALPipelineResourceSignatureCreationDescription& b)
{
  return false;
}

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_Utilities_Implementation_DescriptorHash);
