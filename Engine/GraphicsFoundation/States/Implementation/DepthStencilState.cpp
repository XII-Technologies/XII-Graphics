#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/States/DepthStencilState.h>

// clang-format off

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALStencilOperation, 1)
  XII_ENUM_CONSTANT(xiiGALStencilOperation::Undefined),
  XII_ENUM_CONSTANT(xiiGALStencilOperation::Keep),
  XII_ENUM_CONSTANT(xiiGALStencilOperation::Zero),
  XII_ENUM_CONSTANT(xiiGALStencilOperation::Replace),
  XII_ENUM_CONSTANT(xiiGALStencilOperation::IncrementSaturate),
  XII_ENUM_CONSTANT(xiiGALStencilOperation::DecrementSaturate),
  XII_ENUM_CONSTANT(xiiGALStencilOperation::Invert),
  XII_ENUM_CONSTANT(xiiGALStencilOperation::IncrementWrap),
  XII_ENUM_CONSTANT(xiiGALStencilOperation::DecrementWrap),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALDepthStencilState, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;

// clang-format on

xiiGALDepthStencilState::xiiGALDepthStencilState(xiiGALDevice* pDevice, const xiiGALDepthStencilStateCreationDescription& creationDescription) :
  xiiGALDeviceObject(pDevice), m_Description(creationDescription)
{
}

xiiGALDepthStencilState::~xiiGALDepthStencilState() = default;

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_States_Implementation_DepthStencilState);
