#pragma once

#include <ShaderCompiler/ShaderCompilerDLL.h>

#if BUILDSYSTEM_ENABLE_VULKAN_SUPPORT && (XII_ENABLED(XII_PLATFORM_WINDOWS) || XII_ENABLED(XII_PLATFORM_LINUX))

template <typename T>
struct xiiComPtr;

struct IDxcUtils;
struct IDxcCompiler3;

struct SpvReflectDescriptorBinding;

class xiiShaderCompilerProgram;

class XII_SHADERCOMPILER_DLL xiiShaderCompilerVulkan
{
public:
  xiiShaderCompilerVulkan(IDxcUtils* pDxcUtils, IDxcCompiler3* pDxcCompiler) :
    m_pDxcUtils(pDxcUtils), m_pDxcCompiler(pDxcCompiler)
  {}

  xiiResult CompileShader(xiiStringView sFile, xiiStringView sSource, bool bDebug, xiiStringView sProfile, xiiStringView sEntryPoint, xiiDynamicArray<xiiUInt8>& out_ByteCode);

private:
  friend xiiShaderCompilerProgram;

  IDxcUtils*     m_pDxcUtils    = nullptr;
  IDxcCompiler3* m_pDxcCompiler = nullptr;

  xiiResult ReflectShaderStage(xiiShaderProgramData& inout_Data, xiiBitflags<xiiGALShaderStage> Stage, xiiMap<xiiStringView, xiiEnum<xiiGALInputLayoutSemantic>>& vertexInputMapping);
  xiiResult ReflectConstantBufferLayout(xiiGALShaderByteCode& pStageBinary, xiiGALShaderResourceDescription& binding, const SpvReflectDescriptorBinding& info);
  xiiResult FillResourceBinding(xiiGALShaderByteCode& shaderBinary, xiiGALShaderResourceDescription& binding, const SpvReflectDescriptorBinding& info);
  xiiResult FillSRVResourceBinding(xiiGALShaderByteCode& shaderBinary, xiiGALShaderResourceDescription& binding, const SpvReflectDescriptorBinding& info);
  xiiResult FillUAVResourceBinding(xiiGALShaderByteCode& shaderBinary, xiiGALShaderResourceDescription& binding, const SpvReflectDescriptorBinding& info);
};

#endif
