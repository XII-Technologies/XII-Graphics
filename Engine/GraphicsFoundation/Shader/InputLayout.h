#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Declarations/DeviceObject.h>
#include <GraphicsFoundation/Declarations/GraphicsTypes.h>

/// \brief The maximum number of layout elements.
#define XII_GAL_MAX_LAYOUT_ELEMENTS 16

/// \brief The value which indicates that element offsets be computed automatically, see xiiGALLayoutElement::m_uiRelativeOffset.
#define XII_GAL_LAYOUT_ELEMENT_AUTO_OFFSET 0xFFFFFFFFU

/// \brief The value which indicates that element stride be computed automatically, see xiiGALLayoutElement::m_uiStride.
#define XII_GAL_LAYOUT_ELEMENT_AUTO_STRIDE 0xFFFFFFFFU

/// \brief This describes the input frequency.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALInputElementFrequency
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    Undefined = 0U, ///< The frequency is undefined.
    PerVertex,      ///< The input data is per-vertex data.
    PerInstance,    ///< The input data is per-instance data.

    ENUM_COUNT,

    Default = Undefined
  };
};

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALInputElementFrequency);

/// \brief This describes the vertex attribute semantic.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALInputLayoutSemantic
{
  using StorageType = xiiInt8;

  enum Enum : StorageType
  {
    Undefined = -1,
    Position,
    Normal,
    Tangent,
    Color0,
    Color1,
    Color2,
    Color3,
    Color4,
    Color5,
    Color6,
    Color7,
    TexCoord0,
    TexCoord1,
    TexCoord2,
    TexCoord3,
    TexCoord4,
    TexCoord5,
    TexCoord6,
    TexCoord7,
    TexCoord8,
    TexCoord9,

    BiTangent,
    BoneIndices0,
    BoneIndices1,
    BoneWeights0,
    BoneWeights1,

    ENUM_COUNT,

    Default = Undefined
  };
};

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALInputLayoutSemantic);

/// \brief Shader Vertex Input Attributes.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALVertexInputLayout
{
  XII_DECLARE_POD_TYPE();

  xiiEnum<xiiGALInputLayoutSemantic> m_Semantic        = xiiGALInputLayoutSemantic::Undefined;
  xiiUInt8                           m_uiSemanticIndex = 0U;
  xiiEnum<xiiGALTextureFormat>       m_Format          = xiiGALTextureFormat::Unknown;
};

/// \brief This describes a single element in the input layout.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALLayoutElement : public xiiHashableStruct<xiiGALLayoutElement>
{
  XII_DECLARE_POD_TYPE();

  xiiEnum<xiiGALInputLayoutSemantic>   m_Semantic               = xiiGALInputLayoutSemantic::Undefined;   ///< The element semantic. The default is Undefined.
  xiiUInt32                            m_uiInputIndex           = 0U;                                     ///< The input index of the element specified in the vertex shader. The default is 0.
  xiiUInt32                            m_uiBufferSlot           = 0U;                                     ///< The buffer slot index that this element is read from. The default is 0.
  xiiEnum<xiiGALTextureFormat>         m_Format                 = xiiGALTextureFormat::Unknown;           ///< The element format. The default is xiiGALTextureFormat::Unknown.
  xiiUInt32                            m_uiRelativeOffset       = XII_GAL_LAYOUT_ELEMENT_AUTO_OFFSET;     ///< The relative offset to the element in bytes. If this value is set to XII_GAL_LAYOUT_ELEMENT_AUTO_OFFSET (default value), the offset will be computed automatically by placing the element right after the previous one.
  xiiUInt32                            m_uiStride               = XII_GAL_LAYOUT_ELEMENT_AUTO_STRIDE;     ///< The stride in bytes between two elements for this buffer slot. If this value is is set to XII_GAL_LAYOUT_ELEMENT_AUTO_STRIDE (default value), the stride will be computed automatically assuming that all elements in the same buffer are packed contiguously. If the buffer slot contains multiple layout elements, they all must specify the same stride or use the XII_GAL_LAYOUT_ELEMENT_AUTO_OFFSET value.
  xiiEnum<xiiGALInputElementFrequency> m_Frequency              = xiiGALInputElementFrequency::PerVertex; ///< The element frequency. The default is xiiGALInputElementFrequency::PerVertex.
  xiiUInt32                            m_uiInstanceDataStepRate = 1U;                                     ///< The number of instances to draw using the same per-instance data before advancing in the buffer by one element. The default is 1.
};

/// \brief This describes the input layout creation description.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALInputLayoutCreationDescription : public xiiHashableStruct<xiiGALInputLayoutCreationDescription>
{
  xiiGALShaderHandle                                               m_hShader;        ///< The handle to the shader.
  xiiStaticArray<xiiGALLayoutElement, XII_GAL_MAX_LAYOUT_ELEMENTS> m_LayoutElements; ///< The input layout elements.
};

/// \brief Interface that defines methods to manipulate an input layout object.
class XII_GRAPHICSFOUNDATION_DLL xiiGALInputLayout : public xiiGALDeviceObject
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALInputLayout, xiiGALDeviceObject);

public:
  /// \brief This returns the creation description for this object.
  XII_NODISCARD const xiiGALInputLayoutCreationDescription& GetDescription() const;

protected:
  friend class xiiGALDevice;

  xiiGALInputLayout(xiiGALDevice* pDevice, const xiiGALInputLayoutCreationDescription& creationDescription);

  virtual ~xiiGALInputLayout();

  virtual xiiResult InitPlatform() = 0;

  virtual xiiResult DeInitPlatform() = 0;

protected:
  xiiGALInputLayoutCreationDescription m_Description;
};

#include <GraphicsFoundation/Shader/Implementation/InputLayout_inl.h>
