#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/Shader/InputLayout.h>

// clang-format off

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALInputElementFrequency, 1)
  XII_ENUM_CONSTANT(xiiGALInputElementFrequency::Undefined),
  XII_ENUM_CONSTANT(xiiGALInputElementFrequency::PerVertex),
  XII_ENUM_CONSTANT(xiiGALInputElementFrequency::PerInstance),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALInputLayoutSemantic, 1)
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::Undefined),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::Position),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::Normal),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::Tangent),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::Color0),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::Color1),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::Color2),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::Color3),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::Color4),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::Color5),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::Color6),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::Color7),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::TexCoord0),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::TexCoord1),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::TexCoord2),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::TexCoord3),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::TexCoord4),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::TexCoord5),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::TexCoord6),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::TexCoord7),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::TexCoord8),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::TexCoord9),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::BiTangent),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::BoneIndices0),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::BoneIndices1),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::BoneWeights0),
  XII_ENUM_CONSTANT(xiiGALInputLayoutSemantic::BoneWeights1),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALInputLayout, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;

// clang-format on

xiiGALInputLayout::xiiGALInputLayout(xiiGALDevice* pDevice, const xiiGALInputLayoutCreationDescription& creationDescription) :
  xiiGALDeviceObject(pDevice), m_Description(creationDescription)
{
}

xiiGALInputLayout::~xiiGALInputLayout() = default;

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_Shader_Implementation_InputLayout);
