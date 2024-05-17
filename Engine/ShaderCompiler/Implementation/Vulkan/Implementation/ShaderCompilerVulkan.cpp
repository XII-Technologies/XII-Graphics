
#include <ShaderCompiler/ShaderCompilerPCH.h>

#if BUILDSYSTEM_ENABLE_VULKAN_SUPPORT && (XII_ENABLED(XII_PLATFORM_WINDOWS) || XII_ENABLED(XII_PLATFORM_LINUX))

#  include <Foundation/IO/MemoryStream.h>
#  include <Foundation/Memory/MemoryUtils.h>
#  include <Foundation/Strings/StringConversion.h>

#  include <ShaderCompiler/Implementation/Vulkan/ShaderCompilerVulkan.h>
#  include <ShaderCompiler/ShaderCompiler.h>

#  include <spirv_reflect.h>

#  if XII_ENABLED(XII_PLATFORM_WINDOWS)
#    include <d3dcompiler.h>
#  endif
#  include <dxc/dxcapi.h>

xiiEnum<xiiGALTextureFormat> GetXIIFormatVulkan(SpvReflectFormat format);

xiiResult xiiShaderCompilerVulkan::CompileShader(xiiStringView sFile, xiiStringView sSource, bool bDebug, xiiStringView sProfile, xiiStringView sEntryPoint, xiiDynamicArray<xiiUInt8>& out_ByteCode)
{
  out_ByteCode.Clear();

  xiiStringView    sCompileSource = sSource;
  xiiStringBuilder sDebugSource;

  xiiDynamicArray<xiiStringWChar> args;
  args.PushBack(xiiStringWChar(sFile));
  args.PushBack(L"-E");
  args.PushBack(xiiStringWChar(sEntryPoint));
  args.PushBack(L"-T");
  args.PushBack(xiiStringWChar(sProfile));
  args.PushBack(L"-spirv");
  args.PushBack(L"-fspv-reflect");
  args.PushBack(L"-Zpc"); // Matrices in column-major order
  args.PushBack(L"-fvk-use-dx-position-w");
  args.PushBack(L"-fspv-target-env=vulkan1.1");

  if (bDebug)
  {
    // In debug mode we need to remove '#line' as any shader debugger won't work with them.
    sDebugSource = sSource;
    sDebugSource.ReplaceAll("#line ", "//line ");
    sCompileSource = sDebugSource;

    args.PushBack(L"-Zi"); // Enable debug information.
    args.PushBack(L"-Od"); // Disable optimization
  }
  else
  {
    args.PushBack(L"-O3"); // Optimization Level 3
  }

  xiiHybridArray<LPCWSTR, 16> pszArgs;
  pszArgs.SetCount(args.GetCount());
  for (xiiUInt32 i = 0; i < args.GetCount(); ++i)
  {
    pszArgs[i] = args[i].GetData();
  }

  xiiComPtr<IDxcBlobEncoding> pSource;
  m_pDxcUtils->CreateBlob(sCompileSource.GetStartPointer(), sCompileSource.GetElementCount(), DXC_CP_UTF8, pSource.RawDblPtr());

  DxcBuffer Source;
  Source.Ptr      = pSource->GetBufferPointer();
  Source.Size     = pSource->GetBufferSize();
  Source.Encoding = DXC_CP_UTF8;

  xiiComPtr<IDxcResult> pCompileResult;
  m_pDxcCompiler->Compile(&Source, pszArgs.GetData(), pszArgs.GetCount(), nullptr, IID_PPV_ARGS(pCompileResult.RawDblPtr()));

  xiiComPtr<IDxcBlobUtf8> pCompileError;
  pCompileResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(pCompileError.RawDblPtr()), nullptr);

  HRESULT hrStatus;
  pCompileResult->GetStatus(&hrStatus);
  if (FAILED(hrStatus))
  {
    xiiLog::Error("Shader compilation failed.");

    if (pCompileError != nullptr && pCompileError->GetStringLength() != 0)
    {
      xiiLog::Error("{}", xiiStringUtf8(pCompileError->GetStringPointer()).GetData());
    }
    return XII_FAILURE;
  }
  else
  {
    if (pCompileError != nullptr && pCompileError->GetStringLength() != 0)
    {
      xiiLog::Warning("{}", xiiStringUtf8(pCompileError->GetStringPointer()).GetData());
    }
  }

  xiiComPtr<IDxcBlob>     pShader;
  xiiComPtr<IDxcBlobWide> pShaderName;
  pCompileResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(pShader.RawDblPtr()), pShaderName.RawDblPtr());

  if (pShader == nullptr)
  {
    xiiLog::Error("No shader bytecode was generated.");
    return XII_FAILURE;
  }

  out_ByteCode.SetCountUninitialized(static_cast<xiiUInt32>(pShader->GetBufferSize()));

  xiiMemoryUtils::Copy(out_ByteCode.GetData(), reinterpret_cast<xiiUInt8*>(pShader->GetBufferPointer()), out_ByteCode.GetCount());

  return XII_SUCCESS;
}

