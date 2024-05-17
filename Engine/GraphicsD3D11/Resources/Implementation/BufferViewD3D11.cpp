#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <GraphicsD3D11/Device/DeviceD3D11.h>
#include <GraphicsD3D11/Resources/BufferD3D11.h>
#include <GraphicsD3D11/Resources/BufferViewD3D11.h>

#include <d3d11_2.h>

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALBufferViewD3D11, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALBufferViewD3D11::xiiGALBufferViewD3D11(xiiGALDeviceD3D11* pDeviceD3D11, xiiGALBuffer* pBuffer, const xiiGALBufferViewCreationDescription& creationDescription) :
  xiiGALBufferView(pDeviceD3D11, pBuffer, creationDescription)
{
}

xiiGALBufferViewD3D11::~xiiGALBufferViewD3D11() = default;

xiiResult xiiGALBufferViewD3D11::InitPlatform()
{
  switch (m_Description.m_ViewType)
  {
    case xiiGALBufferViewType::ShaderResource:
    {
      ID3D11ShaderResourceView* pShaderResourceView = nullptr;

      XII_SUCCEED_OR_RETURN(CreateSRV(&pShaderResourceView));

      m_pBufferView = pShaderResourceView;
    }
    break;
    case xiiGALBufferViewType::UnorderedAccess:
    {
      ID3D11UnorderedAccessView* pUnorderedAccessView = nullptr;

      XII_SUCCEED_OR_RETURN(CreateUAV(&pUnorderedAccessView));

      m_pBufferView = pUnorderedAccessView;
    }
    break;

      XII_DEFAULT_CASE_NOT_IMPLEMENTED;
  }
  return XII_SUCCESS;
}

xiiResult xiiGALBufferViewD3D11::DeInitPlatform()
{
  XII_GAL_D3D11_RELEASE(m_pBufferView);

  return XII_SUCCESS;
}

void xiiGALBufferViewD3D11::SetDebugNamePlatform(xiiStringView sName)
{
  if (m_pBufferView != nullptr)
  {
    xiiStringBuilder sb;
    if (FAILED(m_pBufferView->SetPrivateData(WKPDID_D3DDebugObjectName, sName.GetElementCount(), sName.GetData(sb))))
    {
      xiiLog::Error("Failed to set the Direct3D11 input layout debug name.");
    }
  }
}

