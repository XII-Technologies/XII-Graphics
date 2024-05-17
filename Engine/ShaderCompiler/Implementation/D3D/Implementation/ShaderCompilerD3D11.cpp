#include <ShaderCompiler/ShaderCompilerPCH.h>

#if BUILDSYSTEM_ENABLE_D3D11_SUPPORT

#  include <ShaderCompiler/Implementation/D3D/ShaderCompilerD3D11.h>
#  include <ShaderCompiler/ShaderCompiler.h>

#  include <d3dcompiler.h>

XII_DEFINE_AS_POD_TYPE(D3D11_SHADER_INPUT_BIND_DESC);

xiiEnum<xiiGALTextureFormat> GetXIIFormatD3D11(D3D_REGISTER_COMPONENT_TYPE format, xiiUInt32 numComponents);

xiiResult xiiShaderCompilerD3D11::CompileShader(xiiStringView sFile, xiiStringView sSource, bool bDebug, xiiStringView sProfile, xiiStringView sEntryPoint, xiiDynamicArray<xiiUInt8>& out_ByteCode)
{
  out_ByteCode.Clear();

  xiiUInt32        uiCompileFlags = 0U;
  xiiStringView    sCompileSource = sSource;
  xiiStringBuilder sDebugSource;

  if (bDebug)
  {
    uiCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_PREFER_FLOW_CONTROL | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_ENABLE_STRICTNESS;

    // In debug mode we need to remove '#line' as any shader debugger won't work with them.
    sDebugSource = sSource;
    sDebugSource.ReplaceAll("#line ", "//line ");
    sCompileSource = sDebugSource;
  }

  ID3DBlob* pResultBlob = nullptr;
  ID3DBlob* pErrorBlob  = nullptr;

  if (FAILED(D3DCompile(sCompileSource.GetStartPointer(), sCompileSource.GetElementCount(), sFile.GetStartPointer(), nullptr, nullptr, sEntryPoint.GetStartPointer(), sProfile.GetStartPointer(), uiCompileFlags, 0U, &pResultBlob, &pErrorBlob)))
  {
    if (bDebug)
    {
      // Try again with '#line' intact to get correct error messages with file and line info.
      pErrorBlob->Release();
      pErrorBlob = nullptr;
      XII_VERIFY(FAILED(D3DCompile(sSource.GetStartPointer(), sSource.GetElementCount(), sFile.GetStartPointer(), nullptr, nullptr, sEntryPoint.GetStartPointer(), sProfile.GetStartPointer(), uiCompileFlags, 0, &pResultBlob, &pErrorBlob)), "Debug compilation with commented out '#line' failed but original version did not.");
    }

    const char* szError = static_cast<const char*>(pErrorBlob->GetBufferPointer());

    xiiLog::Error("Shader Compilation Failed.");
    xiiLog::Error("Could not compile shader '{0}' for profile '{1}'", sFile, sProfile);
    xiiLog::Error("{0}", szError);

    pErrorBlob->Release();
    return XII_FAILURE;
  }

  if (pErrorBlob != nullptr)
  {
    const char* szError = static_cast<const char*>(pErrorBlob->GetBufferPointer());

    xiiLog::SeriousWarning("{0}", szError);

    pErrorBlob->Release();
  }

  if (pResultBlob != nullptr)
  {
    out_ByteCode.SetCountUninitialized((xiiUInt32)pResultBlob->GetBufferSize());
    xiiMemoryUtils::Copy(out_ByteCode.GetData(), static_cast<xiiUInt8*>(pResultBlob->GetBufferPointer()), out_ByteCode.GetCount());
    pResultBlob->Release();
  }

  return XII_SUCCESS;
}