xiiResult xiiShaderCompilerVulkan::ReflectShaderStage(xiiShaderProgramData& inout_Data, xiiBitflags<xiiGALShaderStage> Stage, xiiMap<xiiStringView, xiiEnum<xiiGALInputLayoutSemantic>>& vertexInputMapping)
{
  XII_LOG_BLOCK("ReflectShaderStage", inout_Data.m_sSourceFile);

  xiiGALShaderByteCode* pShader  = inout_Data.m_ByteCode[xiiGALShaderStage::GetStageIndex(Stage)];
  auto&                 byteCode = pShader->m_ByteCode;

  SpvReflectShaderModule reflectShaderModule = {};
  if (spvReflectCreateShaderModule(byteCode.GetCount(), byteCode.GetData(), &reflectShaderModule) != SPV_REFLECT_RESULT_SUCCESS)
  {
    xiiLog::Error("Extracting shader reflection information failed.");
    return XII_FAILURE;
  }

  XII_SCOPE_EXIT(spvReflectDestroyShaderModule(&reflectShaderModule));

  xiiHybridArray<xiiGALVertexInputLayout, 8>& vertexInputLayouts = pShader->m_VertexInputLayout;
  if (Stage.IsSet(xiiGALShaderStage::Vertex))
  {
    xiiUInt32 uiNumInputVariables = 0;
    if (spvReflectEnumerateInputVariables(&reflectShaderModule, &uiNumInputVariables, nullptr) != SPV_REFLECT_RESULT_SUCCESS)
    {
      xiiLog::Error("Failed to retrieve number of input variables.");
      return XII_FAILURE;
    }

    xiiDynamicArray<SpvReflectInterfaceVariable*> inputVariables;
    inputVariables.SetCount(uiNumInputVariables);

    if (spvReflectEnumerateInputVariables(&reflectShaderModule, &uiNumInputVariables, inputVariables.GetData()) != SPV_REFLECT_RESULT_SUCCESS)
    {
      xiiLog::Error("Failed to retrieve input variables.");
      return XII_FAILURE;
    }

    vertexInputLayouts.Reserve(inputVariables.GetCount());

    for (xiiUInt32 i = 0; i < inputVariables.GetCount(); ++i)
    {
      SpvReflectInterfaceVariable* pInputVariable = inputVariables[i];

      xiiStringBuilder sSemanticName = pInputVariable->semantic;

      if (!sSemanticName.StartsWith_NoCase("SV_"))
      {
        xiiGALVertexInputLayout& attribute = vertexInputLayouts.ExpandAndGetRef();
        attribute.m_uiSemanticIndex        = pInputVariable->location;

        xiiEnum<xiiGALInputLayoutSemantic>* pVAS = vertexInputMapping.GetValue(sSemanticName);
        XII_ASSERT_DEV(pVAS != nullptr, "Unknown vertex input semantic found: {0} in file {1}", sSemanticName, inout_Data.m_sSourceFile);

        if (pVAS != nullptr)
          attribute.m_Semantic = *pVAS;
        else
          xiiLog::Dev("Unknown vertex input semantic found: {}", pInputVariable->semantic);

        attribute.m_Format = GetXIIFormatVulkan(pInputVariable->format);
        XII_ASSERT_DEV(attribute.m_Format != xiiGALTextureFormat::Unknown, "Unknown vertex input format found: {}", pInputVariable->format);
      }
    }
  }

  // Descriptor Bindings
  {
    xiiUInt32 uiNumDescriptorBindings = 0;
    if (spvReflectEnumerateDescriptorBindings(&reflectShaderModule, &uiNumDescriptorBindings, nullptr) != SPV_REFLECT_RESULT_SUCCESS)
    {
      xiiLog::Error("Failed to retrieve the number of descriptor bindings.");
      return XII_FAILURE;
    }

    xiiDynamicArray<SpvReflectDescriptorBinding*> descriptorBindings;
    descriptorBindings.SetCount(uiNumDescriptorBindings);

    if (spvReflectEnumerateDescriptorBindings(&reflectShaderModule, &uiNumDescriptorBindings, descriptorBindings.GetData()) != SPV_REFLECT_RESULT_SUCCESS)
    {
      xiiLog::Error("Failed to retrieve descriptor bindings.");
      return XII_FAILURE;
    }

    for (xiiUInt32 i = 0; i < uiNumDescriptorBindings; ++i)
    {
      auto& descriptorBinding = *descriptorBindings[i];

      xiiLog::Info("Bound Resource: '{}' at slot {} (Count: {})", descriptorBinding.name, descriptorBinding.binding, descriptorBinding.count);

      xiiGALShaderResourceDescription shaderResourceBinding = {};
      shaderResourceBinding.m_Type                          = xiiGALShaderResourceType::Unknown;
      shaderResourceBinding.m_TextureType                   = xiiGALShaderTextureType::Unknown;
      shaderResourceBinding.m_uiArraySize                   = descriptorBinding.count;
      shaderResourceBinding.m_uiDescriptorSet               = descriptorBinding.set;
      shaderResourceBinding.m_uiBindIndex                   = descriptorBinding.binding;
      shaderResourceBinding.m_ShaderStages                  = Stage;
      shaderResourceBinding.m_uiTotalSize                   = 0U;
      shaderResourceBinding.m_sName.Assign(descriptorBinding.name);

      if (FillResourceBinding(*inout_Data.m_ByteCode[xiiGALShaderStage::GetStageIndex(Stage)], shaderResourceBinding, descriptorBinding).Failed())
        continue;

      XII_ASSERT_DEV(shaderResourceBinding.m_Type != xiiGALShaderResourceType::Unknown, "FillResourceBinding should have failed.");

      if (shaderResourceBinding.m_Type != xiiGALShaderResourceType::Unknown)
      {
        inout_Data.m_ByteCode[xiiGALShaderStage::GetStageIndex(Stage)]->m_ShaderResourceBindings.PushBack(shaderResourceBinding);
      }
    }
  }
  return XII_SUCCESS;
}

