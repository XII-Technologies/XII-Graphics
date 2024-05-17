#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Resources/BufferView.h>

/// \brief This describes the buffer access mode.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALBufferMode
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    Undefined = 0U, ///< Undefined buffer access mode.
    Formatted,      ///< Formatted buffer. Access to the buffer will use format conversion operations. In this mode, the element byte stride member of the buffer description defines the buffer element size. Buffer views can use different formats, but the format size must match the element byte stride.
    Structured,     ///< Structured buffer. In this mode, the element byte stride member of the buffer description defines the structure stride.
    Raw,            ///< Raw buffer. In this mode, the buffer is accessed as raw bytes. Formatted views of a raw buffer can also be created similar to formatted buffer. If formatted views are to be created, the element byte stride member of the buffer description must specify the size of the format.

    ENUM_COUNT,

    Default = Undefined
  };
};

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALBufferMode);

/// \brief This describes the miscellaneous buffer flags.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALMiscBufferFlags
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    None        = 0U,         ///< No miscellaneous buffer flags.
    SparseAlias = XII_BIT(0), ///< For a sparse buffer, allow binding the same memory region in different buffer ranges or in different sparse buffers.

    ENUM_COUNT,

    Default = None
  };

  struct Bits
  {
    StorageType SparseAlias : 1;
  };
};

XII_DECLARE_FLAGS_OPERATORS(xiiGALMiscBufferFlags);

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALMiscBufferFlags);

/// \brief This describes the buffer creation description.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALBufferCreationDescription : public xiiHashableStruct<xiiGALBufferCreationDescription>
{
  XII_DECLARE_POD_TYPE();

  xiiStringView                      m_sName;                                                 ///< Resource name. The default is an empty string view.
  xiiUInt64                          m_uiSize                 = 0U;                           ///< The size of the buffer in bytes. For a uniform (constant) buffer, this must be a multiple of 16. The default is 0.
  xiiBitflags<xiiGALBindFlags>       m_BindFlags              = xiiGALBindFlags::None;        ///< The bind flags. Allowed flags are Vertex, Index, Uniform (Constant), Shader Resource, Stream Output, Unordered Access, Indirect Draw Args, Ray Tracing. Allowed flags for sparse resources are stored in the allowed sparse resource properties. The default is None.
  xiiEnum<xiiGALResourceUsage>       m_ResourceUsage          = xiiGALResourceUsage::Default; ///< The resource usage. The default is Default.
  xiiBitflags<xiiGALCPUAccessFlag>   m_CPUAccessFlags         = xiiGALCPUAccessFlag::None;    ///< The CPU access flags or None if no CPU access is allowed. The default is None.
  xiiEnum<xiiGALBufferMode>          m_Mode                   = xiiGALBufferMode::Undefined;  ///< The buffer mode. The default is Undefined.
  xiiBitflags<xiiGALMiscBufferFlags> m_MiscFlags              = xiiGALMiscBufferFlags::None;  ///< The miscellaneous flags. The default is None.
  xiiUInt32                          m_uiElementByteStride    = 0U;                           ///< The buffer element stride in bytes. For a structured buffer, this member defines the size of each buffer element. For a formatted buffer and optionally a raw buffer, this member defines the size of the format that will be used for views created for this buffer. For an index buffer, a stride of 2 will set the index buffer format to 16-bits. The default is 0.
  xiiUInt64                          m_uiImmediateContextMask = XII_BIT(0);                   ///< Defines which immediate contexts are allowed to execute commands that use this buffer. The default is the main immediate context.
                                                                                              ///< Only specify the bits that indicate those immediate contexts where the resource will be used, setting unnecessary bits will result in extra overhead.
};

/// \brief This describes the buffer initial data.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALBufferData : public xiiHashableStruct<xiiGALBufferData>
{
  XII_DECLARE_POD_TYPE();

  const void* m_pData      = nullptr; ///< The pointer to the data.
  xiiUInt64   m_uiDataSize = 0U;      ///< The data size in bytes.

  /// \todo GraphicsFoundation: Add command encoder which will be used to initialize the buffer.
};

/// \brief This describes the sparse buffer properties.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALSparseBufferProperties : public xiiHashableStruct<xiiGALSparseBufferProperties>
{
  XII_DECLARE_POD_TYPE();

  xiiUInt64 m_uiAddressSpaceSize = 0U; ///< The size of the sparse buffer virtual address space.
  xiiUInt32 m_uiBlockSize        = 0U; ///< The size of the sparse memory block.
                                       ///<
                                       ///< \note Offset in the buffer, memory offset and memory size that are used in sparse resource binding command, must be multiples of the block size. In Direct3D11 and Direct3D12, the block size is always 64Kb. In Vulkan, the block size is not documented, but is usually also 64Kb.
};

