#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/Shader/Shader.h>
#include <GraphicsFoundation/Shader/ShaderUtils.h>

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALShader, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiDelegate<void(xiiShaderUtilities::xiiBuiltinShaderType type, xiiShaderUtilities::xiiBuiltinShader& out_shader)> xiiShaderUtilities::g_RequestBuiltinShaderCallback;

xiiGALShader::xiiGALShader(xiiGALDevice* pDevice, const xiiGALShaderCreationDescription& creationDescription) :
  xiiGALDeviceObject(pDevice), m_Description(creationDescription)
{
}

xiiGALShader::~xiiGALShader() = default;

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_Shader_Implementation_Shader);
