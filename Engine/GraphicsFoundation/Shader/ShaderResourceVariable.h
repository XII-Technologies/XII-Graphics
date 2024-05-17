#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Shader/Shader.h>

/// brief This describes the type of shader resource variable.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALShaderResourceVariableType
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    Static = 0U, ///< Shader resource bound to the variable is the same for all shader resource binding instances. It must be set *once* directly through pipeline state object.
    Mutable,     ///< Shader resource bound to the variable is specific to the shader resource binding instance. It must be set *once* through the shader resource binding interface. It cannot be set through the pipeline state interface, and cannot be changed once bound.
    Dynamic,     ///< Shader variable binding is dynamic. It can be set multiple times for every instance of shader resource binding. It cannot be set through the pipeline state interface.

    ENUM_COUNT,

    Default = Static
  };
};

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALShaderResourceVariableType);

/// brief This describes the shader resource variable type flags.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALShaderResourceVariableTypeFlags
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    None    = 0U,                                                 ///< No flags.
    Static  = XII_BIT(xiiGALShaderResourceVariableType::Static),  ///< Static variable type flag.
    Mutable = XII_BIT(xiiGALShaderResourceVariableType::Mutable), ///< Mutable variable type flag.
    Dynamic = XII_BIT(xiiGALShaderResourceVariableType::Dynamic), ///< Dynamic variable type flag.

    MutableDynamic = Mutable | Dynamic,          ///< Mutable and dynamic variable type flags.
    All            = Static | Mutable | Dynamic, ///< All variable type flags.

    Default = None
  };

  struct Bits
  {
    StorageType Static : 1;
    StorageType Mutable : 1;
    StorageType Dynamic : 1;

    StorageType MutableDynamic : 1;
    StorageType All : 1;
  };
};

XII_DECLARE_FLAGS_OPERATORS(xiiGALShaderResourceVariableTypeFlags);

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALShaderResourceVariableTypeFlags);

/// brief This describes the shader resource bind flags.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALShaderResourceBindFlags
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    Static  = xiiGALShaderResourceVariableTypeFlags::Static,  ///< Indicates that static shader variable bindings are to be updated.
    Mutable = xiiGALShaderResourceVariableTypeFlags::Mutable, ///< Indicates that mutable shader variable bindings are to be updated.
    Dynamic = xiiGALShaderResourceVariableTypeFlags::Dynamic, ///< Indicates that dynamic shader variable bindings are to be updated.

    All = xiiGALShaderResourceVariableTypeFlags::All, ///< Indicates that all shader variable types (static, mutable and dynamic) are to be updated.
                                                      ///<
                                                      ///< \note If none of xiiGALShaderResourceBindFlags::Static, xiiGALShaderResourceBindFlags::Mutable, and xiiGALShaderResourceBindFlags::Dynamic flags are set, all variables are updated as if xiiGALShaderResourceBindFlags::All was set.

    KeepExisting = 0x08, ///< If this flag is specified, all existing bindings will be preserved and only unresolved ones will be updated. If this flag is not specified, every shader variable will be updated if the mapping contains corresponding resource.

    VerifyAllResolved = 0x10, ///< If this flag is specified, all shader bindings are expected to be resolved after the call. If this is not the case, debug message will be displayed.
                              ///<
                              ///< \note Only these variables are verified that are being updated by setting xiiGALShaderResourceBindFlags::Static, xiiGALShaderResourceBindFlags::Mutable, and xiiGALShaderResourceBindFlags::Dynamic.

    AllowOverWrite = 0x20, ///< Allow overwriting static and mutable variables.

    Default = Static
  };

  struct Bits
  {
    StorageType Static : 1;
    StorageType Mutable : 1;
    StorageType Dynamic : 1;

    StorageType All : 1;
    StorageType KeepExisting : 1;
    StorageType VerifyAllResolved : 1;
    StorageType AllowOverwrite : 1;
  };
};

XII_DECLARE_FLAGS_OPERATORS(xiiGALShaderResourceBindFlags);

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALShaderResourceBindFlags);

/// brief This describes the shader setting flags.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALSetShaderResourceFlags
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    None           = 0U,         ///< No flags.
    AllowOverwrite = XII_BIT(0), ///< Allow overwriting static and mutable variable bindings.
                                 ///< \remarks By default, static and mutable variables can't be changed once initialized to a non-null resource. This flag is required to explicitly allow overwriting the binding.
                                 ///<
                                 ///< Overwriting static variables does not require synchronization with GPU and does not have effect on shader resource binding objects already created from the pipeline state or resource signature.
                                 ///<
                                 ///< When overwriting a mutable variable binding in Direct3D12 and Vulkan, an application must ensure that the GPU is not accessing the shader resource binding. This can be achieved using syncrhonization tools such as fences.
                                 ///< Synchronization with GPU is not required in Direct3D11, and Metal backends.

    Default = None
  };

  struct Bits
  {
    StorageType AllowOverwrite : 1;
  };
};