xiiResult xiiGALBufferViewD3D11::CreateSRV(ID3D11ShaderResourceView** ppShaderResourceView)
{
  XII_ASSERT_DEV(m_Description.m_ViewType == xiiGALBufferViewType::ShaderResource, "Incorrect view type, expected shader resource view.");

  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);
  xiiGALBufferD3D11* pBufferD3D11 = static_cast<xiiGALBufferD3D11*>(m_pDevice->GetBuffer(m_Description.m_hBuffer));

  D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDescription = {};
  shaderResourceViewDescription.ViewDimension                   = D3D_SRV_DIMENSION_BUFFER;

  const auto& bufferDescription = pBufferD3D11->GetDescription();

  if (bufferDescription.m_Mode == xiiGALBufferMode::Raw && m_Description.m_Format.m_ValueType == xiiGALValueType::Undefined)
  {
    // Raw buffer view.
    xiiUInt32 uiElementByteStride = 4U;

    XII_ASSERT_DEV((m_Description.m_uiByteOffset % 16U) == 0, "Byte offset ({0}) is not a multiple of 16.", m_Description.m_uiByteOffset);
    XII_ASSERT_DEV((m_Description.m_uiByteWidth % uiElementByteStride) == 0, "Byte width ({0}) is not a multiple of 16.", m_Description.m_uiByteWidth);

    shaderResourceViewDescription.BufferEx.FirstElement = static_cast<xiiUInt32>(m_Description.m_uiByteOffset / uiElementByteStride);
    shaderResourceViewDescription.BufferEx.NumElements  = static_cast<xiiUInt32>(m_Description.m_uiByteWidth / uiElementByteStride);
    shaderResourceViewDescription.BufferEx.Flags        = D3D11_BUFFEREX_SRV_FLAG_RAW;
    shaderResourceViewDescription.Format                = DXGI_FORMAT_R32_TYPELESS;
    shaderResourceViewDescription.ViewDimension         = D3D_SRV_DIMENSION_BUFFEREX;
  }
  else
  {
    if ((bufferDescription.m_Mode == xiiGALBufferMode::Formatted) || (bufferDescription.m_Mode == xiiGALBufferMode::Raw && m_Description.m_Format.m_ValueType != xiiGALValueType::Undefined))
    {
      shaderResourceViewDescription.Format = xiiD3D11TypeConversions::GetDXGIFormatFromType(m_Description.m_Format.m_ValueType, m_Description.m_Format.m_uiComponents, m_Description.m_Format.m_bIsNormalized);
    }

    xiiUInt32 uiElementByteStride = 0U;
    if ((bufferDescription.m_Mode == xiiGALBufferMode::Formatted) || (bufferDescription.m_Mode == xiiGALBufferMode::Structured) || (bufferDescription.m_Mode == xiiGALBufferMode::Raw && m_Description.m_Format.m_ValueType != xiiGALValueType::Undefined))
    {
      uiElementByteStride = bufferDescription.m_uiElementByteStride;
    }
    else if (bufferDescription.m_Mode == xiiGALBufferMode::Raw && m_Description.m_Format.m_ValueType == xiiGALValueType::Undefined)
    {
      uiElementByteStride = 4U;
    }

    if (uiElementByteStride != 0)
    {
      XII_ASSERT_DEV((m_Description.m_uiByteOffset % uiElementByteStride) == 0, "Byte offset ({0}) is not a multiple of the element byte stride.", m_Description.m_uiByteOffset);
      XII_ASSERT_DEV((m_Description.m_uiByteWidth % uiElementByteStride) == 0, "Byte width ({0}) is not a multiple of the element byte stride.", m_Description.m_uiByteWidth);

      shaderResourceViewDescription.Buffer.FirstElement = static_cast<xiiUInt32>(m_Description.m_uiByteOffset / uiElementByteStride);
      shaderResourceViewDescription.Buffer.NumElements  = static_cast<xiiUInt32>(m_Description.m_uiByteWidth / uiElementByteStride);
    }
  }

  if (FAILED(pDeviceD3D11->GetD3D11Device()->CreateShaderResourceView(pBufferD3D11->GetBuffer(), &shaderResourceViewDescription, ppShaderResourceView)))
  {
    xiiLog::Error("Failed to create shader resource view.");
    return XII_FAILURE;
  }
  return XII_SUCCESS;
}

