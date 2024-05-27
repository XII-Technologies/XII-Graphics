#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <Foundation/Containers/IterateBits.h>

#include <GraphicsD3D11/CommandEncoder/CommandListD3D11.h>
#include <GraphicsD3D11/Resources/BufferD3D11.h>
#include <GraphicsD3D11/Resources/BufferViewD3D11.h>
#include <GraphicsD3D11/Resources/RenderPassD3D11.h>
#include <GraphicsD3D11/Resources/SamplerD3D11.h>
#include <GraphicsD3D11/Resources/TextureD3D11.h>
#include <GraphicsD3D11/Resources/TextureViewD3D11.h>
#include <GraphicsD3D11/Shader/InputLayoutD3D11.h>
#include <GraphicsD3D11/Shader/ShaderD3D11.h>
#include <GraphicsD3D11/States/PipelineResourceSignatureD3D11.h>
#include <GraphicsD3D11/States/PipelineStateD3D11.h>

#include <GraphicsD3D11/Device/DeviceD3D11.h>

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALPipelineStateD3D11, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALPipelineStateD3D11::xiiGALPipelineStateD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALPipelineStateCreationDescription& creationDescription) :
  xiiGALPipelineState(pDeviceD3D11, creationDescription)
{
}

xiiGALPipelineStateD3D11::~xiiGALPipelineStateD3D11() = default;

xiiResult xiiGALPipelineStateD3D11::InitPlatform()
{
  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);

  xiiGALShaderD3D11* pShaderD3D11 = static_cast<xiiGALShaderD3D11*>(pDeviceD3D11->GetShader(m_Description.m_hShader));
  if (m_Description.IsAnyGraphicsPipeline() && pShaderD3D11->GetVertexShader() == nullptr)
  {
    xiiLog::Error("The given shader has an invalidated vertex shader!");
    return XII_FAILURE;
  }
  m_pShaderD3D11 = pShaderD3D11;

  m_pPipelineResourceSignatureD3D11 = static_cast<xiiGALPipelineResourceSignatureD3D11*>(pDeviceD3D11->GetPipelineResourceSignature(m_Description.m_hPipelineResourceSignature));

  switch (m_Description.m_PipelineType)
  {
    case xiiGALPipelineType::Graphics:
    case xiiGALPipelineType::Mesh:
    {
      m_pRenderPassD3D11        = static_cast<xiiGALRenderPassD3D11*>(pDeviceD3D11->GetRenderPass(m_Description.m_GraphicsPipeline.m_hRenderPass));
      m_pBlendStateD3D11        = static_cast<xiiGALBlendStateD3D11*>(pDeviceD3D11->GetBlendState(m_Description.m_GraphicsPipeline.m_hBlendState));
      m_pInputLayoutD3D11       = static_cast<xiiGALInputLayoutD3D11*>(pDeviceD3D11->GetInputLayout(m_Description.m_GraphicsPipeline.m_hInputLayout));
      m_pRasterizerStateD3D11   = static_cast<xiiGALRasterizerStateD3D11*>(pDeviceD3D11->GetRasterizerState(m_Description.m_GraphicsPipeline.m_hRasterizerState));
      m_pDepthStencilStateD3D11 = static_cast<xiiGALDepthStencilStateD3D11*>(pDeviceD3D11->GetDepthStencilState(m_Description.m_GraphicsPipeline.m_hDepthStencilState));

#define SET_BOUND_SHADER(ShaderStage) m_pBoundShaders[xiiGALShaderStage::GetStageIndex(xiiGALShaderStage::##ShaderStage)] = XII_CONCAT(XII_CONCAT(pShaderD3D11->Get, ##ShaderStage), Shader())

      SET_BOUND_SHADER(Vertex);
      SET_BOUND_SHADER(Pixel);
      SET_BOUND_SHADER(Geometry);
      SET_BOUND_SHADER(Hull);
      SET_BOUND_SHADER(Domain);

#undef SET_BOUND_SHADER
    }
    break;
    case xiiGALPipelineType::Compute:
    {
#define SET_BOUND_SHADER(ShaderStage) m_pBoundShaders[xiiGALShaderStage::GetStageIndex(xiiGALShaderStage::##ShaderStage)] = XII_CONCAT(XII_CONCAT(pShaderD3D11->Get, ##ShaderStage), Shader())

      SET_BOUND_SHADER(Compute);

#undef SET_BOUND_SHADER
    }
    break;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }

  return XII_SUCCESS;
}

