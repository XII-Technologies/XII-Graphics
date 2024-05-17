#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <GraphicsD3D11/Device/DeviceD3D11.h>
#include <GraphicsD3D11/Resources/BufferD3D11.h>

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALBufferD3D11, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALBufferD3D11::xiiGALBufferD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALBufferCreationDescription& creationDescription) :
  xiiGALBuffer(pDeviceD3D11, creationDescription)
{
}

xiiGALBufferD3D11::~xiiGALBufferD3D11() = default;

xiiResult xiiGALBufferD3D11::InitPlatform(const xiiGALBufferData* pInitialData)
{
  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);

  if (m_Description.m_ResourceUsage == xiiGALResourceUsage::Unified)
  {
    xiiLog::Error("Unified resources are not supported in Direct3D 11.");
    return XII_FAILURE;
  }

  D3D11_BUFFER_DESC bufferDescription = {};
  bufferDescription.BindFlags         = xiiD3D11TypeConversions::GetBindFlags(m_Description.m_BindFlags);
  bufferDescription.ByteWidth         = static_cast<xiiUInt32>(m_Description.m_uiSize);
  bufferDescription.Usage             = xiiD3D11TypeConversions::GetUsage(m_Description.m_ResourceUsage);
  bufferDescription.CPUAccessFlags    = xiiD3D11TypeConversions::GetCPUAccessFlags(m_Description.m_CPUAccessFlags);
  bufferDescription.MiscFlags         = 0U;
  bufferDescription.ByteWidth         = static_cast<xiiUInt32>(m_Description.m_uiSize);

  if (m_Description.m_BindFlags.IsSet(xiiGALBindFlags::IndirectDrawArguments))
  {
    bufferDescription.MiscFlags |= D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
  }
  if (m_Description.m_ResourceUsage == xiiGALResourceUsage::Sparse)
  {
    bufferDescription.MiscFlags |= D3D11_RESOURCE_MISC_TILED;
  }

  // Size of each element in the buffer structure (in bytes) when the buffer represents a structured buffer, or
  // the size of the format that is used for views of the buffer.
  bufferDescription.StructureByteStride = m_Description.m_uiElementByteStride;
  if (m_Description.m_BindFlags.IsSet(xiiGALBindFlags::UnorderedAccess) || m_Description.m_BindFlags.IsSet(xiiGALBindFlags::ShaderResource))
  {
    if (m_Description.m_Mode == xiiGALBufferMode::Structured)
    {
      bufferDescription.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

      XII_ASSERT_DEV(bufferDescription.StructureByteStride != 0, "StructureByteStride can not be zero for a structured buffer.");
    }
    else if (m_Description.m_Mode == xiiGALBufferMode::Formatted)
    {
      XII_ASSERT_DEV(bufferDescription.StructureByteStride != 0, "StructureByteStride can not be zero for a formatted buffer.");
    }
    else if (m_Description.m_Mode == xiiGALBufferMode::Raw)
    {
      bufferDescription.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

      XII_ASSERT_DEV(bufferDescription.StructureByteStride != 0, "StructureByteStride can not be zero for a formatted buffer.");
    }
    else
    {
      XII_ASSERT_DEV(false, "Unexpected buffer mode.");
    }
  }

  // If uniform/constant buffer, align size to 64 bytes.
  if (m_Description.m_BindFlags.IsSet(xiiGALBindFlags::UniformBuffer))
  {
    // Note that Direct3D11 does not allow partial updates of constant buffers with UpdateSubresource().
    // Only the entire buffer may be updated.
    bufferDescription.ByteWidth = (xiiUInt32)xiiMemoryUtils::AlignSize(m_Description.m_uiSize, 64ULL);
  }

  // Set the index format for index buffers.
  if (m_Description.m_BindFlags.IsSet(xiiGALBindFlags::IndexBuffer))
    m_IndexFormat = m_Description.m_uiElementByteStride == 2U ? xiiGALValueType::UInt16 : xiiGALValueType::UInt32;

  D3D11_SUBRESOURCE_DATA initialData = {};
  initialData.pSysMem                = (pInitialData && pInitialData->m_pData) ? pInitialData->m_pData : nullptr;
  initialData.SysMemPitch            = 0U;
  initialData.SysMemSlicePitch       = 0U;

  if (FAILED(pDeviceD3D11->GetD3D11Device()->CreateBuffer(&bufferDescription, initialData.pSysMem ? &initialData : nullptr, &m_pBuffer)))
  {
    xiiLog::Error("Failed to create D3D11 buffer.");
    return XII_FAILURE;
  }
  return XII_SUCCESS;
}

xiiResult xiiGALBufferD3D11::DeInitPlatform()
{
  XII_GAL_D3D11_RELEASE(m_pBuffer);

  return XII_SUCCESS;
}

void xiiGALBufferD3D11::SetDebugNamePlatform(xiiStringView sName)
{
  if (m_pBuffer != nullptr)
  {
    xiiStringBuilder sb;
    if (FAILED(m_pBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, sName.GetElementCount(), sName.GetData(sb))))
    {
      xiiLog::Error("Failed to set the Direct3D11 input layout debug name.");
    }
  }
}

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_Resources_Implementation_BufferD3D11);
