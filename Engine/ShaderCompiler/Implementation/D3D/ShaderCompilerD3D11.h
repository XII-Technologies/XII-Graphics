#pragma once

#include <ShaderCompiler/ShaderCompilerDLL.h>

#if BUILDSYSTEM_ENABLE_D3D11_SUPPORT

template <typename T>
struct xiiComPtr;

struct ID3D11ShaderReflectionConstantBuffer;

struct _D3D11_SHADER_INPUT_BIND_DESC;
typedef struct _D3D11_SHADER_INPUT_BIND_DESC D3D11_SHADER_INPUT_BIND_DESC;

struct ID3D11ShaderReflection;
struct ID3D11ShaderReflectionConstantBuffer;

class xiiShaderCompilerProgram;

class XII_SHADERCOMPILER_DLL xiiShaderCompilerD3D11
{
public:
  xiiResult CompileShader(xiiStringView sFile, xiiStringView sSource, bool bDebug, xiiStringView sProfile, xiiStringView sEntryPoint, xiiDynamicArray<xiiUInt8>& out_ByteCode);

private:
  friend xiiShaderCompilerProgram;

  xiiResult ReflectShaderStage(xiiShaderProgramData& inout_Data, xiiBitflags<xiiGALShaderStage> Stage, xiiMap<xiiStringView, xiiEnum<xiiGALInputLayoutSemantic>>& vertexInputMapping);
  xiiResult ReflectConstantBufferLayout(xiiGALShaderByteCode& pStageBinary, xiiGALShaderResourceDescription& binding, ID3D11ShaderReflectionConstantBuffer* pConstantBufferReflection);
  xiiResult FillResourceBinding(xiiGALShaderByteCode& shaderBinary, xiiGALShaderResourceDescription& binding, xiiComPtr<ID3D11ShaderReflection>& pReflector, const D3D11_SHADER_INPUT_BIND_DESC& info);
  xiiResult FillSRVResourceBinding(xiiGALShaderByteCode& shaderBinary, xiiGALShaderResourceDescription& binding, const D3D11_SHADER_INPUT_BIND_DESC& info);
  xiiResult FillUAVResourceBinding(xiiGALShaderByteCode& shaderBinary, xiiGALShaderResourceDescription& binding, const D3D11_SHADER_INPUT_BIND_DESC& info);
};

#endif
