#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Declarations/Descriptors.h>
#include <GraphicsFoundation/Device/Device.h>
#include <GraphicsFoundation/Resources/Buffer.h>

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
  static XII_NODISCARD XII_ALWAYS_INLINE xiiGALBufferHandle CreateIndexBuffer(xiiGALDevice* pDevice, IndexType indexType, xiiUInt32 uiIndexCount, xiiArrayPtr<xiiUInt8> pInitialData = xiiArrayPtr<xiiUInt8>(), bool bDataIsMutable = false)
  {
    XII_ASSERT_DEV(pDevice != nullptr, "Invalid device provided.");

    const bool bIsImmutable = (!pInitialData.IsEmpty() && !bDataIsMutable);

    xiiUInt32 uiIndexSize = 0;
    if (indexType == IndexType::UShort)
      uiIndexSize = sizeof(xiiUInt16);
    else if (indexType == IndexType::UInt)
      uiIndexSize = sizeof(xiiUInt32);

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
};