xiiResult xiiShaderCompilerVulkan::FillResourceBinding(xiiGALShaderByteCode& shaderBinary, xiiGALShaderResourceDescription& binding, const SpvReflectDescriptorBinding& info)
{
  if (info.descriptor_type == SpvReflectDescriptorType::SPV_REFLECT_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR)
  {
    binding.m_Type = xiiGALShaderResourceType::AccelerationStructure;

    return XII_SUCCESS;
  }

  if (info.descriptor_type == SpvReflectDescriptorType::SPV_REFLECT_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)
  {
    binding.m_Type = xiiGALShaderResourceType::InputAttachment;

    return XII_SUCCESS;
  }

  if (info.resource_type == SpvReflectResourceType::SPV_REFLECT_RESOURCE_FLAG_SRV)
  {
    return FillSRVResourceBinding(shaderBinary, binding, info);
  }

  if (info.resource_type == SpvReflectResourceType::SPV_REFLECT_RESOURCE_FLAG_UAV)
  {
    return FillUAVResourceBinding(shaderBinary, binding, info);
  }

  if (info.resource_type == SpvReflectResourceType::SPV_REFLECT_RESOURCE_FLAG_CBV)
  {
    binding.m_Type = xiiGALShaderResourceType::ConstantBuffer;

    return ReflectConstantBufferLayout(shaderBinary, binding, info);
  }

  if (info.resource_type == SpvReflectResourceType::SPV_REFLECT_RESOURCE_FLAG_SAMPLER)
  {
    binding.m_Type = xiiGALShaderResourceType::Sampler;

    return XII_SUCCESS;
  }

  xiiLog::Error("Resource '{}': Unsupported resource type.", info.name);

  return XII_FAILURE;
}

