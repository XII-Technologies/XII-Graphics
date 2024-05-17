#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <GraphicsD3D11/Device/DeviceD3D11.h>
#include <GraphicsD3D11/Resources/TextureD3D11.h>
#include <GraphicsD3D11/Resources/TextureViewD3D11.h>
#include <GraphicsFoundation/Utilities/GraphicsUtilities.h>

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALTextureViewD3D11, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALTextureViewD3D11::xiiGALTextureViewD3D11(xiiGALDeviceD3D11* pDeviceD3D11, xiiGALTexture* pTexture, const xiiGALTextureViewCreationDescription& creationDescription) :
  xiiGALTextureView(pDeviceD3D11, pTexture, creationDescription)
{
}

xiiGALTextureViewD3D11::~xiiGALTextureViewD3D11() = default;

xiiResult xiiGALTextureViewD3D11::InitPlatform()
{
  switch (m_Description.m_ViewType)
  {
    case xiiGALTextureViewType::ShaderResource:
    {
      if (!xiiGALGraphicsUtilities::IsIdentityComponentMapping(m_Description.m_ComponentSwizzle))
      {
        xiiLog::Error("Direct3D11 does not support texture component swizzle.");
        return XII_FAILURE;
      }

      ID3D11ShaderResourceView* pShaderResourceView = nullptr;

      XII_SUCCEED_OR_RETURN(CreateSRV(&pShaderResourceView));

      m_pTextureView = pShaderResourceView;
    }
    break;
    case xiiGALTextureViewType::RenderTarget:
    {
      ID3D11RenderTargetView* pRenderTargetView = nullptr;

      XII_SUCCEED_OR_RETURN(CreateRTV(&pRenderTargetView));

      m_pTextureView = pRenderTargetView;
    }
    break;
    case xiiGALTextureViewType::DepthStencil:
    case xiiGALTextureViewType::ReadOnlyDepthStencil:
    {
      ID3D11DepthStencilView* pDepthStencilView = nullptr;

      XII_SUCCEED_OR_RETURN(CreateDSV(&pDepthStencilView));

      m_pTextureView = pDepthStencilView;
    }
    break;
    case xiiGALTextureViewType::UnorderedAccess:
    {
      ID3D11UnorderedAccessView* pUnorderedAccessView = nullptr;

      XII_SUCCEED_OR_RETURN(CreateUAV(&pUnorderedAccessView));

      m_pTextureView = pUnorderedAccessView;
    }
    break;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }
  return XII_SUCCESS;
}

xiiResult xiiGALTextureViewD3D11::DeInitPlatform()
{
  XII_GAL_D3D11_RELEASE(m_pTextureView);

  return XII_SUCCESS;
}

void xiiGALTextureViewD3D11::SetDebugNamePlatform(xiiStringView sName)
{
  if (m_pTextureView != nullptr)
  {
    xiiStringBuilder sb;
    if (FAILED(m_pTextureView->SetPrivateData(WKPDID_D3DDebugObjectName, sName.GetElementCount(), sName.GetData(sb))))
    {
      xiiLog::Error("Failed to set the Direct3D11 texture view debug name.");
    }
  }
}

