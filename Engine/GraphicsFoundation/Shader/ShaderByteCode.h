#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Shader/InputLayout.h>

/// \brief This describes the shader resource type.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALShaderResourceType
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    Unknown = 0U,          ///< The shader resource type is unknown.
    ConstantBuffer,        ///< Constant (uniform) buffer.
    TextureSRV,            ///< Shader resource view of a texture (sampled image).
    BufferSRV,             ///< Shader resource view of a buffer (read-only storage image).
    TextureUAV,            ///< Unordered access view of a texture (storage image).
    BufferUAV,             ///< Unordered access view of a buffer (storage buffer).
    Sampler,               ///< Sampler (separate sampler).
    InputAttachment,       ///< Input attachment in a render pass.
    AccelerationStructure, ///< Acceleration structure.

    ENUM_COUNT,

    Default = Unknown
  };
};

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALShaderResourceType);

/// \brief This describes texture type of the shader resource type.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALShaderTextureType
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    Unknown = 0U,     ///< The shader resource texture type is unknown.
    Texture1D,        ///< Represents a one-dimensional texture.
    Texture1DArray,   ///< Represents an array of one-dimensional textures.
    Texture2D,        ///< Represents a two-dimensional texture.
    Texture2DArray,   ///< Represents an array of two-dimensional textures.
    Texture2DMS,      ///< Represents a two-dimensional multisample texture.
    Texture2DMSArray, ///< Represents an array of two-dimensional multisample textures.
    Texture3D,        ///< Represents a three-dimensional texture.
    TextureCube,      ///< Represents a cube texture.
    TextureCubeArray, ///< Represents an array of cube textures.

    ENUM_COUNT,

    Default = Unknown
  };
};

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALShaderTextureType);

/// \brief This describes the primitive type of a shader code variable.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALShaderPrimitiveType
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    Unknown = 0U, ///< The primitive type is unknown.
    Void,         ///< Void pointer.
    Bool,         ///< Boolean (bool).
    Int8,         ///< 8-bit integer (int8).
    Int16,        ///< 16-bit integer (int16).
    Int32,        ///< 32-bit integer (int).
    Int64,        ///< 64-bit integer (int64).
    UInt8,        ///< 8-bit unsigned integer (uint8).
    UInt16,       ///< 16-bit unsigned integer (uint16).
    UInt32,       ///< 32-bit unsigned integer (uint).
    UInt64,       ///< 64-bit unsigned integer (uint64).
    Float16,      ///< 16-bit floating-point number (half).
    Float32,      ///< 32-bit floating-point number (float).
    Double,       ///< Double-precision (64-bit) floating-point number (double).
    Min8Float,    ///< 8-bit float (min8float).
    Min10Float,   ///< 10-bit float (min10float).
    Min16Float,   ///< 16-bit float (min16float).
    Min12Int,     ///< 12-bit int (min12int).
    Min16Int,     ///< 16-bit int (min16int).
    Min16UInt,    ///< 16-bit unsigned int (min12uint).
    String,       ///< String (string).

    ENUM_COUNT,

    Default = Unknown
  };

  /// \brief This returns the size of the shader primitive data type.
  static xiiUInt32 GetPrimitiveTypeSize(xiiEnum<xiiGALShaderPrimitiveType> type);

  /// \brief This returns true if the given shader primitive data type is a number representation, else false.
  static bool IsNumberType(xiiEnum<xiiGALShaderPrimitiveType> type);
};

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALShaderPrimitiveType);

/// \brief This describes the class type of a shader code variable.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALShaderVariableClassType
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    Unknown = 0U,  ///< The class type is unknown.
    Scalar,        ///< The variable is a scalar.
    Array,         ///< The variable is an array.
    MatrixRows,    ///< The variable is a row-major matrix.
    MatrixColumns, ///< The variable is a column-major matrix.
    Struct,        ///< The variable is a structure.

    ENUM_COUNT,

    Default = Unknown
  };
};

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALShaderVariableClassType);