xiiResult xiiShaderCompilerVulkan::ReflectConstantBufferLayout(xiiGALShaderByteCode& pStageBinary, xiiGALShaderResourceDescription& binding, const SpvReflectDescriptorBinding& info)
{
  XII_LOG_BLOCK("Constant Buffer Layout", info.name);

  const auto& block = info.block;

  xiiLog::Debug("Constant Buffer has {} variables, Size is {}.", block.member_count, block.padded_size);

  binding.m_uiTotalSize = block.padded_size;

  for (xiiUInt32 uiMember = 0; uiMember < block.member_count; ++uiMember)
  {
    const auto&                     memberBlock       = block.members[uiMember];
    xiiGALShaderVariableDescription memberDescription = {};

    memberDescription.m_sName.Assign(memberBlock.name);
    memberDescription.m_uiOffset = memberBlock.offset;

    xiiUInt32 uiFlags = memberBlock.type_description->type_flags;

    if (uiFlags & SpvReflectTypeFlagBits::SPV_REFLECT_TYPE_FLAG_VOID)
    {
      uiFlags &= ~SpvReflectTypeFlagBits::SPV_REFLECT_TYPE_FLAG_VOID;

      memberDescription.m_Class         = xiiGALShaderVariableClassType::Unknown;
      memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Void;
    }

    if (uiFlags & SpvReflectTypeFlagBits::SPV_REFLECT_TYPE_FLAG_BOOL)
    {
      uiFlags &= ~SpvReflectTypeFlagBits::SPV_REFLECT_TYPE_FLAG_BOOL;

      memberDescription.m_Class         = xiiGALShaderVariableClassType::Scalar;
      memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Bool;
      memberDescription.m_uiArraySize   = 1U;
      memberDescription.m_uiRowCount    = 1U;
      memberDescription.m_uiColumnCount = 1U;
    }

    if (uiFlags & SpvReflectTypeFlagBits::SPV_REFLECT_TYPE_FLAG_INT)
    {
      uiFlags &= ~SpvReflectTypeFlagBits::SPV_REFLECT_TYPE_FLAG_INT;

      memberDescription.m_Class         = xiiGALShaderVariableClassType::Scalar;
      memberDescription.m_uiArraySize   = 1U;
      memberDescription.m_uiRowCount    = 1U;
      memberDescription.m_uiColumnCount = 1U;

      const bool bIsUnsigned = !memberBlock.type_description->traits.numeric.scalar.signedness;
      switch (memberBlock.type_description->traits.numeric.scalar.width)
      {
        case 64U:
        {
          if (bIsUnsigned)
            memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::UInt64;
          else
            memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Int64;
        }
        break;
        case 32U:
        {
          if (bIsUnsigned)
            memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::UInt32;
          else
            memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Int32;
        }
        break;
        case 16U:
        {
          if (bIsUnsigned)
            memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::UInt16;
          else
            memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Int16;
        }
        break;
        case 8U:
        {
          if (bIsUnsigned)
            memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::UInt8;
          else
            memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Int8;
        }
        break;
        default:
        {
          XII_ASSERT_NOT_IMPLEMENTED;
          continue;
        }
      }
    }

    if (uiFlags & SpvReflectTypeFlagBits::SPV_REFLECT_TYPE_FLAG_FLOAT)
    {
      uiFlags &= ~SpvReflectTypeFlagBits::SPV_REFLECT_TYPE_FLAG_FLOAT;

      memberDescription.m_Class         = xiiGALShaderVariableClassType::Scalar;
      memberDescription.m_uiArraySize   = 1U;
      memberDescription.m_uiRowCount    = 1U;
      memberDescription.m_uiColumnCount = 1U;

      switch (memberBlock.type_description->traits.numeric.scalar.width)
      {
        case 64U:
        {
          memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Double;
        }
        break;
        case 32U:
        {
          memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Float32;
        }
        break;
        case 16U:
        {
          memberDescription.m_PrimitiveType = xiiGALShaderPrimitiveType::Float16;
        }
        break;
        default:
        {
          XII_ASSERT_NOT_IMPLEMENTED;
          continue;
        }
      }
    }

    if (uiFlags & SpvReflectTypeFlagBits::SPV_REFLECT_TYPE_FLAG_VECTOR)
    {
      uiFlags &= ~SpvReflectTypeFlagBits::SPV_REFLECT_TYPE_FLAG_VECTOR;

      memberDescription.m_Class = xiiGALShaderVariableClassType::Array;

      XII_ASSERT_DEV(memberDescription.m_PrimitiveType != xiiGALShaderPrimitiveType::Unknown, "Expected a known shader variable primitive type.");

      memberDescription.m_uiRowCount    = 1U;
      memberDescription.m_uiColumnCount = memberBlock.type_description->traits.numeric.vector.component_count;
    }

    if (uiFlags & SpvReflectTypeFlagBits::SPV_REFLECT_TYPE_FLAG_MATRIX)
    {
      uiFlags &= ~SpvReflectTypeFlagBits::SPV_REFLECT_TYPE_FLAG_MATRIX;

      if (memberBlock.decoration_flags & SpvReflectDecorationFlagBits::SPV_REFLECT_DECORATION_ROW_MAJOR)
        memberDescription.m_Class = xiiGALShaderVariableClassType::MatrixRows;
      else if (memberBlock.decoration_flags & SpvReflectDecorationFlagBits::SPV_REFLECT_DECORATION_COLUMN_MAJOR)
        memberDescription.m_Class = xiiGALShaderVariableClassType::MatrixColumns;

      XII_ASSERT_DEV(memberDescription.m_PrimitiveType != xiiGALShaderPrimitiveType::Unknown, "Expected a known shader variable primitive type.");

      memberDescription.m_uiRowCount    = memberBlock.type_description->traits.numeric.matrix.row_count;
      memberDescription.m_uiColumnCount = memberBlock.type_description->traits.numeric.matrix.column_count;
    }

    if (uiFlags & SpvReflectTypeFlagBits::SPV_REFLECT_TYPE_FLAG_STRUCT)
    {
      uiFlags &= ~SpvReflectTypeFlagBits::SPV_REFLECT_TYPE_FLAG_STRUCT;
      uiFlags &= ~SpvReflectTypeFlagBits::SPV_REFLECT_TYPE_FLAG_EXTERNAL_BLOCK;

      memberDescription.m_Class = xiiGALShaderVariableClassType::Struct;
    }

    if (uiFlags & SpvReflectTypeFlagBits::SPV_REFLECT_TYPE_FLAG_ARRAY)
    {
      uiFlags &= ~SpvReflectTypeFlagBits::SPV_REFLECT_TYPE_FLAG_ARRAY;

      if (memberBlock.array.dims_count != 1U)
      {
        xiiLog::Error("Variable '{}': Multi-dimensional arrays are not supported.", memberDescription.m_sName);
        continue;
      }

      memberDescription.m_Class       = xiiGALShaderVariableClassType::Array;
      memberDescription.m_uiArraySize = memberBlock.array.dims[0];
    }

    if (uiFlags != 0)
    {
      xiiLog::Error("Variable '{}': Unknown additional type flags '{}'", memberDescription.m_sName, uiFlags);
    }

    if (memberDescription.m_Class == xiiGALShaderVariableClassType::Unknown)
    {
      xiiLog::Error("Variable '{}': Variable type is unknown / not supported", memberDescription.m_sName);
      continue;
    }

    /// \todo ShaderCompiler: Add member print output.

    binding.m_Variables.PushBack(memberDescription);
  }

  return XII_SUCCESS;
}

