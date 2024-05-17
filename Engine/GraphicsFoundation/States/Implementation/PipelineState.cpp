#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/Device/Device.h>
#include <GraphicsFoundation/Resources/Buffer.h>
#include <GraphicsFoundation/Resources/Sampler.h>
#include <GraphicsFoundation/Resources/Texture.h>
#include <GraphicsFoundation/States/PipelineResourceSignature.h>
#include <GraphicsFoundation/States/PipelineState.h>

// clang-format off

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALShaderVariableFlags, 1)
  XII_ENUM_CONSTANT(xiiGALShaderVariableFlags::None),
  XII_ENUM_CONSTANT(xiiGALShaderVariableFlags::NoDynamicBuffers),
  XII_ENUM_CONSTANT(xiiGALShaderVariableFlags::InputAttachment),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALPipelineShadingRateFlags, 1)
  XII_ENUM_CONSTANT(xiiGALPipelineShadingRateFlags::None),
  XII_ENUM_CONSTANT(xiiGALPipelineShadingRateFlags::PerPrimitive),
  XII_ENUM_CONSTANT(xiiGALPipelineShadingRateFlags::TextureBased),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALPipelineType, 1)
  XII_ENUM_CONSTANT(xiiGALPipelineType::Graphics),
  XII_ENUM_CONSTANT(xiiGALPipelineType::Compute),
  XII_ENUM_CONSTANT(xiiGALPipelineType::Mesh),
  XII_ENUM_CONSTANT(xiiGALPipelineType::RayTracing),
  XII_ENUM_CONSTANT(xiiGALPipelineType::Tile),
  XII_ENUM_CONSTANT(xiiGALPipelineType::Invalid),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALPipelineState, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;

// clang-format on

xiiGALPipelineState::xiiGALPipelineState(xiiGALDevice* pDevice, const xiiGALPipelineStateCreationDescription& creationDescription) :
  xiiGALDeviceObject(pDevice), m_Description(creationDescription)
{
}

xiiGALPipelineState::~xiiGALPipelineState() = default;

void xiiGALPipelineState::SetConstantBuffer(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALBufferHandle hConstantBuffer)
{
  // Check that the pipeline resource signature contains the binding information at the required shader stage.
  bool bResourceFound = false;
  {
    const xiiGALPipelineResourceSignature* pResourceSignature   = m_pDevice->GetPipelineResourceSignature(m_Description.m_hPipelineResourceSignature);
    const auto&                            signatureDescription = pResourceSignature->GetDescription();

    for (const auto& resource : signatureDescription.m_Resources)
    {
      if (resource.m_sName == bindingInformation.m_sName && resource.m_ResourceType == xiiGALShaderResourceType::ConstantBuffer && resource.m_ShaderStages.AreAllSet(bindingInformation.m_ShaderStages))
      {
        bResourceFound = true;
        break;
      }
    }
  }

  if (!bResourceFound)
  {
    xiiLog::Error("The constant buffer resource '{0}' with the required shader stages does not exist in the pipeline resource signature.", bindingInformation.m_sName);
    return;
  }

  xiiGALBuffer* pBuffer = m_pDevice->GetBuffer(hConstantBuffer);

  XII_ASSERT_DEV(pBuffer == nullptr || pBuffer->GetDescription().m_BindFlags.IsSet(xiiGALBindFlags::UniformBuffer), "Incorrect buffer bind flags. The buffer must be created with xiiGALBindFlags::UniformBuffer if not invalidated");

  SetConstantBufferPlatform(bindingInformation, pBuffer);
}

void xiiGALPipelineState::SetShaderResourceBufferView(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALBufferViewHandle hBufferView)
{
  // Check that the pipeline resource signature contains the binding information at the required shader stage.
  bool bResourceFound = false;
  {
    const xiiGALPipelineResourceSignature* pResourceSignature   = m_pDevice->GetPipelineResourceSignature(m_Description.m_hPipelineResourceSignature);
    const auto&                            signatureDescription = pResourceSignature->GetDescription();

    for (const auto& resource : signatureDescription.m_Resources)
    {
      if (resource.m_sName == bindingInformation.m_sName && resource.m_ResourceType == xiiGALShaderResourceType::BufferSRV && resource.m_ShaderStages.AreAllSet(bindingInformation.m_ShaderStages))
      {
        bResourceFound = true;
        break;
      }
    }
  }

  if (!bResourceFound)
  {
    xiiLog::Error("The buffer resource view '{0}' with the required shader stages does not exist in the pipeline resource signature.", bindingInformation.m_sName);
    return;
  }

  xiiGALBufferView* pBufferView = m_pDevice->GetBufferView(hBufferView);

  XII_ASSERT_DEV(pBufferView == nullptr || pBufferView->GetDescription().m_ViewType == xiiGALBufferViewType::ShaderResource, "Incorrect buffer view type. The view must be created with xiiGALBufferViewType::ShaderResource if not invalidated.");

  SetShaderResourceBufferViewPlatform(bindingInformation, pBufferView);
}