xiiResult xiiGALPipelineStateD3D11::DeInitPlatform()
{
  return XII_SUCCESS;
}

void xiiGALPipelineStateD3D11::SetConstantBufferPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALBuffer* pConstantBuffer)
{
  XII_ASSERT_RELEASE(bindingInformation.m_uiBindSet == 0, "In D3D11, Shader resources use a single descriptor set.");
  XII_ASSERT_RELEASE(bindingInformation.m_uiBindSlot < XII_GAL_MAX_CONSTANT_BUFFER_COUNT, "Constant buffer bind slot ({0}) must be in the range [0, {1})!", bindingInformation.m_uiBindSlot, XII_GAL_MAX_CONSTANT_BUFFER_COUNT);

  ID3D11Buffer* pD3D11Buffer = pConstantBuffer != nullptr ? static_cast<xiiGALBufferD3D11*>(pConstantBuffer)->GetBuffer() : nullptr;

  xiiStaticBitfield32 stageBitfield = xiiStaticBitfield32::FromMask(bindingInformation.m_ShaderStages.GetValue());
  for (auto it = stageBitfield.GetIterator(); it.IsValid(); ++it)
  {
    xiiUInt32 uiStageIndex = it.Value();

    if (m_pBoundConstantBuffers[uiStageIndex][bindingInformation.m_uiBindSlot] != pD3D11Buffer)
    {
      m_pBoundConstantBuffers[uiStageIndex][bindingInformation.m_uiBindSlot] = pD3D11Buffer;
      m_BoundConstantBuffersRange[uiStageIndex].SetToIncludeValue(bindingInformation.m_uiBindSlot);
    }
  }
}

void xiiGALPipelineStateD3D11::SetShaderResourceBufferViewPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALBufferView* pBufferView)
{
  if (pBufferView != nullptr && UnsetUnorderedAccessViews(pBufferView->GetBuffer()))
  {
    // Flush context.
  }

  ID3D11ShaderResourceView* pD3D11ShaderResourceView = pBufferView != nullptr ? static_cast<ID3D11ShaderResourceView*>(static_cast<xiiGALBufferViewD3D11*>(pBufferView)->GetBufferView()) : nullptr;

  xiiStaticBitfield32 stageBitfield = xiiStaticBitfield32::FromMask(bindingInformation.m_ShaderStages.GetValue());
  for (auto it = stageBitfield.GetIterator(); it.IsValid(); ++it)
  {
    xiiUInt32 uiStageIndex = it.Value();

    auto& boundShaderResourceViews = m_pBoundShaderResourceViews[uiStageIndex];
    boundShaderResourceViews.EnsureCount(bindingInformation.m_uiBindSlot + 1);

    auto& resourcesForResourceViews = m_ResourcesForResourceViews[uiStageIndex];
    resourcesForResourceViews.EnsureCount(bindingInformation.m_uiBindSlot + 1);

    if (boundShaderResourceViews[bindingInformation.m_uiBindSlot] != pD3D11ShaderResourceView)
    {
      boundShaderResourceViews[bindingInformation.m_uiBindSlot]  = pD3D11ShaderResourceView;
      resourcesForResourceViews[bindingInformation.m_uiBindSlot] = pBufferView != nullptr ? pBufferView->GetBuffer() : nullptr;
      m_BoundShaderResourceViewsRange[uiStageIndex].SetToIncludeValue(bindingInformation.m_uiBindSlot);
    }
  }
}

