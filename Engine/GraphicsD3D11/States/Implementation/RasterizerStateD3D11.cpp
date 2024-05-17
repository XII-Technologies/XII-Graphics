#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <GraphicsD3D11/Device/DeviceD3D11.h>
#include <GraphicsD3D11/States/RasterizerStateD3D11.h>

#include <GraphicsD3D11/Utilities/D3D11TypeConversions.h>

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALRasterizerStateD3D11, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALRasterizerStateD3D11::xiiGALRasterizerStateD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALRasterizerStateCreationDescription& creationDescription) :
  xiiGALRasterizerState(pDeviceD3D11, creationDescription)
{
}

xiiGALRasterizerStateD3D11::~xiiGALRasterizerStateD3D11() = default;

xiiResult xiiGALRasterizerStateD3D11::InitPlatform()
{
  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);

  D3D11_RASTERIZER_DESC rasterizerDescription = {};
  rasterizerDescription.FillMode              = xiiD3D11TypeConversions::GetFillMode(m_Description.m_FillMode);
  rasterizerDescription.CullMode              = xiiD3D11TypeConversions::GetCullMode(m_Description.m_CullMode);
  rasterizerDescription.FrontCounterClockwise = D3D11_BOOL(m_Description.m_bFrontCounterClockwise);
  rasterizerDescription.DepthBias             = m_Description.m_iDepthBias;
  rasterizerDescription.DepthBiasClamp        = m_Description.m_fDepthBiasClamp;
  rasterizerDescription.SlopeScaledDepthBias  = m_Description.m_fSlopeScaledDepthBias;
  rasterizerDescription.DepthClipEnable       = D3D11_BOOL(m_Description.m_bDepthClipEnable);
  rasterizerDescription.AntialiasedLineEnable = D3D11_BOOL(m_Description.m_bAntialiasedLineEnable);
  rasterizerDescription.MultisampleEnable     = D3D11_BOOL(m_Description.m_bAntialiasedLineEnable);

  // Not available in the D3D11 API currently.
  // rasterizerDescription.ScissorEnable = D3D11_BOOL(m_Description.m_bScissorEnable);

  if (FAILED(pDeviceD3D11->GetD3D11Device()->CreateRasterizerState(&rasterizerDescription, &m_pRasterizerState)))
  {
    xiiLog::Error("Failed to create the Direct3D11 rasterizer state.");
    return XII_FAILURE;
  }
  return XII_SUCCESS;
}

xiiResult xiiGALRasterizerStateD3D11::DeInitPlatform()
{
  XII_GAL_D3D11_RELEASE(m_pRasterizerState);

  return XII_SUCCESS;
}

void xiiGALRasterizerStateD3D11::SetDebugNamePlatform(xiiStringView sName)
{
  if (m_pRasterizerState != nullptr)
  {
    xiiStringBuilder sb;
    if (FAILED(m_pRasterizerState->SetPrivateData(WKPDID_D3DDebugObjectName, sName.GetElementCount(), sName.GetData(sb))))
    {
      xiiLog::Error("Failed to set the Direct3D11 rasterizer state debug name.");
    }
  }
}

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_States_Implementation_RasterizerStateD3D11);