xiiResult xiiGALTextureViewD3D11::CreateSRV(ID3D11ShaderResourceView** ppShaderResourceView)
{
  XII_ASSERT_DEV(m_Description.m_ViewType == xiiGALTextureViewType::ShaderResource, "xiiGALTextureViewType::ShaderResource view type is expected.");

  if (!(m_Description.m_ResourceDimension == xiiGALResourceDimension::Texture2D || m_Description.m_ResourceDimension == xiiGALResourceDimension::Texture2DArray || m_Description.m_ResourceDimension == xiiGALResourceDimension::TextureCube || m_Description.m_ResourceDimension == xiiGALResourceDimension::TextureCubeArray))
  {
    xiiLog::Error("Unsupported texture view type. Only Texture2D and TextureCube and arrays are supported.");
    return XII_FAILURE;
  }

  XII_ASSERT_DEV(m_Description.m_Format != xiiGALTextureFormat::Unknown, "");

  xiiGALDeviceD3D11*  pDeviceD3D11  = static_cast<xiiGALDeviceD3D11*>(m_pDevice);
  xiiGALTextureD3D11* pTextureD3D11 = static_cast<xiiGALTextureD3D11*>(m_pDevice->GetTexture(m_Description.m_hTexture));

  const auto& textureDescription = pTextureD3D11->GetDescription();

  XII_ASSERT_DEV(textureDescription.m_BindFlags.IsSet(xiiGALBindFlags::ShaderResource), "xiiGALBindFlags::ShaderResource is not set.");

  D3D11_SHADER_RESOURCE_VIEW_DESC viewDescription = {};
  viewDescription.Format                          = xiiD3D11TypeConversions::GetFormat(m_Description.m_Format);

  switch (m_Description.m_ResourceDimension)
  {
    case xiiGALResourceDimension::Texture1D:
    {
      viewDescription.ViewDimension             = D3D_SRV_DIMENSION_TEXTURE1D;
      viewDescription.Texture1D.MipLevels       = m_Description.m_uiMipLevelCount;
      viewDescription.Texture1D.MostDetailedMip = m_Description.m_uiMostDetailedMip;
    }
    break;
    case xiiGALResourceDimension::Texture1DArray:
    {
      viewDescription.ViewDimension                  = D3D_SRV_DIMENSION_TEXTURE1DARRAY;
      viewDescription.Texture1DArray.ArraySize       = m_Description.m_uiArrayOrDepthSlicesCount;
      viewDescription.Texture1DArray.FirstArraySlice = m_Description.m_uiFirstArrayOrDepthSlice;
      viewDescription.Texture1DArray.MipLevels       = m_Description.m_uiMipLevelCount;
      viewDescription.Texture1DArray.MostDetailedMip = m_Description.m_uiMostDetailedMip;
    }
    break;
    case xiiGALResourceDimension::Texture2D:
    {
      if (textureDescription.m_uiSampleCount > 1U)
      {
        viewDescription.ViewDimension                           = D3D_SRV_DIMENSION_TEXTURE2DMS;
        viewDescription.Texture2DMS.UnusedField_NothingToDefine = 0U;
      }
      else
      {
        viewDescription.ViewDimension             = D3D_SRV_DIMENSION_TEXTURE2D;
        viewDescription.Texture2D.MipLevels       = m_Description.m_uiMipLevelCount;
        viewDescription.Texture2D.MostDetailedMip = m_Description.m_uiMostDetailedMip;
      }
    }
    break;
    case xiiGALResourceDimension::Texture2DArray:
    {
      if (textureDescription.m_uiSampleCount > 1U)
      {
        viewDescription.ViewDimension                    = D3D_SRV_DIMENSION_TEXTURE2DMSARRAY;
        viewDescription.Texture2DMSArray.ArraySize       = m_Description.m_uiArrayOrDepthSlicesCount;
        viewDescription.Texture2DMSArray.FirstArraySlice = m_Description.m_uiFirstArrayOrDepthSlice;
      }
      else
      {
        viewDescription.ViewDimension                  = D3D_SRV_DIMENSION_TEXTURE2DARRAY;
        viewDescription.Texture2DArray.ArraySize       = m_Description.m_uiArrayOrDepthSlicesCount;
        viewDescription.Texture2DArray.FirstArraySlice = m_Description.m_uiFirstArrayOrDepthSlice;
        viewDescription.Texture2DArray.MipLevels       = m_Description.m_uiMipLevelCount;
        viewDescription.Texture2DArray.MostDetailedMip = m_Description.m_uiMostDetailedMip;
      }
    }
    break;
    case xiiGALResourceDimension::Texture3D:
    {
      viewDescription.ViewDimension             = D3D_SRV_DIMENSION_TEXTURE3D;
      viewDescription.Texture3D.MipLevels       = m_Description.m_uiMipLevelCount;
      viewDescription.Texture3D.MostDetailedMip = m_Description.m_uiMostDetailedMip;
    }
    break;
    case xiiGALResourceDimension::TextureCube:
    {
      viewDescription.ViewDimension               = D3D_SRV_DIMENSION_TEXTURECUBE;
      viewDescription.TextureCube.MipLevels       = m_Description.m_uiMipLevelCount;
      viewDescription.TextureCube.MostDetailedMip = m_Description.m_uiMostDetailedMip;
    }
    break;
    case xiiGALResourceDimension::TextureCubeArray:
    {
      viewDescription.ViewDimension                     = D3D_SRV_DIMENSION_TEXTURECUBEARRAY;
      viewDescription.TextureCubeArray.MipLevels        = m_Description.m_uiMipLevelCount;
      viewDescription.TextureCubeArray.MostDetailedMip  = m_Description.m_uiMostDetailedMip;
      viewDescription.TextureCubeArray.First2DArrayFace = m_Description.m_uiFirstArrayOrDepthSlice;
      viewDescription.TextureCubeArray.NumCubes         = m_Description.m_uiArrayOrDepthSlicesCount / 6U;
    }
    break;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }

  if (FAILED(pDeviceD3D11->GetD3D11Device()->CreateShaderResourceView(pTextureD3D11->GetTexture(), &viewDescription, ppShaderResourceView)))
  {
    xiiLog::Error("Failed to create the Direct3D11 shader resource view.");
    return XII_FAILURE;
  }
  return XII_SUCCESS;
}

