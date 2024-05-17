#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <GraphicsD3D11/Device/DeviceD3D11.h>
#include <GraphicsD3D11/States/DepthStencilStateD3D11.h>


// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALDepthStencilStateD3D11, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALDepthStencilStateD3D11::xiiGALDepthStencilStateD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALDepthStencilStateCreationDescription& creationDescription) :
  xiiGALDepthStencilState(pDeviceD3D11, creationDescription)
{
}

xiiGALDepthStencilStateD3D11::~xiiGALDepthStencilStateD3D11() = default;

xiiResult xiiGALDepthStencilStateD3D11::InitPlatform()
{
  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);

  D3D11_DEPTH_STENCIL_DESC depthStencilDescription = {};
  depthStencilDescription.DepthEnable              = D3D11_BOOL(m_Description.m_bDepthEnable);
  depthStencilDescription.DepthWriteMask           = m_Description.m_bDepthWriteEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
  depthStencilDescription.DepthFunc                = xiiD3D11TypeConversions::GetComparisonFunc(m_Description.m_ComparisonDepthFunction);

  depthStencilDescription.StencilEnable    = D3D11_BOOL(m_Description.m_bStencilEnable);
  depthStencilDescription.StencilReadMask  = m_Description.m_uiStencilReadMask;
  depthStencilDescription.StencilWriteMask = m_Description.m_uiStencilWriteMask;

  depthStencilDescription.FrontFace.StencilFailOp      = xiiD3D11TypeConversions::GetStencilOp(m_Description.m_FrontFace.m_StencilFailOperation);
  depthStencilDescription.FrontFace.StencilDepthFailOp = xiiD3D11TypeConversions::GetStencilOp(m_Description.m_FrontFace.m_StencilDepthFailOperation);
  depthStencilDescription.FrontFace.StencilPassOp      = xiiD3D11TypeConversions::GetStencilOp(m_Description.m_FrontFace.m_StencilPassOperation);
  depthStencilDescription.FrontFace.StencilFunc        = xiiD3D11TypeConversions::GetComparisonFunc(m_Description.m_FrontFace.m_ComparisonFunction);

  depthStencilDescription.BackFace.StencilFailOp      = xiiD3D11TypeConversions::GetStencilOp(m_Description.m_BackFace.m_StencilFailOperation);
  depthStencilDescription.BackFace.StencilDepthFailOp = xiiD3D11TypeConversions::GetStencilOp(m_Description.m_BackFace.m_StencilDepthFailOperation);
  depthStencilDescription.BackFace.StencilPassOp      = xiiD3D11TypeConversions::GetStencilOp(m_Description.m_BackFace.m_StencilPassOperation);
  depthStencilDescription.BackFace.StencilFunc        = xiiD3D11TypeConversions::GetComparisonFunc(m_Description.m_BackFace.m_ComparisonFunction);

  if (FAILED(pDeviceD3D11->GetD3D11Device()->CreateDepthStencilState(&depthStencilDescription, &m_pDepthStencilState)))
  {
    xiiLog::Error("Failed to create the Direct3D11 depth stencil state.");
    return XII_FAILURE;
  }
  return XII_SUCCESS;
}

xiiResult xiiGALDepthStencilStateD3D11::DeInitPlatform()
{
  XII_GAL_D3D11_RELEASE(m_pDepthStencilState);

  return XII_SUCCESS;
}

void xiiGALDepthStencilStateD3D11::SetDebugNamePlatform(xiiStringView sName)
{
  if (m_pDepthStencilState != nullptr)
  {
    xiiStringBuilder sb;
    if (FAILED(m_pDepthStencilState->SetPrivateData(WKPDID_D3DDebugObjectName, sName.GetElementCount(), sName.GetData(sb))))
    {
      xiiLog::Error("Failed to set the Direct3D11 depth stencil state debug name.");
    }
  }
}

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_States_Implementation_DepthStencilStateD3D11);