/// \brief Interface that defines methods to manipulate a buffer object.
class XII_GRAPHICSFOUNDATION_DLL xiiGALBuffer : public xiiGALResource
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALBuffer, xiiGALResource);

public:
  /// \brief This returns the creation description for this object.
  XII_NODISCARD const xiiGALBufferCreationDescription& GetDescription() const;

  /// \brief This returns the handle of the default view.
  ///
  /// \param viewType - The type of the requested view. See xiiGALBufferViewType.
  ///
  /// \return The handle to the buffer view.
  ///
  /// \remarks Default views are only created for structured and raw buffers. As for formatted buffers the view format is unknown at buffer initialization time, no default views are created.
  ///
  /// \note The function does not increase the reference counter for the returned interface, so ReleaseRef() must *NOT* be called.
  XII_NODISCARD xiiGALBufferViewHandle GetDefaultView(xiiEnum<xiiGALBufferViewType> viewType);

  /// \brief This sets the buffer usage state.
  ///
  /// \note This method does not perform state transition, but resets the internal buffer state to the given value.
  ///       This method should be used after the application finished manually managing the buffer state and wants to hand over state management back to the engine.
  virtual void SetState(xiiBitflags<xiiGALResourceStateFlags> stateFlags) = 0;

  /// \brief This returns the internal buffer state.
  XII_NODISCARD virtual xiiBitflags<xiiGALResourceStateFlags> GetState() const = 0;

  /// \brief This returns the buffer memory properties.
  ///
  /// The memory properties are only relevant for persistently mapped buffers.
  /// In particular, if the memory is not coherent, an application must call xiiGALBuffer::FlushMappedRange() to make writes by the CPU available to the GPU, and
  /// call xiiGALBuffer::InvalidateMappedRange() to make writes by the GPU visible to the CPU.
  XII_NODISCARD virtual xiiGALMemoryProperties GetMemoryProperties() const = 0;

  /// \brief This flushes the specified range of non-coherent memory from the host cache to make it available to the GPU.
  ///
  /// \param uiStartOffset - The offset in bytes from the beginning of the buffer to the start of the memory range to flush.
  /// \param uiSize        - The size in bytes of the memory range to flush.
  ///
  /// This method should only be used for persistently-mapped buffers that do not report the xiiGALMemoryProperties::HostCoherent property. After an application modifies
  /// a mapped memory range on the CPU, it must flush the range to make it available to the GPU.
  ///
  /// \note This method must not be called for Dynamic buffers. When a mapped buffer is unmapped, it is automatically flushed by the engine if necessary.
  virtual void FlushMappedRange(xiiUInt64 uiStartOffset, xiiUInt64 uiSize) = 0;

  /// \brief This invalidates the specified range of non-coherent memory modified by the GPU to make it visible to the CPU.
  ///
  /// \param uiStartOffset - The offset in bytes from the beginning of the buffer to the start of the memory to invalidate.
  /// \param uiSize        - The size in bytes of the memory range to invalidate.
  ///
  /// This method should only be used for persistently-mapped buffers that do not report the xiiGALMemoryProperties::HostCoherent property. After an application modifies
  /// a mapped memory range on the CPU, it must invalidate the range to make it visible to the CPU.
  ///
  /// \note This method must not be called for Dynamic buffers. When a mapped buffer is unmapped, it is automatically flushed by the engine if necessary.
  virtual void InvalidateMappedRange(xiiUInt64 uiStartOffset, xiiUInt64 uiSize) = 0;

  /// \brief This returns the sparse buffer memory properties.
  XII_NODISCARD virtual xiiGALSparseBufferProperties GetSparseProperties() const = 0;

protected:
  friend class xiiGALDevice;

  xiiGALBuffer(xiiGALDevice* pDevice, const xiiGALBufferCreationDescription& creationDescription);

  virtual ~xiiGALBuffer();

  virtual xiiResult InitPlatform(const xiiGALBufferData* pInitialData) = 0;

  virtual xiiResult DeInitPlatform() = 0;

protected:
  xiiGALBufferCreationDescription m_Description;

private:
  void CreateDefaultResourceViews(xiiGALBufferHandle hBuffer);
};

#include <GraphicsFoundation/Resources/Implementation/Buffer_inl.h>