xiiResult xiiGALTextureViewD3D11::CreateRTV(ID3D11RenderTargetView** ppRenderTargetView)
{
  XII_ASSERT_DEV(m_Description.m_ViewType == xiiGALTextureViewType::RenderTarget, "xiiGALTextureViewType::RenderTarget view type is expected.");

  if (!(m_Description.m_ResourceDimension == xiiGALResourceDimension::Texture2D || m_Description.m_ResourceDimension == xiiGALResourceDimension::Texture2DArray))
  {
    xiiLog::Error("Unsupported texture view type. Only Texture2D and arrays are supported.");
    return XII_FAILURE;
  }

  XII_ASSERT_DEV(m_Description.m_Format != xiiGALTextureFormat::Unknown, "");

  xiiGALDeviceD3D11*  pDeviceD3D11  = static_cast<xiiGALDeviceD3D11*>(m_pDevice);
  xiiGALTextureD3D11* pTextureD3D11 = static_cast<xiiGALTextureD3D11*>(m_pDevice->GetTexture(m_Description.m_hTexture));

  const auto& textureDescription = pTextureD3D11->GetDescription();

  XII_ASSERT_DEV(textureDescription.m_BindFlags.IsSet(xiiGALBindFlags::RenderTarget), "xiiGALBindFlags::RenderTarget is not set.");

  D3D11_RENDER_TARGET_VIEW_DESC viewDescription = {};
  viewDescription.Format                        = xiiD3D11TypeConversions::GetFormat(m_Description.m_Format);

  switch (m_Description.m_ResourceDimension)
  {
    case xiiGALResourceDimension::Texture1D:
    {
      viewDescription.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE1D;
      viewDescription.Texture1D.MipSlice = m_Description.m_uiMostDetailedMip;
    }
    break;
    case xiiGALResourceDimension::Texture1DArray:
    {
      viewDescription.ViewDimension                  = D3D11_RTV_DIMENSION_TEXTURE1DARRAY;
      viewDescription.Texture1DArray.ArraySize       = m_Description.m_uiArrayOrDepthSlicesCount;
      viewDescription.Texture1DArray.FirstArraySlice = m_Description.m_uiFirstArrayOrDepthSlice;
      viewDescription.Texture1DArray.MipSlice        = m_Description.m_uiMostDetailedMip;
    }
    break;
    case xiiGALResourceDimension::Texture2D:
    {
      if (textureDescription.m_uiSampleCount > 1U)
      {
        viewDescription.ViewDimension                           = D3D11_RTV_DIMENSION_TEXTURE2DMS;
        viewDescription.Texture2DMS.UnusedField_NothingToDefine = 0U;
      }
      else
      {
        viewDescription.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
        viewDescription.Texture2D.MipSlice = m_Description.m_uiMostDetailedMip;
      }
    }
    break;
    case xiiGALResourceDimension::Texture2DArray:
    {
      if (textureDescription.m_uiSampleCount > 1U)
      {
        viewDescription.ViewDimension                    = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
        viewDescription.Texture2DMSArray.ArraySize       = m_Description.m_uiArrayOrDepthSlicesCount;
        viewDescription.Texture2DMSArray.FirstArraySlice = m_Description.m_uiFirstArrayOrDepthSlice;
      }
      else
      {
        viewDescription.ViewDimension                  = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
        viewDescription.Texture2DArray.ArraySize       = m_Description.m_uiArrayOrDepthSlicesCount;
        viewDescription.Texture2DArray.FirstArraySlice = m_Description.m_uiFirstArrayOrDepthSlice;
        viewDescription.Texture2DArray.MipSlice        = m_Description.m_uiMostDetailedMip;
      }
    }
    break;
    case xiiGALResourceDimension::Texture3D:
    {
      viewDescription.ViewDimension         = D3D11_RTV_DIMENSION_TEXTURE3D;
      viewDescription.Texture3D.MipSlice    = m_Description.m_uiMostDetailedMip;
      viewDescription.Texture3D.FirstWSlice = m_Description.m_uiFirstArrayOrDepthSlice;
      viewDescription.Texture3D.WSize       = m_Description.m_uiArrayOrDepthSlicesCount;
    }
    break;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }

  if (FAILED(pDeviceD3D11->GetD3D11Device()->CreateRenderTargetView(pTextureD3D11->GetTexture(), &viewDescription, ppRenderTargetView)))
  {
    xiiLog::Error("Failed to create the Direct3D11 render target view.");
    return XII_FAILURE;
  }
  return XII_SUCCESS;
}