void xiiGALPipelineStateD3D11::SetShaderResourceTextureViewPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALTextureView* pTextureView)
{
  if (pTextureView != nullptr && UnsetUnorderedAccessViews(pTextureView->GetTexture()))
  {
    // Flush context.
  }

  ID3D11ShaderResourceView* pD3D11ShaderResourceView = pTextureView != nullptr ? static_cast<ID3D11ShaderResourceView*>(static_cast<xiiGALTextureViewD3D11*>(pTextureView)->GetTextureView()) : nullptr;

  xiiStaticBitfield32 stageBitfield = xiiStaticBitfield32::FromMask(bindingInformation.m_ShaderStages.GetValue());
  for (auto it = stageBitfield.GetIterator(); it.IsValid(); ++it)
  {
    xiiUInt32 uiStageIndex = it.Value();

    auto& boundShaderResourceViews = m_pBoundShaderResourceViews[uiStageIndex];
    boundShaderResourceViews.EnsureCount(bindingInformation.m_uiBindSlot + 1);

    auto& resourcesForResourceViews = m_ResourcesForResourceViews[uiStageIndex];
    resourcesForResourceViews.EnsureCount(bindingInformation.m_uiBindSlot + 1);

    if (boundShaderResourceViews[bindingInformation.m_uiBindSlot] != pD3D11ShaderResourceView)
    {
      boundShaderResourceViews[bindingInformation.m_uiBindSlot]  = pD3D11ShaderResourceView;
      resourcesForResourceViews[bindingInformation.m_uiBindSlot] = pTextureView != nullptr ? pTextureView->GetTexture() : nullptr;
      m_BoundShaderResourceViewsRange[uiStageIndex].SetToIncludeValue(bindingInformation.m_uiBindSlot);
    }
  }
}

void xiiGALPipelineStateD3D11::SetUnorderedAccessBufferViewPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALBufferView* pBufferView)
{
  if (pBufferView && UnsetResourceViews(pBufferView->GetBuffer()))
  {
    // Flush context.
  }

  ID3D11UnorderedAccessView* pUnorderedAccessViewD3D11 = pBufferView != nullptr ? static_cast<ID3D11UnorderedAccessView*>(static_cast<xiiGALBufferViewD3D11*>(pBufferView)->GetBufferView()) : nullptr;

  m_BoundUnoderedAccessViews.EnsureCount(bindingInformation.m_uiBindSlot + 1);
  m_ResourcesForUnorderedAccessViews.EnsureCount(bindingInformation.m_uiBindSlot + 1);

  if (m_BoundUnoderedAccessViews[bindingInformation.m_uiBindSlot] != pUnorderedAccessViewD3D11)
  {
    m_BoundUnoderedAccessViews[bindingInformation.m_uiBindSlot]         = pUnorderedAccessViewD3D11;
    m_ResourcesForUnorderedAccessViews[bindingInformation.m_uiBindSlot] = pBufferView != nullptr ? pBufferView->GetBuffer() : nullptr;
    m_BoundUnoderedAccessViewsRange.SetToIncludeValue(bindingInformation.m_uiBindSlot);
  }
}

void xiiGALPipelineStateD3D11::SetUnorderedAccessTextureViewPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALTextureView* pTextureView)
{
  if (pTextureView && UnsetResourceViews(pTextureView->GetTexture()))
  {
    // Flush context.
  }

  ID3D11UnorderedAccessView* pUnorderedAccessViewD3D11 = pTextureView != nullptr ? static_cast<ID3D11UnorderedAccessView*>(static_cast<xiiGALTextureViewD3D11*>(pTextureView)->GetTextureView()) : nullptr;

  m_BoundUnoderedAccessViews.EnsureCount(bindingInformation.m_uiBindSlot + 1);
  m_ResourcesForUnorderedAccessViews.EnsureCount(bindingInformation.m_uiBindSlot + 1);

  if (m_BoundUnoderedAccessViews[bindingInformation.m_uiBindSlot] != pUnorderedAccessViewD3D11)
  {
    m_BoundUnoderedAccessViews[bindingInformation.m_uiBindSlot]         = pUnorderedAccessViewD3D11;
    m_ResourcesForUnorderedAccessViews[bindingInformation.m_uiBindSlot] = pTextureView != nullptr ? pTextureView->GetTexture() : nullptr;
    m_BoundUnoderedAccessViewsRange.SetToIncludeValue(bindingInformation.m_uiBindSlot);
  }
}

