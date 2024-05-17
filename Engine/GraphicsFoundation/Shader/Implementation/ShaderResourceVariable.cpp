#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/Shader/ShaderResourceVariable.h>

// clang-format off
XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALShaderResourceVariableType, 1)
  XII_ENUM_CONSTANT(xiiGALShaderResourceVariableType::Static),
  XII_ENUM_CONSTANT(xiiGALShaderResourceVariableType::Mutable),
  XII_ENUM_CONSTANT(xiiGALShaderResourceVariableType::Dynamic),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALShaderResourceVariableTypeFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALShaderResourceVariableTypeFlags::Static),
  XII_BITFLAGS_CONSTANT(xiiGALShaderResourceVariableTypeFlags::Mutable),
  XII_BITFLAGS_CONSTANT(xiiGALShaderResourceVariableTypeFlags::Dynamic),
  XII_BITFLAGS_CONSTANT(xiiGALShaderResourceVariableTypeFlags::MutableDynamic),
  XII_BITFLAGS_CONSTANT(xiiGALShaderResourceVariableTypeFlags::All),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALShaderResourceBindFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALShaderResourceBindFlags::Static),
  XII_BITFLAGS_CONSTANT(xiiGALShaderResourceBindFlags::Mutable),
  XII_BITFLAGS_CONSTANT(xiiGALShaderResourceBindFlags::Dynamic),
  XII_BITFLAGS_CONSTANT(xiiGALShaderResourceBindFlags::All),
  XII_BITFLAGS_CONSTANT(xiiGALShaderResourceBindFlags::KeepExisting),
  XII_BITFLAGS_CONSTANT(xiiGALShaderResourceBindFlags::VerifyAllResolved),
  XII_BITFLAGS_CONSTANT(xiiGALShaderResourceBindFlags::AllowOverWrite),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALSetShaderResourceFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALSetShaderResourceFlags::None),
  XII_BITFLAGS_CONSTANT(xiiGALSetShaderResourceFlags::AllowOverwrite),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALShaderResourceVariable, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALShaderResourceVariable::xiiGALShaderResourceVariable() :
  xiiGALObject()
{
}

xiiGALShaderResourceVariable::~xiiGALShaderResourceVariable() = default;

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_Shader_Implementation_ShaderResourceVariable);
