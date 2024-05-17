#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <GraphicsD3D11/Device/DeviceD3D11.h>
#include <GraphicsD3D11/States/BlendStateD3D11.h>

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALBlendStateD3D11, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALBlendStateD3D11::xiiGALBlendStateD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALBlendStateCreationDescription& creationDescription) :
  xiiGALBlendState(pDeviceD3D11, creationDescription)
{
}

xiiGALBlendStateD3D11::~xiiGALBlendStateD3D11() = default;

xiiResult xiiGALBlendStateD3D11::InitPlatform()
{
  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);

  D3D11_BLEND_DESC blendDescription       = {};
  blendDescription.AlphaToCoverageEnable  = D3D11_BOOL(m_Description.m_bAlphaToCoverage);
  blendDescription.IndependentBlendEnable = D3D11_BOOL(m_Description.m_bIndependentBlend);

  for (xiiUInt32 uiAttachmentIndex = 0; uiAttachmentIndex < m_Description.m_RenderTargets.GetCount(); ++uiAttachmentIndex)
  {
    auto& rtBlendState      = m_Description.m_RenderTargets[uiAttachmentIndex];
    auto& rtAttachmentState = blendDescription.RenderTarget[uiAttachmentIndex];

    rtAttachmentState.BlendEnable = D3D11_BOOL(rtBlendState.m_bBlendEnable);

    rtAttachmentState.SrcBlend  = xiiD3D11TypeConversions::GetBlendFactor(rtBlendState.m_SourceBlend);
    rtAttachmentState.DestBlend = xiiD3D11TypeConversions::GetBlendFactor(rtBlendState.m_DestinationBlend);
    rtAttachmentState.BlendOp   = xiiD3D11TypeConversions::GetBlendOp(rtBlendState.m_BlendOperation);

    rtAttachmentState.SrcBlendAlpha  = xiiD3D11TypeConversions::GetBlendFactor(rtBlendState.m_SourceBlendAlpha);
    rtAttachmentState.DestBlendAlpha = xiiD3D11TypeConversions::GetBlendFactor(rtBlendState.m_DestinationBlendAlpha);
    rtAttachmentState.BlendOpAlpha   = xiiD3D11TypeConversions::GetBlendOp(rtBlendState.m_BlendOperationAlpha);

    rtAttachmentState.RenderTargetWriteMask = xiiD3D11TypeConversions::GetColorWriteMask(rtBlendState.m_ColorMask);
  }

  if (FAILED(pDeviceD3D11->GetD3D11Device()->CreateBlendState(&blendDescription, &m_pBlendState)))
  {
    xiiLog::Error("Failed to create the Direct3D11 blend state.");
    return XII_FAILURE;
  }
  return XII_SUCCESS;
}

xiiResult xiiGALBlendStateD3D11::DeInitPlatform()
{
  XII_GAL_D3D11_RELEASE(m_pBlendState);

  return XII_SUCCESS;
}

void xiiGALBlendStateD3D11::SetDebugNamePlatform(xiiStringView sName)
{
  if (m_pBlendState != nullptr)
  {
    xiiStringBuilder sb;
    if (FAILED(m_pBlendState->SetPrivateData(WKPDID_D3DDebugObjectName, sName.GetElementCount(), sName.GetData(sb))))
    {
      xiiLog::Error("Failed to set the Direct3D11 blend state debug name.");
    }
  }
}

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_States_Implementation_BlendStateD3D11);