void xiiGALPipelineStateD3D11::SetSamplerPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALSampler* pSampler)
{
  XII_ASSERT_RELEASE(bindingInformation.m_uiBindSet == 0, "In D3D11, Shader resources use a single descriptor set.");
  XII_ASSERT_RELEASE(bindingInformation.m_uiBindSlot < XII_GAL_MAX_SAMPLER_COUNT, "Sampler bind slot ({0}) must be in the range [0, {1})!", bindingInformation.m_uiBindSlot, XII_GAL_MAX_SAMPLER_COUNT);

  ID3D11SamplerState* pD3D11SamplerState = pSampler != nullptr ? static_cast<xiiGALSamplerD3D11*>(pSampler)->GetSampler() : nullptr;

  xiiStaticBitfield32 stageBitfield = xiiStaticBitfield32::FromMask(bindingInformation.m_ShaderStages.GetValue());
  for (auto it = stageBitfield.GetIterator(); it.IsValid(); ++it)
  {
    xiiUInt32 uiStageIndex = it.Value();

    if (m_pBoundSamplerStates[uiStageIndex][bindingInformation.m_uiBindSlot] != pD3D11SamplerState)
    {
      m_pBoundSamplerStates[uiStageIndex][bindingInformation.m_uiBindSlot] = pD3D11SamplerState;
      m_BoundSamplerStatesRange[uiStageIndex].SetToIncludeValue(bindingInformation.m_uiBindSlot);
    }
  }
}

//////////////////////////////////////////////////////////////////////////

static void SetShaderResources(xiiGALShaderStage::Enum stage, ID3D11DeviceContext* pContext, xiiUInt32 uiStartSlot, xiiUInt32 uiNumSlots, ID3D11ShaderResourceView** pShaderResourceViews)
{
  switch (stage)
  {
    case xiiGALShaderStage::Vertex:
      pContext->VSSetShaderResources(uiStartSlot, uiNumSlots, pShaderResourceViews);
      break;
    case xiiGALShaderStage::Hull:
      pContext->HSSetShaderResources(uiStartSlot, uiNumSlots, pShaderResourceViews);
      break;
    case xiiGALShaderStage::Domain:
      pContext->DSSetShaderResources(uiStartSlot, uiNumSlots, pShaderResourceViews);
      break;
    case xiiGALShaderStage::Geometry:
      pContext->GSSetShaderResources(uiStartSlot, uiNumSlots, pShaderResourceViews);
      break;
    case xiiGALShaderStage::Pixel:
      pContext->PSSetShaderResources(uiStartSlot, uiNumSlots, pShaderResourceViews);
      break;
    case xiiGALShaderStage::Compute:
      pContext->CSSetShaderResources(uiStartSlot, uiNumSlots, pShaderResourceViews);
      break;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }
}

static void SetConstantBuffers(xiiGALShaderStage::Enum stage, ID3D11DeviceContext* pContext, xiiUInt32 uiStartSlot, xiiUInt32 uiNumSlots, ID3D11Buffer** pConstantBuffers)
{
  switch (stage)
  {
    case xiiGALShaderStage::Vertex:
      pContext->VSSetConstantBuffers(uiStartSlot, uiNumSlots, pConstantBuffers);
      break;
    case xiiGALShaderStage::Hull:
      pContext->HSSetConstantBuffers(uiStartSlot, uiNumSlots, pConstantBuffers);
      break;
    case xiiGALShaderStage::Domain:
      pContext->DSSetConstantBuffers(uiStartSlot, uiNumSlots, pConstantBuffers);
      break;
    case xiiGALShaderStage::Geometry:
      pContext->GSSetConstantBuffers(uiStartSlot, uiNumSlots, pConstantBuffers);
      break;
    case xiiGALShaderStage::Pixel:
      pContext->PSSetConstantBuffers(uiStartSlot, uiNumSlots, pConstantBuffers);
      break;
    case xiiGALShaderStage::Compute:
      pContext->CSSetConstantBuffers(uiStartSlot, uiNumSlots, pConstantBuffers);
      break;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }
}