/// \brief This describes shader resource.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALShaderResourceDescription : public xiiHashableStruct<xiiGALShaderResourceDescription>
{
  XII_DECLARE_MEM_RELOCATABLE_TYPE();

  xiiHashedString                                  m_sName;                                               ///< The shader resource name.
  xiiEnum<xiiGALShaderResourceType>                m_Type            = xiiGALShaderResourceType::Unknown; ///< The shader resource type. The default is xiiGALShaderResourceType::Unknown.
  xiiEnum<xiiGALShaderTextureType>                 m_TextureType     = xiiGALShaderTextureType::Unknown;  ///< The shader resource texture type for Texture resources. The default is xiiGALShaderTextureType::Unknown.
  xiiUInt32                                        m_uiArraySize     = 0U;                                ///< The array size. For a non-array resource this value should be 1.
  xiiUInt32                                        m_uiBindIndex     = xiiInvalidIndex;                   ///< The resource binding index in the shader.
  xiiUInt32                                        m_uiDescriptorSet = xiiInvalidIndex;                   ///< The descriptor set to which this resource belongs to.
  xiiBitflags<xiiGALShaderStage>                   m_ShaderStages    = xiiGALShaderStage::Unknown;        ///< The shader stages to which this resource is bound.
  xiiUInt32                                        m_uiTotalSize     = 0U;                                ///< For a resource with variables, the total size of the resource.
  xiiDynamicArray<xiiGALShaderVariableDescription> m_Variables;                                           ///< An array of member variables for shader constant buffer, or push constants.
};

/// \brief This describes shader code variable.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALShaderVariableDescription : public xiiHashableStruct<xiiGALShaderVariableDescription>
{
  XII_DECLARE_MEM_RELOCATABLE_TYPE();

  xiiHashedString                                  m_sName;                                                  ///< The variable name.
  xiiEnum<xiiGALShaderVariableClassType>           m_Class         = xiiGALShaderVariableClassType::Unknown; ///< The variable class.
  xiiEnum<xiiGALShaderPrimitiveType>               m_PrimitiveType = xiiGALShaderPrimitiveType::Unknown;     ///< The variable primitive data type..
  xiiUInt8                                         m_uiRowCount    = 0U;                                     ///< For a matrix type, the number of rows.
  xiiUInt8                                         m_uiColumnCount = 0U;                                     ///< For a matrix type, the number of columns. For a vector type, the number of components.
  xiiUInt32                                        m_uiOffset      = 0U;                                     ///< The offset in bytes between the start of the parent structure and this variable.
  xiiUInt32                                        m_uiArraySize   = 0U;                                     ///< The array size.
  xiiDynamicArray<xiiGALShaderVariableDescription> m_Members;                                                ///< For a structure, an array of member variables.

  /// \brief This copies the data in a variant into into its destination, given its structure in the shader variable description.
  static void CopyDataFormVariant(xiiUInt8* pDestination, xiiVariant* pValue, const xiiGALShaderVariableDescription& description);
};

/// \brief This class wraps shader byte code storage.
///
/// Since byte code can have different requirements for alignment, padding etc. this class manages it.
/// Also since byte code is shared between multiple shaders (e.g. same vertex shaders for different pixel shaders)
/// the instances of the byte codes are reference counted.
class XII_GRAPHICSFOUNDATION_DLL xiiGALShaderByteCode : public xiiGALObject
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALShaderByteCode, xiiGALObject);

public:
  xiiGALShaderByteCode();

  xiiGALShaderByteCode(const xiiArrayPtr<const xiiUInt8>& pByteCode);

  /// \brief This returns a raw pointer to the shader bytecode.
  XII_NODISCARD const void* GetByteCode() const;

  /// \brief This returns the size of the shader bytecode.
  XII_NODISCARD xiiUInt32 GetSize() const;

  /// \brief This returns true if the shader bytecode is not empty, else returns false.
  XII_NODISCARD bool IsValid() const;

  /// \brief This retrieves the shader resource description of the resource with the given name.
  XII_NODISCARD const xiiGALShaderResourceDescription* GetDescription(const xiiTempHashedString& sName) const;

public:
  void CopyFrom(const xiiArrayPtr<const xiiUInt8>& pByteCode);

  // Filled by the shader compiler platform implementation.

  xiiDynamicArray<xiiUInt8>                           m_ByteCode;
  xiiHybridArray<xiiGALShaderResourceDescription, 8U> m_ShaderResourceBindings;
  xiiHybridArray<xiiGALVertexInputLayout, 8U>         m_VertexInputLayout;

  // Filled by the compiler base library.

  xiiBitflags<xiiGALShaderStage> m_ShaderStage           = xiiGALShaderStage::Unknown;
  bool                           m_bWasCompiledWithDebug = false;
};

#include <GraphicsFoundation/Shader/Implementation/ShaderByteCode_inl.h>
