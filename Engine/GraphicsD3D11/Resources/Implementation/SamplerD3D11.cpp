#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <GraphicsD3D11/Device/DeviceD3D11.h>
#include <GraphicsD3D11/Resources/SamplerD3D11.h>

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALSamplerD3D11, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALSamplerD3D11::xiiGALSamplerD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALSamplerCreationDescription& creationDescription) :
  xiiGALSampler(pDeviceD3D11, creationDescription)
{
}

xiiGALSamplerD3D11::~xiiGALSamplerD3D11() = default;

xiiResult xiiGALSamplerD3D11::InitPlatform()
{
  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);

  D3D11_SAMPLER_DESC samplerDescription = {};
  samplerDescription.AddressU           = xiiD3D11TypeConversions::GetTextureAddressMode(m_Description.m_AddressU);
  samplerDescription.AddressV           = xiiD3D11TypeConversions::GetTextureAddressMode(m_Description.m_AddressV);
  samplerDescription.AddressW           = xiiD3D11TypeConversions::GetTextureAddressMode(m_Description.m_AddressW);
  samplerDescription.BorderColor[0]     = m_Description.m_BorderColor.r;
  samplerDescription.BorderColor[1]     = m_Description.m_BorderColor.g;
  samplerDescription.BorderColor[2]     = m_Description.m_BorderColor.b;
  samplerDescription.BorderColor[3]     = m_Description.m_BorderColor.a;
  samplerDescription.ComparisonFunc     = xiiD3D11TypeConversions::GetComparisonFunc(m_Description.m_ComparisonFunction);
  samplerDescription.MaxAnisotropy      = m_Description.m_uiMaxAnisotropy;
  samplerDescription.MinLOD             = m_Description.m_fMinLOD;
  samplerDescription.MaxLOD             = m_Description.m_fMaxLOD;
  samplerDescription.MipLODBias         = m_Description.m_fMipLODBias;

  if (m_Description.m_MagFilter == xiiGALFilterType::Anisotropic || m_Description.m_MinFilter == xiiGALFilterType::Anisotropic || m_Description.m_MipFilter == xiiGALFilterType::Anisotropic)
  {
    if (m_Description.m_ComparisonFunction == xiiGALComparisonFunction::Never)
    {
      samplerDescription.Filter = xiiD3D11TypeConversions::GetFilter(xiiGALFilterType::Anisotropic, xiiGALFilterType::Anisotropic, xiiGALFilterType::Anisotropic);
    }
    else
    {
      samplerDescription.Filter = xiiD3D11TypeConversions::GetFilter(xiiGALFilterType::ComparisonAnisotropic, xiiGALFilterType::ComparisonAnisotropic, xiiGALFilterType::ComparisonAnisotropic);
    }
  }
  else
  {
    samplerDescription.Filter = xiiD3D11TypeConversions::GetFilter(m_Description.m_MinFilter, m_Description.m_MagFilter, m_Description.m_MipFilter);
  }

  if (FAILED(pDeviceD3D11->GetD3D11Device()->CreateSamplerState(&samplerDescription, &m_pSampler)))
  {
    xiiLog::Error("Failed to create sampler D3D11 sampler state.");
    return XII_FAILURE;
  }
  return XII_SUCCESS;
}

xiiResult xiiGALSamplerD3D11::DeInitPlatform()
{
  XII_GAL_D3D11_RELEASE(m_pSampler);

  return XII_FAILURE;
}

void xiiGALSamplerD3D11::SetDebugNamePlatform(xiiStringView sName)
{
  if (m_pSampler != nullptr)
  {
    xiiStringBuilder sb;
    if (FAILED(m_pSampler->SetPrivateData(WKPDID_D3DDebugObjectName, sName.GetElementCount(), sName.GetData(sb))))
    {
      xiiLog::Error("Failed to set the Direct3D11 sampler debug name.");
    }
  }
}

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_Resources_Implementation_SamplerD3D11);