static void SetSamplers(xiiGALShaderStage::Enum stage, ID3D11DeviceContext* pContext, xiiUInt32 uiStartSlot, xiiUInt32 uiNumSlots, ID3D11SamplerState** pSamplerStates)
{
  switch (stage)
  {
    case xiiGALShaderStage::Vertex:
      pContext->VSSetSamplers(uiStartSlot, uiNumSlots, pSamplerStates);
      break;
    case xiiGALShaderStage::Hull:
      pContext->HSSetSamplers(uiStartSlot, uiNumSlots, pSamplerStates);
      break;
    case xiiGALShaderStage::Domain:
      pContext->DSSetSamplers(uiStartSlot, uiNumSlots, pSamplerStates);
      break;
    case xiiGALShaderStage::Geometry:
      pContext->GSSetSamplers(uiStartSlot, uiNumSlots, pSamplerStates);
      break;
    case xiiGALShaderStage::Pixel:
      pContext->PSSetSamplers(uiStartSlot, uiNumSlots, pSamplerStates);
      break;
    case xiiGALShaderStage::Compute:
      pContext->CSSetSamplers(uiStartSlot, uiNumSlots, pSamplerStates);
      break;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }
}

//////////////////////////////////////////////////////////////////////////

xiiResult xiiGALPipelineStateD3D11::CommitShaderResources(xiiGALCommandListD3D11* pCommandListD3D11)
{
  auto pContext = pCommandListD3D11->GetD3D11DeferredContext();

  // Set shaders.
  if (m_Description.IsAnyGraphicsPipeline())
  {
    pContext->VSSetShader(static_cast<ID3D11VertexShader*>(m_pBoundShaders[xiiGALShaderStage::GetStageIndex(xiiGALShaderStage::Vertex)]), nullptr, 0U);
    pContext->HSSetShader(static_cast<ID3D11HullShader*>(m_pBoundShaders[xiiGALShaderStage::GetStageIndex(xiiGALShaderStage::Hull)]), nullptr, 0U);
    pContext->DSSetShader(static_cast<ID3D11DomainShader*>(m_pBoundShaders[xiiGALShaderStage::GetStageIndex(xiiGALShaderStage::Domain)]), nullptr, 0U);
    pContext->GSSetShader(static_cast<ID3D11GeometryShader*>(m_pBoundShaders[xiiGALShaderStage::GetStageIndex(xiiGALShaderStage::Geometry)]), nullptr, 0U);
    pContext->PSSetShader(static_cast<ID3D11PixelShader*>(m_pBoundShaders[xiiGALShaderStage::GetStageIndex(xiiGALShaderStage::Pixel)]), nullptr, 0U);
  }
  else if (m_Description.IsComputePipeline())
  {
    pContext->CSSetShader(static_cast<ID3D11ComputeShader*>(m_pBoundShaders[xiiGALShaderStage::GetStageIndex(xiiGALShaderStage::Compute)]), nullptr, 0U);
  }

  // Set input layout.
  pContext->IASetInputLayout((m_pInputLayoutD3D11 != nullptr) ? m_pInputLayoutD3D11->GetInputLayout() : nullptr);

  // Set constant (uniform) buffers.
  for (xiiUInt32 uiStage = 0; uiStage < xiiGALShaderStage::ENUM_COUNT; ++uiStage)
  {
    if (m_BoundConstantBuffersRange[uiStage].IsValid())
    {
      const xiiUInt32 uiStartSlot = m_BoundConstantBuffersRange[uiStage].m_uiMin;
      const xiiUInt32 uiNumSlots  = m_BoundConstantBuffersRange[uiStage].GetCount();

      SetConstantBuffers(xiiGALShaderStage::GetStageFlag(uiStage), pContext, uiStartSlot, uiNumSlots, m_pBoundConstantBuffers[uiStage] + uiStartSlot);
    }
  }

  // Set Unordered Access Views (UAVs) before shader resource views (SRVs), since UAVs are outputs that need to be unbound before they need to be potentially rebound as SRVs.
  if (m_BoundUnoderedAccessViewsRange.IsValid())
  {
    const xiiUInt32 uiStartSlot = m_BoundUnoderedAccessViewsRange.m_uiMin;
    const xiiUInt32 uiNumSlots  = m_BoundUnoderedAccessViewsRange.GetCount();
    pContext->CSSetUnorderedAccessViews(uiStartSlot, uiNumSlots, m_BoundUnoderedAccessViews.GetData() + uiStartSlot, nullptr); // Maybe consider unordered access views count reset.
  }

  for (xiiUInt32 uiStage = 0; uiStage < xiiGALShaderStage::ENUM_COUNT; ++uiStage)
  {
    // Need to do bindings even on inactive shader stages since we might miss unbindings otherwise!
    if (m_BoundShaderResourceViewsRange[uiStage].IsValid())
    {
      const xiiUInt32 uiStartSlot = m_BoundShaderResourceViewsRange[uiStage].m_uiMin;
      const xiiUInt32 uiNumSlots  = m_BoundShaderResourceViewsRange[uiStage].GetCount();

      SetShaderResources(xiiGALShaderStage::GetStageFlag(uiStage), pContext, uiStartSlot, uiNumSlots, m_pBoundShaderResourceViews[uiStage].GetData() + uiStartSlot);
    }

    // Don't need to unset sampler stages for unbound shader stages.
    if (m_pBoundShaders[uiStage] == nullptr)
      continue;

    if (m_BoundSamplerStatesRange[uiStage].IsValid())
    {
      const xiiUInt32 uiStartSlot = m_BoundSamplerStatesRange[uiStage].m_uiMin;
      const xiiUInt32 uiNumSlots  = m_BoundSamplerStatesRange[uiStage].GetCount();

      SetSamplers(xiiGALShaderStage::GetStageFlag(uiStage), pContext, uiStartSlot, uiNumSlots, m_pBoundSamplerStates[uiStage] + uiStartSlot);
    }
  }
  return XII_SUCCESS;
}

