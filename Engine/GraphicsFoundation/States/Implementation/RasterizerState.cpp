#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/States/RasterizerState.h>

// clang-format off

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALFillMode, 1)
  XII_ENUM_CONSTANT(xiiGALFillMode::Undefined),
  XII_ENUM_CONSTANT(xiiGALFillMode::Wireframe),
  XII_ENUM_CONSTANT(xiiGALFillMode::Solid),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALCullMode, 1)
  XII_ENUM_CONSTANT(xiiGALCullMode::Undefined),
  XII_ENUM_CONSTANT(xiiGALCullMode::None),
  XII_ENUM_CONSTANT(xiiGALCullMode::Front),
  XII_ENUM_CONSTANT(xiiGALCullMode::Back),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALRasterizerState, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;

// clang-format on

xiiGALRasterizerState::xiiGALRasterizerState(xiiGALDevice* pDevice, const xiiGALRasterizerStateCreationDescription& creationDescription) :
  xiiGALDeviceObject(pDevice), m_Description(creationDescription)
{
}

xiiGALRasterizerState::~xiiGALRasterizerState() = default;

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_States_Implementation_RasterizerState);