xiiResult xiiGALTextureViewD3D11::CreateDSV(ID3D11DepthStencilView** ppDepthStencilView)
{
  XII_ASSERT_DEV(m_Description.m_ViewType == xiiGALTextureViewType::DepthStencil || m_Description.m_ViewType == xiiGALTextureViewType::ReadOnlyDepthStencil, "xiiGALTextureViewType::DepthStencil or xiiGALTextureViewType::ReadOnlyDepthStencil view type is expected.");

  if (!(m_Description.m_ResourceDimension == xiiGALResourceDimension::Texture2D || m_Description.m_ResourceDimension == xiiGALResourceDimension::Texture2DArray))
  {
    xiiLog::Error("Unsupported texture view type. Only Texture2D and arrays are supported.");
    return XII_FAILURE;
  }

  XII_ASSERT_DEV(m_Description.m_Format != xiiGALTextureFormat::Unknown, "");

  xiiGALDeviceD3D11*  pDeviceD3D11  = static_cast<xiiGALDeviceD3D11*>(m_pDevice);
  xiiGALTextureD3D11* pTextureD3D11 = static_cast<xiiGALTextureD3D11*>(m_pDevice->GetTexture(m_Description.m_hTexture));

  const auto& textureDescription = pTextureD3D11->GetDescription();

  XII_ASSERT_DEV(textureDescription.m_BindFlags.IsSet(xiiGALBindFlags::DepthStencil), "xiiGALBindFlags::DepthStencil is not set.");

  D3D11_DEPTH_STENCIL_VIEW_DESC viewDescription = {};
  viewDescription.Format                        = xiiD3D11TypeConversions::GetFormat(m_Description.m_Format);

  if (m_Description.m_ViewType == xiiGALTextureViewType::ReadOnlyDepthStencil)
  {
    viewDescription.Flags |= D3D11_DSV_READ_ONLY_DEPTH;

    const auto& formatDescription = xiiGALGraphicsUtilities::GetTextureFormatProperties(m_Description.m_Format);
    if (formatDescription.m_ComponentType == xiiGALTextureFormatComponentType::DepthStencil)
    {
      viewDescription.Flags |= D3D11_DSV_READ_ONLY_STENCIL;
    }
  }

  switch (m_Description.m_ResourceDimension)
  {
    case xiiGALResourceDimension::Texture1D:
    {
      viewDescription.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE1D;
      viewDescription.Texture1D.MipSlice = m_Description.m_uiMostDetailedMip;
    }
    break;
    case xiiGALResourceDimension::Texture1DArray:
    {
      viewDescription.ViewDimension                  = D3D11_DSV_DIMENSION_TEXTURE1DARRAY;
      viewDescription.Texture1DArray.ArraySize       = m_Description.m_uiArrayOrDepthSlicesCount;
      viewDescription.Texture1DArray.FirstArraySlice = m_Description.m_uiFirstArrayOrDepthSlice;
      viewDescription.Texture1DArray.MipSlice        = m_Description.m_uiMostDetailedMip;
    }
    break;
    case xiiGALResourceDimension::Texture2D:
    {
      if (textureDescription.m_uiSampleCount > 1U)
      {
        viewDescription.ViewDimension                           = D3D11_DSV_DIMENSION_TEXTURE2DMS;
        viewDescription.Texture2DMS.UnusedField_NothingToDefine = 0U;
      }
      else
      {
        viewDescription.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
        viewDescription.Texture2D.MipSlice = m_Description.m_uiMostDetailedMip;
      }
    }
    break;
    case xiiGALResourceDimension::Texture2DArray:
    {
      if (textureDescription.m_uiSampleCount > 1U)
      {
        viewDescription.ViewDimension                    = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
        viewDescription.Texture2DMSArray.ArraySize       = m_Description.m_uiArrayOrDepthSlicesCount;
        viewDescription.Texture2DMSArray.FirstArraySlice = m_Description.m_uiFirstArrayOrDepthSlice;
      }
      else
      {
        viewDescription.ViewDimension                  = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
        viewDescription.Texture2DArray.ArraySize       = m_Description.m_uiArrayOrDepthSlicesCount;
        viewDescription.Texture2DArray.FirstArraySlice = m_Description.m_uiFirstArrayOrDepthSlice;
        viewDescription.Texture2DArray.MipSlice        = m_Description.m_uiMostDetailedMip;
      }
    }
    break;
    case xiiGALResourceDimension::Texture3D:
    {
      xiiLog::Error("Depth stencil views are not supported for 3D textures.");
      return XII_FAILURE;
    }
    break;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }

  if (FAILED(pDeviceD3D11->GetD3D11Device()->CreateDepthStencilView(pTextureD3D11->GetTexture(), &viewDescription, ppDepthStencilView)))
  {
    xiiLog::Error("Failed to create the Direct3D11 depth stencil view.");
    return XII_FAILURE;
  }
  return XII_SUCCESS;
}