xiiResult xiiShaderCompilerD3D11::ReflectShaderStage(xiiShaderProgramData& inout_Data, xiiBitflags<xiiGALShaderStage> Stage, xiiMap<xiiStringView, xiiEnum<xiiGALInputLayoutSemantic>>& vertexInputMapping)
{
  XII_LOG_BLOCK("ReflectShaderStage", inout_Data.m_sSourceFile);

  xiiGALShaderByteCode* pShader  = inout_Data.m_ByteCode[xiiGALShaderStage::GetStageIndex((xiiGALShaderStage::Enum)Stage.GetValue())];
  auto&                 byteCode = pShader->m_ByteCode;

  xiiComPtr<ID3D11ShaderReflection> pReflector;
  if (FAILED(D3DReflect(byteCode.GetData(), byteCode.GetCount(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(pReflector.RawDblPtr()))))
  {
    xiiLog::Error("Failed to create shader reflector.");
    return XII_FAILURE;
  }

  D3D11_SHADER_DESC shaderDescription;
  if (FAILED(pReflector->GetDesc(&shaderDescription)))
  {
    xiiLog::Error("Failed to extract shader information.");
    return XII_FAILURE;
  }

  // Vertex Attributes
  xiiHybridArray<xiiGALVertexInputLayout, 8>& vertexInputLayouts = pShader->m_VertexInputLayout;
  if (Stage.IsSet(xiiGALShaderStage::Vertex))
  {
    xiiUInt32 uiNumVars = shaderDescription.InputParameters;

    xiiDynamicArray<D3D11_PARAMETER_DESC*> inputParameters;
    inputParameters.SetCount(uiNumVars);

    vertexInputLayouts.Reserve(inputParameters.GetCount());

    for (xiiUInt32 i = 0; i < inputParameters.GetCount(); ++i)
    {
      D3D11_SIGNATURE_PARAMETER_DESC parameterDesc;
      if FAILED (pReflector->GetInputParameterDesc(i, &parameterDesc))
      {
        xiiLog::Error("Failed to retrieve shader parameter descriptor");
        return XII_FAILURE;
      }

      xiiStringBuilder sSemanticName = parameterDesc.SemanticName;
      sSemanticName.AppendFormat("{}", parameterDesc.SemanticIndex);

      if (!sSemanticName.StartsWith_NoCase("SV_"))
      {
        xiiGALVertexInputLayout& attribute = vertexInputLayouts.ExpandAndGetRef();
        attribute.m_uiSemanticIndex        = parameterDesc.SemanticIndex;

        xiiEnum<xiiGALInputLayoutSemantic>* pVAS = vertexInputMapping.GetValue(sSemanticName);
        XII_ASSERT_DEV(pVAS != nullptr, "Unknown vertex input semantic found: {0} in file {1}", sSemanticName, inout_Data.m_sSourceFile);

        if (pVAS != nullptr)
          attribute.m_Semantic = *pVAS;
        else
          xiiLog::Dev("Unknown vertex input semantic found: {}", parameterDesc.SemanticName);

        attribute.m_Format = GetXIIFormatD3D11(parameterDesc.ComponentType, parameterDesc.Mask);
        XII_ASSERT_DEV(attribute.m_Format != xiiGALTextureFormat::Unknown, "Unknown vertex input format found: {}", parameterDesc.ComponentType);
      }
    }
  }

  // Descriptor Bindings
  {
    xiiUInt32 uiNumBoundResources = shaderDescription.BoundResources;

    xiiDynamicArray<D3D11_SHADER_INPUT_BIND_DESC> boundResources;
    boundResources.SetCount(uiNumBoundResources);

    for (xiiUInt32 i = 0; i < uiNumBoundResources; ++i)
    {
      D3D11_SHADER_INPUT_BIND_DESC& inputDescription = boundResources[i];
      if (FAILED(pReflector->GetResourceBindingDesc(i, &inputDescription)))
      {
        xiiLog::Error("Failed to retrieve shader input descriptor");
        return XII_FAILURE;
      }

      xiiLog::Info("Bound Resource: '{}' at slot {} (Count: {})", inputDescription.Name, inputDescription.BindPoint, inputDescription.BindCount);

      xiiGALShaderResourceDescription shaderResourceBinding = {};
      shaderResourceBinding.m_Type                          = xiiGALShaderResourceType::Unknown;
      shaderResourceBinding.m_TextureType                   = xiiGALShaderTextureType::Unknown;
      shaderResourceBinding.m_uiArraySize                   = inputDescription.BindCount;
      shaderResourceBinding.m_uiDescriptorSet               = 0;
      shaderResourceBinding.m_uiBindIndex                   = inputDescription.BindPoint;
      shaderResourceBinding.m_ShaderStages                  = Stage;
      shaderResourceBinding.m_sName.Assign(inputDescription.Name);

      if (FillResourceBinding(*inout_Data.m_ByteCode[xiiGALShaderStage::GetStageIndex((xiiGALShaderStage::Enum)Stage.GetValue())], shaderResourceBinding, pReflector, inputDescription).Failed())
        continue;

      XII_ASSERT_DEV(shaderResourceBinding.m_Type != xiiGALShaderResourceType::Unknown, "FillResourceBinding should have failed.");

      if (shaderResourceBinding.m_Type != xiiGALShaderResourceType::Unknown)
      {
        inout_Data.m_ByteCode[xiiGALShaderStage::GetStageIndex((xiiGALShaderStage::Enum)Stage.GetValue())]->m_ShaderResourceBindings.PushBack(shaderResourceBinding);
      }
    }
  }

  return XII_SUCCESS;
}

xiiResult xiiShaderCompilerD3D11::FillResourceBinding(xiiGALShaderByteCode& shaderBinary, xiiGALShaderResourceDescription& binding, xiiComPtr<ID3D11ShaderReflection>& pReflector, const D3D11_SHADER_INPUT_BIND_DESC& info)
{
  // clang-format off
  if (info.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_STRUCTURED
    || info.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE
    || info.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_TBUFFER
    || info.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_BYTEADDRESS)
  // clang-format on
  {
    return FillSRVResourceBinding(shaderBinary, binding, info);
  }

  // clang-format off
  if (info.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_RWTYPED
    || info.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_RWSTRUCTURED
    || info.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_RWBYTEADDRESS
    || info.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_APPEND_STRUCTURED
    || info.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_CONSUME_STRUCTURED
    || info.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER
    || info.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_FEEDBACKTEXTURE)
  // clang-format on
  {
    return FillUAVResourceBinding(shaderBinary, binding, info);
  }

  if (info.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_CBUFFER)
  {
    binding.m_Type = xiiGALShaderResourceType::ConstantBuffer;

    return ReflectConstantBufferLayout(shaderBinary, binding, pReflector->GetConstantBufferByName(info.Name));
  }

  if (info.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_SAMPLER)
  {
    binding.m_Type = xiiGALShaderResourceType::Sampler;

    return XII_SUCCESS;
  }

  if (info.Type == D3D_SIT_RTACCELERATIONSTRUCTURE)
  {
    binding.m_Type = xiiGALShaderResourceType::AccelerationStructure;

    return XII_SUCCESS;
  }

  xiiLog::Error("Resource '{}': Unsupported resource type.", info.Name);

  return XII_FAILURE;
}

xiiResult xiiShaderCompilerD3D11::ReflectConstantBufferLayout(xiiGALShaderByteCode& pStageBinary, xiiGALShaderResourceDescription& binding, ID3D11ShaderReflectionConstantBuffer* pConstantBufferReflection)
{
  XII_LOG_BLOCK("Constant Buffer Layout", binding.m_sName);

  D3D11_SHADER_BUFFER_DESC shaderDescription;

  if (FAILED(pConstantBufferReflection->GetDesc(&shaderDescription)))
  {
    xiiLog::Error("Failed to retrieve constant buffer descriptor.");
    return XII_FAILURE;
  }

  xiiLog::Debug("Constant Buffer has {} variables, Size is {} {}.", shaderDescription.Variables, shaderDescription.Size, (shaderDescription.Size > 1 ? "bytes" : "byte"));

  binding.m_uiTotalSize = shaderDescription.Size;

  for (xiiUInt32 i = 0; i < shaderDescription.Variables; ++i)
  {
    ID3D11ShaderReflectionVariable* pCBVariable = pConstantBufferReflection->GetVariableByIndex(i);

    D3D11_SHADER_VARIABLE_DESC variableDescription;
    if (FAILED(pCBVariable->GetDesc(&variableDescription)))
    {
      xiiLog::Error("Failed to retrieve shader variable descriptor.");
      return XII_FAILURE;
    }

    ID3D11ShaderReflectionType* pTypeDescription = pCBVariable->GetType();

    D3D11_SHADER_TYPE_DESC typeDescription;
    if (FAILED(pTypeDescription->GetDesc(&typeDescription)))
    {
      xiiLog::Info("Failed to retrieve shader variable type descriptor");
      return XII_FAILURE;
    }

    xiiGALShaderVariableDescription memberDescription = {};
    memberDescription.m_uiOffset                      = variableDescription.StartOffset;
    memberDescription.m_uiArraySize                   = xiiMath::Max(typeDescription.Elements, 1U);
    memberDescription.m_sName.Assign(variableDescription.Name);

    switch (typeDescription.Class)
    {
      case D3D_SVC_SCALAR:
        memberDescription.m_Class         = xiiGALShaderVariableClassType::Scalar;
        memberDescription.m_uiRowCount    = 1U;
        memberDescription.m_uiColumnCount = 1U;
        break;
      case D3D_SVC_VECTOR:
        memberDescription.m_Class         = xiiGALShaderVariableClassType::Array;
        memberDescription.m_uiRowCount    = 1U;
        memberDescription.m_uiColumnCount = typeDescription.Columns;
        break;
      case D3D_SVC_MATRIX_ROWS:
        memberDescription.m_Class         = xiiGALShaderVariableClassType::MatrixRows;
        memberDescription.m_uiRowCount    = typeDescription.Rows;
        memberDescription.m_uiColumnCount = typeDescription.Columns;
        break;
      case D3D_SVC_MATRIX_COLUMNS:
        memberDescription.m_Class         = xiiGALShaderVariableClassType::MatrixColumns;
        memberDescription.m_uiRowCount    = typeDescription.Rows;
        memberDescription.m_uiColumnCount = typeDescription.Columns;
        break;
      case D3D_SVC_STRUCT:
        memberDescription.m_Class = xiiGALShaderVariableClassType::Struct;
        break;
      default:
        XII_ASSERT_NOT_IMPLEMENTED;
        continue;
    }

    switch (typeDescription.Type)
    {
      case D3D_SVT_VOID:
        memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Void;
        break;
      case D3D_SVT_BOOL:
        memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Bool;
        break;
      case D3D_SVT_INT16:
        memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Int16;
        break;
      case D3D_SVT_INT:
        memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Int32;
        break;
      case D3D_SVT_INT64:
        memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Int64;
        break;
      case D3D_SVT_UINT8:
        memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::UInt8;
        break;
      case D3D_SVT_UINT16:
        memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::UInt16;
        break;
      case D3D_SVT_UINT:
        memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::UInt32;
        break;
      case D3D_SVT_UINT64:
        memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::UInt64;
        break;
      case D3D_SVT_FLOAT16:
        memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Float16;
        break;
      case D3D_SVT_FLOAT:
        memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Float32;
        break;
      case D3D_SVT_DOUBLE:
        memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Double;
        break;
      case D3D_SVT_MIN8FLOAT:
        memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Min8Float;
        break;
      case D3D_SVT_MIN10FLOAT:
        memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Min10Float;
        break;
      case D3D_SVT_MIN16FLOAT:
        memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Min16Float;
        break;
      case D3D_SVT_MIN12INT:
        memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Min12Int;
        break;
      case D3D_SVT_MIN16INT:
        memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Min16Int;
        break;
      case D3D_SVT_MIN16UINT:
        memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Min16UInt;
        break;
      case D3D_SVT_STRING:
        memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::String;
        break;
      default:
        XII_ASSERT_NOT_IMPLEMENTED;
        continue;
    }

    /// \todo ShaderCompiler: Add member print output.

    binding.m_Variables.PushBack(memberDescription);
  }

  return XII_SUCCESS;
}

xiiResult xiiShaderCompilerD3D11::FillSRVResourceBinding(xiiGALShaderByteCode& shaderBinary, xiiGALShaderResourceDescription& binding, const D3D11_SHADER_INPUT_BIND_DESC& info)
{
  if (info.Type == D3D_SIT_STRUCTURED || info.Type == D3D_SIT_BYTEADDRESS || info.Type == D3D_SVT_BUFFER)
  {
    binding.m_Type = xiiGALShaderResourceType::BufferSRV;
    return XII_SUCCESS;
  }
  else if (info.Type == D3D_SIT_TEXTURE)
  {
    switch (info.Dimension)
    {
      case D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_TEXTURE1D:
        binding.m_Type        = xiiGALShaderResourceType::TextureSRV;
        binding.m_TextureType = xiiGALShaderTextureType::Texture1D;
        break;
      case D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_TEXTURE1DARRAY:
        binding.m_Type        = xiiGALShaderResourceType::TextureSRV;
        binding.m_TextureType = xiiGALShaderTextureType::Texture1DArray;
        break;
      case D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_TEXTURE2D:
        binding.m_Type        = xiiGALShaderResourceType::TextureSRV;
        binding.m_TextureType = xiiGALShaderTextureType::Texture2D;
        break;
      case D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_TEXTURE2DARRAY:
        binding.m_Type        = xiiGALShaderResourceType::TextureSRV;
        binding.m_TextureType = xiiGALShaderTextureType::Texture2DArray;
        break;
      case D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_TEXTURE2DMS:
        binding.m_Type        = xiiGALShaderResourceType::TextureSRV;
        binding.m_TextureType = xiiGALShaderTextureType::Texture2DMS;
        break;
      case D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_TEXTURE2DMSARRAY:
        binding.m_Type        = xiiGALShaderResourceType::TextureSRV;
        binding.m_TextureType = xiiGALShaderTextureType::Texture2DMSArray;
        break;
      case D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_TEXTURE3D:
        binding.m_Type        = xiiGALShaderResourceType::TextureSRV;
        binding.m_TextureType = xiiGALShaderTextureType::Texture3D;
        break;
      case D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_TEXTURECUBE:
        binding.m_Type        = xiiGALShaderResourceType::TextureSRV;
        binding.m_TextureType = xiiGALShaderTextureType::TextureCube;
        break;
      case D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_TEXTURECUBEARRAY:
        binding.m_Type        = xiiGALShaderResourceType::TextureSRV;
        binding.m_TextureType = xiiGALShaderTextureType::TextureCubeArray;
        break;
      default:
        XII_ASSERT_NOT_IMPLEMENTED;
        return XII_FAILURE;
    }

    return XII_SUCCESS;
  }

  return XII_FAILURE;
}

xiiResult xiiShaderCompilerD3D11::FillUAVResourceBinding(xiiGALShaderByteCode& shaderBinary, xiiGALShaderResourceDescription& binding, const D3D11_SHADER_INPUT_BIND_DESC& info)
{
  switch (info.Type)
  {
    case D3D_SIT_UAV_RWTYPED:
    {
      switch (info.Dimension)
      {
        case D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_TEXTURE1D:
        case D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_TEXTURE1DARRAY:
        case D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_TEXTURE2D:
        case D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_TEXTURE2DARRAY:
        case D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_BUFFER:
        case D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_BUFFEREX:
          binding.m_Type = xiiGALShaderResourceType::TextureUAV;
          break;

        default:
          XII_ASSERT_NOT_IMPLEMENTED;
          return XII_FAILURE;
      }

      return XII_SUCCESS;
    }

    case D3D_SIT_UAV_RWSTRUCTURED:
    case D3D_SIT_UAV_RWBYTEADDRESS:
    case D3D_SIT_UAV_APPEND_STRUCTURED:
    case D3D_SIT_UAV_CONSUME_STRUCTURED:
    case D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER:
      binding.m_Type = xiiGALShaderResourceType::BufferUAV;
      return XII_SUCCESS;
  }

  return XII_FAILURE;
}

xiiEnum<xiiGALTextureFormat> GetXIIFormatD3D11(D3D_REGISTER_COMPONENT_TYPE format, xiiUInt32 numComponents)
{
  switch (format)
  {
    case D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_UINT32:
    {
      switch (numComponents)
      {
        case 0xF:
          return xiiGALTextureFormat::RGBA32UInt;
        case 0x7:
          return xiiGALTextureFormat::RGB32UInt;
        case 0x3:
          return xiiGALTextureFormat::RG32UInt;
        case 0x1:
          return xiiGALTextureFormat::R32UInt;
      }
    }
    break;
    case D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_SINT32:
    {
      switch (numComponents)
      {
        case 0xF:
          return xiiGALTextureFormat::RGBA32SInt;
        case 0x7:
          return xiiGALTextureFormat::RGB32SInt;
        case 0x3:
          return xiiGALTextureFormat::RG32SInt;
        case 0x1:
          return xiiGALTextureFormat::R32SInt;
      }
    }
    break;
    case D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_FLOAT32:
    {
      switch (numComponents)
      {
        case 0xF:
          return xiiGALTextureFormat::RGBA32Float;
        case 0x7:
          return xiiGALTextureFormat::RGB32Float;
        case 0x3:
          return xiiGALTextureFormat::RG32Float;
        case 0x1:
          return xiiGALTextureFormat::R32Float;
      }
    }
    break;

    case D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_UNKNOWN:
    default:
      return xiiGALTextureFormat::Unknown;
  }

  return xiiGALTextureFormat::Unknown;
}
#endif