xiiResult xiiShaderCompilerVulkan::FillSRVResourceBinding(xiiGALShaderByteCode& shaderBinary, xiiGALShaderResourceDescription& binding, const SpvReflectDescriptorBinding& info)
{
  if (info.descriptor_type == SpvReflectDescriptorType::SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER)
  {
    if (info.type_description->op == SpvOp::SpvOpTypeStruct)
    {
      binding.m_Type = xiiGALShaderResourceType::BufferSRV;
      return XII_SUCCESS;
    }
  }

  if (info.descriptor_type == SpvReflectDescriptorType::SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE)
  {
    binding.m_Type = xiiGALShaderResourceType::TextureSRV;

    switch (info.image.dim)
    {
      case SpvDim::SpvDim1D:
      {
        if (info.image.ms == 0)
        {
          if (info.image.arrayed > 0)
          {
            binding.m_TextureType = xiiGALShaderTextureType::Texture1DArray;
            return XII_SUCCESS;
          }
          else
          {
            binding.m_TextureType = xiiGALShaderTextureType::Texture1D;
            return XII_SUCCESS;
          }
        }
      }
      break;

      case SpvDim::SpvDim2D:
      {
        if (info.image.ms == 0)
        {
          if (info.image.arrayed > 0)
          {
            binding.m_TextureType = xiiGALShaderTextureType::Texture2DArray;
            return XII_SUCCESS;
          }
          else
          {
            binding.m_TextureType = xiiGALShaderTextureType::Texture2D;
            return XII_SUCCESS;
          }
        }
        else
        {
          if (info.image.arrayed > 0)
          {
            binding.m_TextureType = xiiGALShaderTextureType::Texture2DMSArray;
            return XII_SUCCESS;
          }
          else
          {
            binding.m_TextureType = xiiGALShaderTextureType::Texture2DMS;
            return XII_SUCCESS;
          }
        }
      }
      break;

      case SpvDim::SpvDim3D:
      {
        if (info.image.ms == 0 && info.image.arrayed == 0)
        {
          binding.m_TextureType = xiiGALShaderTextureType::Texture3D;
          return XII_SUCCESS;
        }
      }
      break;

      case SpvDim::SpvDimCube:
      {
        if (info.image.ms == 0)
        {
          if (info.image.arrayed == 0)
          {
            binding.m_TextureType = xiiGALShaderTextureType::TextureCube;
            return XII_SUCCESS;
          }
          else
          {
            binding.m_TextureType = xiiGALShaderTextureType::TextureCubeArray;
            return XII_SUCCESS;
          }
        }
      }
      break;

      case SpvDim::SpvDimBuffer:
        binding.m_Type = xiiGALShaderResourceType::BufferSRV;
        return XII_SUCCESS;

      case SpvDim::SpvDimRect:
        XII_ASSERT_NOT_IMPLEMENTED;
        return XII_FAILURE;

      case SpvDim::SpvDimSubpassData:
        XII_ASSERT_NOT_IMPLEMENTED;
        return XII_FAILURE;

      case SpvDim::SpvDimMax:
        XII_ASSERT_DEV(false, "Invalid enum value");
        break;
    }

    if (info.image.ms > 0)
    {
      xiiLog::Error("Resource '{}': Multi-sampled textures of this type are not supported.", info.name);
      return XII_FAILURE;
    }

    if (info.image.arrayed > 0)
    {
      xiiLog::Error("Resource '{}': Array-textures of this type are not supported.", info.name);
      return XII_FAILURE;
    }
  }

  if (info.descriptor_type == SpvReflectDescriptorType::SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER)
  {
    if (info.image.dim == SpvDim::SpvDimBuffer)
    {
      binding.m_Type = xiiGALShaderResourceType::BufferSRV;
      return XII_SUCCESS;
    }

    xiiLog::Error("Resource '{}': Unsupported texel buffer SRV type.", info.name);
    return XII_FAILURE;
  }

  xiiLog::Error("Resource '{}': Unsupported SRV type.", info.name);
  return XII_FAILURE;
}