xiiResult xiiGALBufferViewD3D11::CreateUAV(ID3D11UnorderedAccessView** ppUnorderedAccessView)
{
  XII_ASSERT_DEV(m_Description.m_ViewType == xiiGALBufferViewType::UnorderedAccess, "Incorrect view type, expected an unordered access view.");

  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);
  xiiGALBufferD3D11* pBufferD3D11 = static_cast<xiiGALBufferD3D11*>(m_pDevice->GetBuffer(m_Description.m_hBuffer));

  D3D11_UNORDERED_ACCESS_VIEW_DESC unorderedAccessViewDescription = {};
  unorderedAccessViewDescription.ViewDimension                    = D3D11_UAV_DIMENSION_BUFFER;

  const auto& bufferDescription = pBufferD3D11->GetDescription();

  if ((bufferDescription.m_Mode == xiiGALBufferMode::Formatted) || (bufferDescription.m_BindFlags == xiiGALBufferMode::Raw && m_Description.m_Format.m_ValueType != xiiGALValueType::Undefined))
  {
    unorderedAccessViewDescription.Format = xiiD3D11TypeConversions::GetDXGIFormatFromType(m_Description.m_Format.m_ValueType, m_Description.m_Format.m_uiComponents, m_Description.m_Format.m_bIsNormalized);
  }

  xiiUInt32 uiElementByteStride = 0U;
  if ((bufferDescription.m_Mode == xiiGALBufferMode::Formatted) || (bufferDescription.m_Mode == xiiGALBufferMode::Structured) || (bufferDescription.m_Mode == xiiGALBufferMode::Raw && m_Description.m_Format.m_ValueType != xiiGALValueType::Undefined))
  {
    uiElementByteStride = bufferDescription.m_uiElementByteStride;
  }
  else if (bufferDescription.m_Mode == xiiGALBufferMode::Raw && m_Description.m_Format.m_ValueType == xiiGALValueType::Undefined)
  {
    uiElementByteStride = 4U;
  }

  if (uiElementByteStride != 0)
  {
    XII_ASSERT_DEV((m_Description.m_uiByteOffset % uiElementByteStride) == 0, "Byte offset ({0}) is not a multiple of the element byte stride.", m_Description.m_uiByteOffset);
    XII_ASSERT_DEV((m_Description.m_uiByteWidth % uiElementByteStride) == 0, "Byte width ({0}) is not a multiple of the element byte stride.", m_Description.m_uiByteWidth);

    unorderedAccessViewDescription.Buffer.FirstElement = static_cast<xiiUInt32>(m_Description.m_uiByteOffset / uiElementByteStride);
    unorderedAccessViewDescription.Buffer.NumElements  = static_cast<xiiUInt32>(m_Description.m_uiByteWidth / uiElementByteStride);
  }

  if (bufferDescription.m_Mode == xiiGALBufferMode::Raw && m_Description.m_Format.m_ValueType != xiiGALValueType::Undefined)
  {
    unorderedAccessViewDescription.Format       = DXGI_FORMAT_R32_TYPELESS;
    unorderedAccessViewDescription.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
  }
  else
  {
    unorderedAccessViewDescription.Buffer.Flags = 0;
  }

  if (FAILED(pDeviceD3D11->GetD3D11Device()->CreateUnorderedAccessView(pBufferD3D11->GetBuffer(), &unorderedAccessViewDescription, ppUnorderedAccessView)))
  {
    xiiLog::Error("Failed to create unordered access view.");
    return XII_FAILURE;
  }
  return XII_SUCCESS;
}

xiiGALSparseBufferProperties xiiGALBufferD3D11::GetSparseProperties() const
{
  XII_ASSERT_DEV(m_Description.m_ResourceUsage == xiiGALResourceUsage::Sparse, "xiiGALBuffer::GetSparseProperties() should only be used for sparse buffer.");

  xiiGALDeviceD3D11* pDeviceD3D11   = static_cast<xiiGALDeviceD3D11*>(m_pDevice);
  ID3D11Device2*     pDeviceD3D11_2 = static_cast<ID3D11Device2*>(pDeviceD3D11->GetD3D11Device());

  xiiUInt32        uiTileCountForEntireResource      = 0;
  D3D11_TILE_SHAPE standardTileShapeForNonPackedMips = {};
  pDeviceD3D11_2->GetResourceTiling(m_pBuffer, &uiTileCountForEntireResource, nullptr, &standardTileShapeForNonPackedMips, nullptr, 0, nullptr);

  XII_ASSERT_DEV(standardTileShapeForNonPackedMips.WidthInTexels == D3D11_2_TILED_RESOURCE_TILE_SIZE_IN_BYTES, "Expected a standard block size.");

  return xiiGALSparseBufferProperties{.m_uiAddressSpaceSize = xiiUInt64{uiTileCountForEntireResource} * standardTileShapeForNonPackedMips.WidthInTexels,
                                      .m_uiBlockSize        = standardTileShapeForNonPackedMips.WidthInTexels};
}

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_Resources_Implementation_BufferViewD3D11);