void xiiGALPipelineState::SetShaderResourceTextureView(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALTextureViewHandle hTextureView)
{
  // Check that the pipeline resource signature contains the binding information at the required shader stage.
  bool bResourceFound = false;
  {
    const xiiGALPipelineResourceSignature* pResourceSignature   = m_pDevice->GetPipelineResourceSignature(m_Description.m_hPipelineResourceSignature);
    const auto&                            signatureDescription = pResourceSignature->GetDescription();

    for (const auto& resource : signatureDescription.m_Resources)
    {
      if (resource.m_sName == bindingInformation.m_sName && resource.m_ResourceType == xiiGALShaderResourceType::TextureSRV && resource.m_ShaderStages.AreAllSet(bindingInformation.m_ShaderStages))
      {
        bResourceFound = true;
        break;
      }
    }
  }

  if (!bResourceFound)
  {
    xiiLog::Error("The texture resource view '{0}' with the required shader stages does not exist in the pipeline resource signature.", bindingInformation.m_sName);
    return;
  }

  xiiGALTextureView* pTextureView = m_pDevice->GetTextureView(hTextureView);

  XII_ASSERT_DEV(pTextureView == nullptr || pTextureView->GetDescription().m_ViewType == xiiGALTextureViewType::ShaderResource, "Incorrect texture view type. The view must be created with xiiGALTextureViewType::ShaderResource if not invalidated.");

  SetShaderResourceTextureViewPlatform(bindingInformation, pTextureView);
}

void xiiGALPipelineState::SetUnorderedAccessBufferView(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALBufferViewHandle hBufferView)
{
  // Check that the pipeline resource signature contains the binding information at the required shader stage.
  bool bResourceFound = false;
  {
    const xiiGALPipelineResourceSignature* pResourceSignature   = m_pDevice->GetPipelineResourceSignature(m_Description.m_hPipelineResourceSignature);
    const auto&                            signatureDescription = pResourceSignature->GetDescription();

    for (const auto& resource : signatureDescription.m_Resources)
    {
      if (resource.m_sName == bindingInformation.m_sName && resource.m_ResourceType == xiiGALShaderResourceType::BufferUAV && resource.m_ShaderStages.AreAllSet(bindingInformation.m_ShaderStages))
      {
        bResourceFound = true;
        break;
      }
    }
  }

  if (!bResourceFound)
  {
    xiiLog::Error("The unordered access buffer view '{0}' with the required shader stages does not exist in the pipeline resource signature.", bindingInformation.m_sName);
    return;
  }

  xiiGALBufferView* pBufferView = m_pDevice->GetBufferView(hBufferView);

  XII_ASSERT_DEV(pBufferView == nullptr || pBufferView->GetDescription().m_ViewType == xiiGALBufferViewType::UnorderedAccess, "Incorrect buffer view type. The view must be created with xiiGALBufferViewType::UnorderedAccess if not invalidated.");

  SetUnorderedAccessBufferViewPlatform(bindingInformation, pBufferView);
}

void xiiGALPipelineState::SetUnorderedAccessTextureView(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALTextureViewHandle hTextureView)
{
  // Check that the pipeline resource signature contains the binding information at the required shader stage.
  bool bResourceFound = false;
  {
    const xiiGALPipelineResourceSignature* pResourceSignature   = m_pDevice->GetPipelineResourceSignature(m_Description.m_hPipelineResourceSignature);
    const auto&                            signatureDescription = pResourceSignature->GetDescription();

    for (const auto& resource : signatureDescription.m_Resources)
    {
      if (resource.m_sName == bindingInformation.m_sName && resource.m_ResourceType == xiiGALShaderResourceType::TextureUAV && resource.m_ShaderStages.AreAllSet(bindingInformation.m_ShaderStages))
      {
        bResourceFound = true;
        break;
      }
    }
  }

  if (!bResourceFound)
  {
    xiiLog::Error("The unordered access texture view '{0}' with the required shader stages does not exist in the pipeline resource signature.", bindingInformation.m_sName);
    return;
  }

  xiiGALTextureView* pTextureView = m_pDevice->GetTextureView(hTextureView);

  XII_ASSERT_DEV(pTextureView == nullptr || pTextureView->GetDescription().m_ViewType == xiiGALTextureViewType::UnorderedAccess, "Incorrect texture view type. The view must be created with xiiGALTextureViewType::UnorderedAccess if not invalidated.");

  SetUnorderedAccessTextureViewPlatform(bindingInformation, pTextureView);
}

void xiiGALPipelineState::SetSampler(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALSamplerHandle hSampler)
{
  // Check that the pipeline resource signature contains the binding information at the required shader stage.
  bool bResourceFound = false;
  {
    const xiiGALPipelineResourceSignature* pResourceSignature   = m_pDevice->GetPipelineResourceSignature(m_Description.m_hPipelineResourceSignature);
    const auto&                            signatureDescription = pResourceSignature->GetDescription();

    for (const auto& resource : signatureDescription.m_Resources)
    {
      if (resource.m_sName == bindingInformation.m_sName && resource.m_ResourceType == xiiGALShaderResourceType::Sampler && resource.m_ShaderStages.AreAllSet(bindingInformation.m_ShaderStages))
      {
        bResourceFound = true;
        break;
      }
    }
  }

  if (!bResourceFound)
  {
    xiiLog::Error("The sampler resource '{0}' with the required shader stages does not exist in the pipeline resource signature.", bindingInformation.m_sName);
    return;
  }

  xiiGALSampler* pSampler = m_pDevice->GetSampler(hSampler);

  SetSamplerPlatform(bindingInformation, pSampler);
}

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_States_Implementation_PipelineState);