xiiResult xiiShaderCompilerVulkan::FillUAVResourceBinding(xiiGALShaderByteCode& shaderBinary, xiiGALShaderResourceDescription& binding, const SpvReflectDescriptorBinding& info)
{
  if (info.descriptor_type == SpvReflectDescriptorType::SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE)
  {
    binding.m_Type = xiiGALShaderResourceType::TextureUAV;
    return XII_SUCCESS;
  }

  if (info.descriptor_type == SpvReflectDescriptorType::SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER)
  {
    if (info.image.dim == SpvDim::SpvDimBuffer)
    {
      binding.m_Type = xiiGALShaderResourceType::BufferUAV;
      return XII_SUCCESS;
    }

    xiiLog::Error("Resource '{}': Unsupported texel buffer UAV type.", info.name);
    return XII_FAILURE;
  }

  xiiLog::Error("Resource '{}': Unsupported UAV type.", info.name);
  return XII_FAILURE;
}

xiiEnum<xiiGALTextureFormat> GetXIIFormatVulkan(SpvReflectFormat format)
{
  switch (format)
  {
    case SpvReflectFormat::SPV_REFLECT_FORMAT_R32_UINT:
      return xiiGALTextureFormat::R32UInt;
    case SpvReflectFormat::SPV_REFLECT_FORMAT_R32_SINT:
      return xiiGALTextureFormat::R32SInt;
    case SpvReflectFormat::SPV_REFLECT_FORMAT_R32_SFLOAT:
      return xiiGALTextureFormat::R32Float;
    case SpvReflectFormat::SPV_REFLECT_FORMAT_R32G32_UINT:
      return xiiGALTextureFormat::RG32UInt;
    case SpvReflectFormat::SPV_REFLECT_FORMAT_R32G32_SINT:
      return xiiGALTextureFormat::RG32SInt;
    case SpvReflectFormat::SPV_REFLECT_FORMAT_R32G32_SFLOAT:
      return xiiGALTextureFormat::RG32Float;
    case SpvReflectFormat::SPV_REFLECT_FORMAT_R32G32B32_UINT:
      return xiiGALTextureFormat::RGB32UInt;
    case SpvReflectFormat::SPV_REFLECT_FORMAT_R32G32B32_SINT:
      return xiiGALTextureFormat::RGB32SInt;
    case SpvReflectFormat::SPV_REFLECT_FORMAT_R32G32B32_SFLOAT:
      return xiiGALTextureFormat::RGB32Float;
    case SpvReflectFormat::SPV_REFLECT_FORMAT_R32G32B32A32_UINT:
      return xiiGALTextureFormat::RGBA32UInt;
    case SpvReflectFormat::SPV_REFLECT_FORMAT_R32G32B32A32_SINT:
      return xiiGALTextureFormat::RGBA32SInt;
    case SpvReflectFormat::SPV_REFLECT_FORMAT_R32G32B32A32_SFLOAT:
      return xiiGALTextureFormat::RGBA32Float;
    case SpvReflectFormat::SPV_REFLECT_FORMAT_UNDEFINED:
    default:
      return xiiGALTextureFormat::Unknown;
  }
}

#endif