xiiResult xiiGALTextureViewD3D11::CreateUAV(ID3D11UnorderedAccessView** ppUnorderedAccessView)
{
  XII_ASSERT_DEV(m_Description.m_ViewType == xiiGALTextureViewType::UnorderedAccess, "xiiGALTextureViewType::UnorderedAccess view type is expected.");

  if (!(m_Description.m_ResourceDimension == xiiGALResourceDimension::Texture2D || m_Description.m_ResourceDimension == xiiGALResourceDimension::Texture2DArray))
  {
    xiiLog::Error("Unsupported texture view type. Only Texture2D and arrays are supported.");
    return XII_FAILURE;
  }

  XII_ASSERT_DEV(m_Description.m_Format != xiiGALTextureFormat::Unknown, "");

  xiiGALDeviceD3D11*  pDeviceD3D11  = static_cast<xiiGALDeviceD3D11*>(m_pDevice);
  xiiGALTextureD3D11* pTextureD3D11 = static_cast<xiiGALTextureD3D11*>(m_pDevice->GetTexture(m_Description.m_hTexture));

  const auto& textureDescription = pTextureD3D11->GetDescription();

  XII_ASSERT_DEV(textureDescription.m_BindFlags.IsSet(xiiGALBindFlags::UnorderedAccess), "xiiGALBindFlags::UnorderedAccess is not set.");

  if (textureDescription.m_uiSampleCount > 1U)
  {
    xiiLog::Error("Unordered access views are not allowed for multi-sampled resources.");
    return XII_FAILURE;
  }

  D3D11_UNORDERED_ACCESS_VIEW_DESC viewDescription = {};
  viewDescription.Format                           = xiiD3D11TypeConversions::GetFormat(m_Description.m_Format);

  switch (m_Description.m_ResourceDimension)
  {
    case xiiGALResourceDimension::Texture1D:
    {
      viewDescription.ViewDimension      = D3D11_UAV_DIMENSION_TEXTURE1D;
      viewDescription.Texture1D.MipSlice = m_Description.m_uiMostDetailedMip;
    }
    break;
    case xiiGALResourceDimension::Texture1DArray:
    {
      viewDescription.ViewDimension                  = D3D11_UAV_DIMENSION_TEXTURE1DARRAY;
      viewDescription.Texture1DArray.ArraySize       = m_Description.m_uiArrayOrDepthSlicesCount;
      viewDescription.Texture1DArray.FirstArraySlice = m_Description.m_uiFirstArrayOrDepthSlice;
      viewDescription.Texture1DArray.MipSlice        = m_Description.m_uiMostDetailedMip;
    }
    break;
    case xiiGALResourceDimension::Texture2D:
    {
      viewDescription.ViewDimension      = D3D11_UAV_DIMENSION_TEXTURE2D;
      viewDescription.Texture2D.MipSlice = m_Description.m_uiMostDetailedMip;
    }
    break;
    case xiiGALResourceDimension::Texture2DArray:
    {
      viewDescription.ViewDimension                  = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
      viewDescription.Texture2DArray.ArraySize       = m_Description.m_uiArrayOrDepthSlicesCount;
      viewDescription.Texture2DArray.FirstArraySlice = m_Description.m_uiFirstArrayOrDepthSlice;
      viewDescription.Texture2DArray.MipSlice        = m_Description.m_uiMostDetailedMip;
    }
    break;
    case xiiGALResourceDimension::Texture3D:
    {
      viewDescription.ViewDimension         = D3D11_UAV_DIMENSION_TEXTURE3D;
      viewDescription.Texture3D.MipSlice    = m_Description.m_uiMostDetailedMip;
      viewDescription.Texture3D.FirstWSlice = m_Description.m_uiFirstArrayOrDepthSlice;
      viewDescription.Texture3D.WSize       = m_Description.m_uiArrayOrDepthSlicesCount;
    }
    break;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }

  if (FAILED(pDeviceD3D11->GetD3D11Device()->CreateUnorderedAccessView(pTextureD3D11->GetTexture(), &viewDescription, ppUnorderedAccessView)))
  {
    xiiLog::Error("Failed to create the Direct3D11 unordered access view.");
    return XII_FAILURE;
  }
  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_Resources_Implementation_TextureViewD3D11);
