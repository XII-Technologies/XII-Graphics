#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Declarations/Descriptors.h>
#include <GraphicsFoundation/Device/Device.h>
#include <GraphicsFoundation/Resources/Buffer.h>
#include <GraphicsFoundation/Resources/Texture.h>

/// \brief Provides utility functions that are common with interfacing with the GAL device.
class XII_GRAPHICSFOUNDATION_DLL xiiGALDeviceUtilities
{
public:
  enum class IndexType
  {
    None,   ///< Indices are not used, vertices are only used to form primitives.
    UShort, ///< 16 bit indices are used to select which vertices shall form a primitive, thus meshes can only use up to 65535 vertices.
    UInt,   ///< 32 bit indices are used to select which vertices shall form a primitive.
  };

  /// \brief Creates a vertex buffer with the given vertex size and vertex count.
  ///
  /// \param pDevice        - The device associated with the buffer.
  /// \param uiVertexSize   - The size of a single vertex in the buffer.
  /// \param uiVertexCount  - The number of vertices in the buffer.
  /// \param pInitialData   - The initial data in bytes, that the buffer should contain after creation.
  /// \param bDataIsMutable - Specifies whether the buffer should be considered immutable in its usage.
  static XII_NODISCARD XII_ALWAYS_INLINE xiiGALBufferHandle CreateVertexBuffer(xiiGALDevice* pDevice, xiiUInt32 uiVertexSize, xiiUInt32 uiVertexCount, xiiArrayPtr<xiiUInt8> pInitialData = xiiArrayPtr<xiiUInt8>(), bool bDataIsMutable = false)
  {
    XII_ASSERT_DEV(pDevice != nullptr, "Invalid device provided.");

    const bool bIsImmutable = (!pInitialData.IsEmpty() && !bDataIsMutable);

    xiiGALBufferCreationDescription bufferDescription;
    bufferDescription.m_BindFlags           = xiiGALBindFlags::VertexBuffer;
    bufferDescription.m_uiElementByteStride = uiVertexSize;
    bufferDescription.m_uiSize              = uiVertexSize * xiiMath::Max(1U, uiVertexCount);
    bufferDescription.m_ResourceUsage       = bIsImmutable ? xiiGALResourceUsage::Immutable : xiiGALResourceUsage::Dynamic;
    bufferDescription.m_CPUAccessFlags      = bIsImmutable ? xiiGALCPUAccessFlag::None : xiiGALCPUAccessFlag::Write;

    xiiGALBufferData initialData;
    initialData.m_pData      = pInitialData.GetPtr();
    initialData.m_uiDataSize = pInitialData.GetCount();

    return pDevice->CreateBuffer(bufferDescription, &initialData);
  }

  /// \brief Creates an index buffer with the given index type and index count.
  ///
  /// \param pDevice        - The device associated with the buffer.
  /// \param indexType      - The index buffer type. See xiiGALDeviceUtilities::IndexType for details.
  /// \param uiIndexCount   - The number of indices in the buffer.
  /// \param pInitialData   - The initial data in bytes, that the buffer should contain after creation.
  /// \param bDataIsMutable - Specifies whether the buffer should be considered immutable in its usage.
  static XII_NODISCARD XII_ALWAYS_INLINE xiiGALBufferHandle CreateIndexBuffer(xiiGALDevice* pDevice, IndexType indexType, xiiUInt32 uiIndexCount, xiiArrayPtr<xiiUInt8> pInitialData = xiiArrayPtr<xiiUInt8>(), bool bDataIsMutable = false)
  {
    XII_ASSERT_DEV(pDevice != nullptr, "Invalid device provided.");

    const bool bIsImmutable = (!pInitialData.IsEmpty() && !bDataIsMutable);

    xiiUInt32 uiIndexSize = 0;
    if (indexType == IndexType::UShort)
      uiIndexSize = sizeof(xiiUInt16);
    else if (indexType == IndexType::UInt)
      uiIndexSize = sizeof(xiiUInt32);

    XII_ASSERT_DEV(uiIndexCount != 0U, "Implementation Error: Unexpected index size.");

    xiiGALBufferCreationDescription bufferDescription;
    bufferDescription.m_BindFlags           = xiiGALBindFlags::IndexBuffer;
    bufferDescription.m_uiElementByteStride = uiIndexSize;
    bufferDescription.m_uiSize              = uiIndexSize * xiiMath::Max(1U, uiIndexCount);
    bufferDescription.m_ResourceUsage       = bIsImmutable ? xiiGALResourceUsage::Immutable : xiiGALResourceUsage::Dynamic;
    bufferDescription.m_CPUAccessFlags      = bIsImmutable ? xiiGALCPUAccessFlag::None : xiiGALCPUAccessFlag::Write;

    xiiGALBufferData initialData;
    initialData.m_pData      = pInitialData.GetPtr();
    initialData.m_uiDataSize = pInitialData.GetCount();

    return pDevice->CreateBuffer(bufferDescription, &initialData);
  }

  /// \brief Creates a constant buffer with the given size.
  ///
  /// \param  pDevice     - The device associated with the buffer.
  /// \param uiBufferSize - The size of the buffer in bytes.
  static XII_NODISCARD XII_ALWAYS_INLINE xiiGALBufferHandle CreateConstantBuffer(xiiGALDevice* pDevice, xiiUInt32 uiBufferSize)
  {
    XII_ASSERT_DEV(pDevice != nullptr, "Invalid device provided.");

    xiiGALBufferCreationDescription bufferDescription;
    bufferDescription.m_BindFlags           = xiiGALBindFlags::UniformBuffer;
    bufferDescription.m_uiElementByteStride = 0U;
    bufferDescription.m_uiSize              = uiBufferSize;
    bufferDescription.m_ResourceUsage       = xiiGALResourceUsage::Dynamic;
    bufferDescription.m_CPUAccessFlags      = xiiGALCPUAccessFlag::Write;

    return pDevice->CreateBuffer(bufferDescription);
  }

  /// \brief Creates a render target description with the given paramters.
  ///
  /// \param size          - The size (width and height) of the render target.
  /// \param format        - The render target format. See xiiGALTextureFormat for details.
  /// \param uiSampleCount - The number of samples in the render target. The default is xiiGALMSAASampleCount::OneSample.
  static XII_NODISCARD XII_ALWAYS_INLINE xiiGALTextureCreationDescription CreateRenderTargetDescription(xiiSizeU32 size, xiiGALTextureFormat::Enum format, xiiUInt32 uiSampleCount = xiiGALMSAASampleCount::OneSample)
  {
    return xiiGALTextureCreationDescription{
      .m_Type               = xiiGALResourceDimension::Texture2D,
      .m_Size               = size,
      .m_uiArraySizeOrDepth = 1U,
      .m_Format             = format,
      .m_uiMipLevels        = 1U,
      .m_uiSampleCount      = uiSampleCount,
      .m_BindFlags          = xiiGALBindFlags::ShaderResource | (xiiGALTextureFormat::IsDepthFormat(format) ? xiiGALBindFlags::DepthStencil : xiiGALBindFlags::RenderTarget),
      .m_Usage              = xiiGALResourceUsage::Default,
      .m_MiscFlags          = xiiGALMiscTextureFlags::None,
    };
  }
};
