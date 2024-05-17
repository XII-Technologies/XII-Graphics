#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/States/BlendState.h>

// clang-format off

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALBlendFactor, 1)
  XII_ENUM_CONSTANT(xiiGALBlendFactor::Undefined),
  XII_ENUM_CONSTANT(xiiGALBlendFactor::Zero),
  XII_ENUM_CONSTANT(xiiGALBlendFactor::One),
  XII_ENUM_CONSTANT(xiiGALBlendFactor::SourceColor),
  XII_ENUM_CONSTANT(xiiGALBlendFactor::InverseSourceColor),
  XII_ENUM_CONSTANT(xiiGALBlendFactor::SourceAlpha),
  XII_ENUM_CONSTANT(xiiGALBlendFactor::InverseSourceAlpha),
  XII_ENUM_CONSTANT(xiiGALBlendFactor::DestinationAlpha),
  XII_ENUM_CONSTANT(xiiGALBlendFactor::InverseDestinationAlpha),
  XII_ENUM_CONSTANT(xiiGALBlendFactor::DestinationColor),
  XII_ENUM_CONSTANT(xiiGALBlendFactor::InverseDestinationColor),
  XII_ENUM_CONSTANT(xiiGALBlendFactor::SourceAlphaSaturate),
  XII_ENUM_CONSTANT(xiiGALBlendFactor::BlendFactor),
  XII_ENUM_CONSTANT(xiiGALBlendFactor::InverseBlendFactor),
  XII_ENUM_CONSTANT(xiiGALBlendFactor::SourceOneColor),
  XII_ENUM_CONSTANT(xiiGALBlendFactor::InverseSourceOneColor),
  XII_ENUM_CONSTANT(xiiGALBlendFactor::SourceOneAlpha),
  XII_ENUM_CONSTANT(xiiGALBlendFactor::InverseSourceOneAlpha),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALBlendOperation, 1)
  XII_ENUM_CONSTANT(xiiGALBlendOperation::Undefined),
  XII_ENUM_CONSTANT(xiiGALBlendOperation::Add),
  XII_ENUM_CONSTANT(xiiGALBlendOperation::Subtract),
  XII_ENUM_CONSTANT(xiiGALBlendOperation::ReverseSubtract),
  XII_ENUM_CONSTANT(xiiGALBlendOperation::Min),
  XII_ENUM_CONSTANT(xiiGALBlendOperation::Max),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALColorMask, 1)
  XII_BITFLAGS_CONSTANT(xiiGALColorMask::None),
  XII_BITFLAGS_CONSTANT(xiiGALColorMask::Red),
  XII_BITFLAGS_CONSTANT(xiiGALColorMask::Green),
  XII_BITFLAGS_CONSTANT(xiiGALColorMask::Blue),
  XII_BITFLAGS_CONSTANT(xiiGALColorMask::Alpha),
  XII_BITFLAGS_CONSTANT(xiiGALColorMask::RG),
  XII_BITFLAGS_CONSTANT(xiiGALColorMask::RGB),
  XII_BITFLAGS_CONSTANT(xiiGALColorMask::RGBA),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALLogicOperation, 1)
  XII_ENUM_CONSTANT(xiiGALLogicOperation::Clear),
  XII_ENUM_CONSTANT(xiiGALLogicOperation::Set),
  XII_ENUM_CONSTANT(xiiGALLogicOperation::Copy),
  XII_ENUM_CONSTANT(xiiGALLogicOperation::CopyInverted),
  XII_ENUM_CONSTANT(xiiGALLogicOperation::NoOperation),
  XII_ENUM_CONSTANT(xiiGALLogicOperation::Invert),
  XII_ENUM_CONSTANT(xiiGALLogicOperation::AND),
  XII_ENUM_CONSTANT(xiiGALLogicOperation::NAND),
  XII_ENUM_CONSTANT(xiiGALLogicOperation::OR),
  XII_ENUM_CONSTANT(xiiGALLogicOperation::NOR),
  XII_ENUM_CONSTANT(xiiGALLogicOperation::XOR),
  XII_ENUM_CONSTANT(xiiGALLogicOperation::Equivalent),
  XII_ENUM_CONSTANT(xiiGALLogicOperation::AndReversed),
  XII_ENUM_CONSTANT(xiiGALLogicOperation::AndInverted),
  XII_ENUM_CONSTANT(xiiGALLogicOperation::OrReversed),
  XII_ENUM_CONSTANT(xiiGALLogicOperation::OrInverted),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALBlendState, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;

// clang-format on

xiiGALBlendState::xiiGALBlendState(xiiGALDevice* pDevice, const xiiGALBlendStateCreationDescription& creationDescription) :
  xiiGALDeviceObject(pDevice), m_Description(creationDescription)
{
}

xiiGALBlendState::~xiiGALBlendState() = default;

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_States_Implementation_BlendState);