bool xiiGALPipelineStateD3D11::UnsetResourceViews(const xiiGALResource* pResource)
{
  bool bResult = false;

  for (xiiUInt32 uiStage = 0U; uiStage < xiiGALShaderStage::ENUM_COUNT; ++uiStage)
  {
    for (xiiUInt32 uiSlot = 0U; uiSlot < m_ResourcesForResourceViews[uiStage].GetCount(); ++uiSlot)
    {
      if (m_ResourcesForResourceViews[uiStage][uiSlot] == pResource)
      {
        m_ResourcesForResourceViews[uiStage][uiSlot] = nullptr;
        m_pBoundShaderResourceViews[uiStage][uiSlot] = nullptr;
        m_BoundShaderResourceViewsRange[uiStage].SetToIncludeValue(uiSlot);

        bResult = true;
      }
    }
  }
  return bResult;
}

bool xiiGALPipelineStateD3D11::UnsetUnorderedAccessViews(const xiiGALResource* pResource)
{
  bool bResult = false;

  for (xiiUInt32 uiSlot = 0U; uiSlot < m_ResourcesForUnorderedAccessViews.GetCount(); ++uiSlot)
  {
    if (m_ResourcesForUnorderedAccessViews[uiSlot] == pResource)
    {
      m_ResourcesForUnorderedAccessViews[uiSlot] = nullptr;
      m_BoundUnoderedAccessViews[uiSlot]         = nullptr;
      m_BoundUnoderedAccessViewsRange.SetToIncludeValue(uiSlot);

      bResult = true;
    }
  }
  return bResult;
}

void xiiGALPipelineStateD3D11::ResetBoundResources()
{
  for (xiiUInt32 uiStage = 0; uiStage < xiiGALShaderStage::ENUM_COUNT; ++uiStage)
  {
    for (xiiUInt32 i = 0; i < XII_GAL_MAX_CONSTANT_BUFFER_COUNT; ++i)
    {
      m_pBoundConstantBuffers[uiStage][i] = nullptr;
    }
    m_BoundConstantBuffersRange[uiStage].Reset();

    m_pBoundShaderResourceViews[uiStage].Clear();
    m_ResourcesForResourceViews[uiStage].Clear();
    m_BoundShaderResourceViewsRange[uiStage].Reset();

    for (xiiUInt32 i = 0; i < XII_GAL_MAX_SAMPLER_COUNT; ++i)
    {
      m_pBoundSamplerStates[uiStage][i] = nullptr;
    }
    m_BoundSamplerStatesRange[uiStage].Reset();
  }

  m_BoundUnoderedAccessViews.Clear();
  m_ResourcesForUnorderedAccessViews.Clear();
  m_BoundUnoderedAccessViewsRange.Reset();
}

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_States_Implementation_PipelineStateD3D11);