XII_DECLARE_FLAGS_OPERATORS(xiiGALSetShaderResourceFlags);

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALSetShaderResourceFlags);

/// \brief Interface that defines methods to manipulate a shader resource variable object.
class XII_GRAPHICSFOUNDATION_DLL xiiGALShaderResourceVariable : public xiiGALObject
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALShaderResourceVariable, xiiGALObject);

public:
  /// \brief This binds a resource to the variable.
  ///
  /// \param pResource - A pointer to the resource to bind.
  /// \param flags     - Flags. See xiiGALSetShaderResourceFlags.
  ///
  /// \remark The method performs run-time correctness checks. For instance, shader resource view cannot be assigned to a constant buffer variable.
  virtual void Set(xiiGALResource* pResource, xiiBitflags<xiiGALSetShaderResourceFlags> flags = xiiGALSetShaderResourceFlags::None) = 0;

  /// \brief This binds a resource array to the variable.
  ///
  /// \param ppResources - A pointer to the array of resources.
  /// \param flags       - Flags. See xiiGALSetShaderResourceFlags.
  ///
  /// \remark The method performs run-time correctness checks. For instance, shader resource view cannot be assigned to a constant buffer variable.
  virtual void SetArray(xiiArrayPtr<xiiGALResource* const> ppResources, xiiBitflags<xiiGALSetShaderResourceFlags> flags = xiiGALSetShaderResourceFlags::None) = 0;

  /// \brief This binds a specified constant buffer range to the variable.
  ///
  /// \param pResource    - The pointer to the resource.
  /// \param uiOffset     - The offset in bytes to the start of the buffer range to bind.
  /// \param uiSize       - The size in bytes of the buffer range to bind.
  /// \param uiArrayIndex - For array variables, the index of the array element.
  /// \param              - flags Flags. See xiiGALSetShaderResourceFlags.
  ///
  /// \remarks This method is only allowed for constant buffers. If dynamic offset is further set by SetBufferOffset() method, it is added to the base offset set by this method.
  ///          The method resets dynamic offset previously set for this variable to zero.
  ///
  /// \warning The Offset must be an integer multiple of m_uiConstantBufferOffsetAlignment member specified by the device limits xiiGALDeviceLimits.
  virtual void SetBufferRange(xiiGALResource* pResource, xiiUInt64 uiOffset, xiiUInt64 uiSize, xiiUInt32 uiArrayIndex = 0U, xiiBitflags<xiiGALSetShaderResourceFlags> flags = xiiGALSetShaderResourceFlags::None) = 0;

  /// \brief This sets the constant or structured buffer dynamic offset.
  ///
  /// \param uiOffset     - The additional offset in bytes, that is added to the base offset (see remarks). Only 32-bit offsets are supported.
  /// \param uiArrayIndex - For array variables, the index of the array element.
  ///
  /// \remarks This method is only allowed for constant or structured buffer variables that were not created with xiiGALShaderVariableFlags::NoDynamicBuffers or xiiGALPipelineResourceFlags::NoDynamicBuffers.
  ///          The method is also not allowed for static resource variables.
  ///
  /// \warning The Offset must be an integer multiple of m_uiConstantBufferOffsetAlignment member when setting the offset for a constant buffer, or m_uiStructuredBufferOffsetAlignment when
  ///          setting the offset for a structured buffer, as specified by the device limits. (See xiiGALDeviceLimits)
  ///
  ///          For constant buffers, the offset is added to the offset that was previously set by SetBufferRange() method (if any). For structured buffers, the offset is added
  ///          to the base offset specified by the buffer view.
  ///
  ///          Changing the buffer offset does not require committing the SRB. From the engine point of view, buffers with dynamic offsets are treated similar to dynamic
  ///          buffers, and thus affected by xiiGALDrawFlagDynamicResourceBuffersIntact flag.
  virtual void SetBufferOffset(xiiUInt32 uiOffset, xiiUInt32 uiArrayIndex = 0U) = 0;

  /// \brief This returns the shader resource variable type.
  XII_NODISCARD virtual xiiEnum<xiiGALShaderResourceVariableType> GetType() const = 0;

  /// \brief This returns shader resource description.
  virtual void GetResourceDescription(xiiGALShaderResourceDescription& resourceDeccription) = 0;

  /// \brief This returns the variable index that can be used to access the variable.
  XII_NODISCARD virtual xiiUInt32 GetIndex() const = 0;

  /// \brief This returns a pointer to the resource that is bound to this variable.
  ///
  /// \brief uiArrayIndex The resource array index that can be used to access the variable. This must be 0 for non-array variables.
  XII_NODISCARD virtual xiiArrayPtr<xiiGALResource*> Get(xiiUInt32 uiIndex = 0U) const = 0;

protected:
  friend class xiiGALDevice;

  xiiGALShaderResourceVariable();

  virtual ~xiiGALShaderResourceVariable();
};

#include <GraphicsFoundation/Shader/Implementation/ShaderResourceVariable_inl.h>
