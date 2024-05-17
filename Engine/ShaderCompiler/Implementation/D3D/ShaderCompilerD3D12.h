#pragma once

#include <ShaderCompiler/ShaderCompilerDLL.h>

#if BUILDSYSTEM_ENABLE_D3D12_SUPPORT

template <typename T>
struct xiiComPtr;

struct IDxcUtils;
struct IDxcCompiler3;

struct _D3D12_SHADER_INPUT_BIND_DESC;
typedef struct _D3D12_SHADER_INPUT_BIND_DESC D3D12_SHADER_INPUT_BIND_DESC;

struct ID3D12ShaderReflection;
struct ID3D12ShaderReflectionConstantBuffer;

class xiiShaderCompilerProgram;

class XII_SHADERCOMPILER_DLL xiiShaderCompilerD3D12
{
public:
  xiiShaderCompilerD3D12(IDxcUtils* pDxcUtils, IDxcCompiler3* pDxcCompiler) :
    m_pDxcUtils(pDxcUtils), m_pDxcCompiler(pDxcCompiler)
  {}

  xiiResult CompileShader(xiiStringView sFile, xiiStringView sSource, bool bDebug, xiiStringView sProfile, xiiStringView sEntryPoint, xiiDynamicArray<xiiUInt8>& out_ByteCode);

private:
  friend xiiShaderCompilerProgram;

  IDxcUtils*     m_pDxcUtils    = nullptr;
  IDxcCompiler3* m_pDxcCompiler = nullptr;

  xiiResult ReflectShaderStage(xiiShaderProgramData& inout_Data, xiiBitflags<xiiGALShaderStage> Stage, xiiMap<xiiStringView, xiiEnum<xiiGALInputLayoutSemantic>>& vertexInputMapping);
  xiiResult ReflectConstantBufferLayout(xiiGALShaderByteCode& pStageBinary, xiiGALShaderResourceDescription& binding, ID3D12ShaderReflectionConstantBuffer* pConstantBufferReflection);
  xiiResult FillResourceBinding(xiiGALShaderByteCode& shaderBinary, xiiGALShaderResourceDescription& binding, xiiComPtr<ID3D12ShaderReflection>& pReflector, const D3D12_SHADER_INPUT_BIND_DESC& info);
  xiiResult FillSRVResourceBinding(xiiGALShaderByteCode& shaderBinary, xiiGALShaderResourceDescription& binding, const D3D12_SHADER_INPUT_BIND_DESC& info);
  xiiResult FillUAVResourceBinding(xiiGALShaderByteCode& shaderBinary, xiiGALShaderResourceDescription& binding, const D3D12_SHADER_INPUT_BIND_DESC& info);
};

#endif
