#include <ShaderCompiler/ShaderCompilerPCH.h>

#include <ShaderCompiler/ShaderCompiler.h>

#include <Foundation/Configuration/Startup.h>
#include <Foundation/IO/MemoryStream.h>
#include <Foundation/Memory/MemoryUtils.h>
#include <Foundation/Strings/StringConversion.h>

#if BUILDSYSTEM_ENABLE_D3D11_SUPPORT
#  include <ShaderCompiler/Implementation/D3D/ShaderCompilerD3D11.h>
#endif

#if BUILDSYSTEM_ENABLE_D3D12_SUPPORT
#  include <ShaderCompiler/Implementation/D3D/ShaderCompilerD3D12.h>
#endif

#if BUILDSYSTEM_ENABLE_VULKAN_SUPPORT && (XII_ENABLED(XII_PLATFORM_WINDOWS) || XII_ENABLED(XII_PLATFORM_LINUX))
#  include <ShaderCompiler/Implementation/Vulkan/ShaderCompilerVulkan.h>
#endif

#if XII_ENABLED(XII_PLATFORM_WINDOWS)
#  include <d3dcompiler.h>
#endif

#if (BUILDSYSTEM_ENABLE_D3D12_SUPPORT || BUILDSYSTEM_ENABLE_VULKAN_SUPPORT) && (XII_ENABLED(XII_PLATFORM_WINDOWS) || XII_ENABLED(XII_PLATFORM_LINUX))
#  include <dxc/dxcapi.h>

xiiComPtr<IDxcUtils>     s_pDxcUtils;
xiiComPtr<IDxcCompiler3> s_pDxcCompiler;
#endif

// clang-format off
XII_BEGIN_SUBSYSTEM_DECLARATION(ShaderCompiler, ShaderCompilerPlugin)

  BEGIN_SUBSYSTEM_DEPENDENCIES
    "Foundation"
  END_SUBSYSTEM_DEPENDENCIES

  ON_CORESYSTEMS_STARTUP
  {
    #if (BUILDSYSTEM_ENABLE_D3D12_SUPPORT || BUILDSYSTEM_ENABLE_VULKAN_SUPPORT) && (XII_ENABLED(XII_PLATFORM_WINDOWS) || XII_ENABLED(XII_PLATFORM_LINUX))
    DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(s_pDxcUtils.Put()));
    DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(s_pDxcCompiler.Put()));
    #endif
  }

  ON_CORESYSTEMS_SHUTDOWN
  {
    #if (BUILDSYSTEM_ENABLE_D3D12_SUPPORT || BUILDSYSTEM_ENABLE_VULKAN_SUPPORT) && (XII_ENABLED(XII_PLATFORM_WINDOWS) || XII_ENABLED(XII_PLATFORM_LINUX))
    s_pDxcUtils = {};
    s_pDxcCompiler = {};
    #endif
  }

XII_END_SUBSYSTEM_DECLARATION;

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiShaderCompilerProgram, 1, xiiRTTIDefaultAllocator<xiiShaderCompilerProgram>)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

////////// Utility Functions //////////

xiiStringView xiiShaderCompilerProgram::GetProfileName(xiiStringView sPlatform, xiiBitflags<xiiGALShaderStage> Stage)
{
  if (sPlatform == "NULL_SM")
    return "null_sm";

  xiiStringBuilder sPlatformStripped = sPlatform;
  sPlatformStripped.ReplaceFirst("D3D_", "");
  sPlatformStripped.ReplaceFirst("VK_", "");

  if (xiiStringUtils::IsEqual(sPlatformStripped, "SM40_93"))
  {
    if (Stage.IsSet(xiiGALShaderStage::Vertex))
      return "vs_4_0_level_9_3";
    if (Stage.IsSet(xiiGALShaderStage::Pixel))
      return "ps_4_0_level_9_3";
  }

  if (xiiStringUtils::IsEqual(sPlatformStripped, "SM40"))
  {
    if (Stage.IsSet(xiiGALShaderStage::Vertex))
      return "vs_4_0";
    if (Stage.IsSet(xiiGALShaderStage::Geometry))
      return "gs_4_0";
    if (Stage.IsSet(xiiGALShaderStage::Pixel))
      return "ps_4_0";
    if (Stage.IsSet(xiiGALShaderStage::Compute))
      return "cs_4_0";
  }

  if (xiiStringUtils::IsEqual(sPlatformStripped, "SM41"))
  {
    if (Stage.IsSet(xiiGALShaderStage::Geometry))
      return "gs_4_0";
    if (Stage.IsSet(xiiGALShaderStage::Vertex))
      return "vs_4_1";
    if (Stage.IsSet(xiiGALShaderStage::Pixel))
      return "ps_4_1";
    if (Stage.IsSet(xiiGALShaderStage::Compute))
      return "cs_4_1";
  }

  if (xiiStringUtils::IsEqual(sPlatformStripped, "SM50"))
  {
    if (Stage.IsSet(xiiGALShaderStage::Vertex))
      return "vs_5_0";
    if (Stage.IsSet(xiiGALShaderStage::Hull))
      return "hs_5_0";
    if (Stage.IsSet(xiiGALShaderStage::Domain))
      return "ds_5_0";
    if (Stage.IsSet(xiiGALShaderStage::Geometry))
      return "gs_5_0";
    if (Stage.IsSet(xiiGALShaderStage::Pixel))
      return "ps_5_0";
    if (Stage.IsSet(xiiGALShaderStage::Compute))
      return "cs_5_0";
  }

  if (xiiStringUtils::IsEqual(sPlatformStripped, "SM51"))
  {
    if (Stage.IsSet(xiiGALShaderStage::Vertex))
      return "vs_5_1";
    if (Stage.IsSet(xiiGALShaderStage::Hull))
      return "hs_5_1";
    if (Stage.IsSet(xiiGALShaderStage::Domain))
      return "ds_5_1";
    if (Stage.IsSet(xiiGALShaderStage::Geometry))
      return "gs_5_1";
    if (Stage.IsSet(xiiGALShaderStage::Pixel))
      return "ps_5_1";
    if (Stage.IsSet(xiiGALShaderStage::Compute))
      return "cs_5_1";
  }

  if (xiiStringUtils::IsEqual(sPlatformStripped, "SM60"))
  {
    if (Stage.IsSet(xiiGALShaderStage::Vertex))
      return "vs_6_0";
    if (Stage.IsSet(xiiGALShaderStage::Hull))
      return "hs_6_0";
    if (Stage.IsSet(xiiGALShaderStage::Domain))
      return "ds_6_0";
    if (Stage.IsSet(xiiGALShaderStage::Geometry))
      return "gs_6_0";
    if (Stage.IsSet(xiiGALShaderStage::Pixel))
      return "ps_6_0";
    if (Stage.IsSet(xiiGALShaderStage::Compute))
      return "cs_6_0";
  }

  if (xiiStringUtils::IsEqual(sPlatformStripped, "SM61"))
  {
    if (Stage.IsSet(xiiGALShaderStage::Vertex))
      return "vs_6_1";
    if (Stage.IsSet(xiiGALShaderStage::Hull))
      return "hs_6_1";
    if (Stage.IsSet(xiiGALShaderStage::Domain))
      return "ds_6_1";
    if (Stage.IsSet(xiiGALShaderStage::Geometry))
      return "gs_6_1";
    if (Stage.IsSet(xiiGALShaderStage::Pixel))
      return "ps_6_1";
    if (Stage.IsSet(xiiGALShaderStage::Compute))
      return "cs_6_1";
  }

  if (xiiStringUtils::IsEqual(sPlatformStripped, "SM62"))
  {
    if (Stage.IsSet(xiiGALShaderStage::Vertex))
      return "vs_6_2";
    if (Stage.IsSet(xiiGALShaderStage::Hull))
      return "hs_6_2";
    if (Stage.IsSet(xiiGALShaderStage::Domain))
      return "ds_6_2";
    if (Stage.IsSet(xiiGALShaderStage::Geometry))
      return "gs_6_2";
    if (Stage.IsSet(xiiGALShaderStage::Pixel))
      return "ps_6_2";
    if (Stage.IsSet(xiiGALShaderStage::Compute))
      return "cs_6_2";
  }

  if (xiiStringUtils::IsEqual(sPlatformStripped, "SM63"))
  {
    if (Stage.IsSet(xiiGALShaderStage::Vertex))
      return "vs_6_3";
    if (Stage.IsSet(xiiGALShaderStage::Hull))
      return "hs_6_3";
    if (Stage.IsSet(xiiGALShaderStage::Domain))
      return "ds_6_3";
    if (Stage.IsSet(xiiGALShaderStage::Geometry))
      return "gs_6_3";
    if (Stage.IsSet(xiiGALShaderStage::Pixel))
      return "ps_6_3";
    if (Stage.IsSet(xiiGALShaderStage::Compute))
      return "cs_6_3";

    if (Stage.IsAnySet(xiiGALShaderStage::RayGeneration | xiiGALShaderStage::RayMiss | xiiGALShaderStage::RayAnyHit | xiiGALShaderStage::RayClosestHit | xiiGALShaderStage::RayIntersection | xiiGALShaderStage::Callable))
      return "lib_6_3";
  }

  if (xiiStringUtils::IsEqual(sPlatformStripped, "SM64"))
  {
    if (Stage.IsSet(xiiGALShaderStage::Vertex))
      return "vs_6_4";
    if (Stage.IsSet(xiiGALShaderStage::Hull))
      return "hs_6_4";
    if (Stage.IsSet(xiiGALShaderStage::Domain))
      return "ds_6_4";
    if (Stage.IsSet(xiiGALShaderStage::Geometry))
      return "gs_6_4";
    if (Stage.IsSet(xiiGALShaderStage::Pixel))
      return "ps_6_4";
    if (Stage.IsSet(xiiGALShaderStage::Compute))
      return "cs_6_4";

    if (Stage.IsAnySet(xiiGALShaderStage::RayGeneration | xiiGALShaderStage::RayMiss | xiiGALShaderStage::RayAnyHit | xiiGALShaderStage::RayClosestHit | xiiGALShaderStage::RayIntersection | xiiGALShaderStage::Callable))
      return "lib_6_4";
  }

  if (xiiStringUtils::IsEqual(sPlatformStripped, "SM65"))
  {
    if (Stage.IsSet(xiiGALShaderStage::Vertex))
      return "vs_6_5";
    if (Stage.IsSet(xiiGALShaderStage::Hull))
      return "hs_6_5";
    if (Stage.IsSet(xiiGALShaderStage::Domain))
      return "ds_6_5";
    if (Stage.IsSet(xiiGALShaderStage::Geometry))
      return "gs_6_5";
    if (Stage.IsSet(xiiGALShaderStage::Pixel))
      return "ps_6_5";
    if (Stage.IsSet(xiiGALShaderStage::Compute))
      return "cs_6_5";

    if (Stage.IsAnySet(xiiGALShaderStage::RayGeneration | xiiGALShaderStage::RayMiss | xiiGALShaderStage::RayAnyHit | xiiGALShaderStage::RayClosestHit | xiiGALShaderStage::RayIntersection | xiiGALShaderStage::Callable))
      return "lib_6_5";

    if (Stage.IsSet(xiiGALShaderStage::Amplification))
      return "as_6_5";

    if (Stage.IsSet(xiiGALShaderStage::Mesh))
      return "ms_6_5";
  }

  if (xiiStringUtils::IsEqual(sPlatformStripped, "SM66"))
  {
    if (Stage.IsSet(xiiGALShaderStage::Vertex))
      return "vs_6_6";
    if (Stage.IsSet(xiiGALShaderStage::Hull))
      return "hs_6_6";
    if (Stage.IsSet(xiiGALShaderStage::Domain))
      return "ds_6_6";
    if (Stage.IsSet(xiiGALShaderStage::Geometry))
      return "gs_6_6";
    if (Stage.IsSet(xiiGALShaderStage::Pixel))
      return "ps_6_6";
    if (Stage.IsSet(xiiGALShaderStage::Compute))
      return "cs_6_6";

    if (Stage.IsAnySet(xiiGALShaderStage::RayGeneration | xiiGALShaderStage::RayMiss | xiiGALShaderStage::RayAnyHit | xiiGALShaderStage::RayClosestHit | xiiGALShaderStage::RayIntersection | xiiGALShaderStage::Callable))
      return "lib_6_6";

    if (Stage.IsSet(xiiGALShaderStage::Amplification))
      return "as_6_6";

    if (Stage.IsSet(xiiGALShaderStage::Mesh))
      return "ms_6_6";
  }

  XII_REPORT_FAILURE("Unknown (or unsupported) Platform '{0}' or Stage {1}", sPlatform, Stage.GetValue());

  return {};
}

xiiEnum<xiiGALGraphicsDeviceType> xiiShaderCompilerProgram::GetProfileNameDeviceType(xiiStringView sPlatform, xiiStringView sProfileName)
{
  xiiStringBuilder sPlatform0 = sPlatform;
  xiiStringBuilder sProfile   = sProfileName;

  if (sPlatform0 == "NULL_SM")
  {
    return xiiGALGraphicsDeviceType::Null;
  }

  if (sPlatform0.FindSubString("D3D_") != nullptr)
  {
    if (sProfile.FindSubString("s_6") != nullptr || sProfile.FindSubString("s_5_1") != nullptr)
    {
      return xiiGALGraphicsDeviceType::Direct3D12;
    }
    else
    {
      return xiiGALGraphicsDeviceType::Direct3D11;
    }
  }

  if (sPlatform0.FindSubString("VK_") != nullptr)
  {
    return xiiGALGraphicsDeviceType::Vulkan;
  }

  XII_ASSERT_NOT_IMPLEMENTED;

  return xiiGALGraphicsDeviceType::Undefined;
}

xiiResult xiiShaderCompilerProgram::Initialize(xiiStringView sPlatformName)
{
  if (m_InputLayoutMapping.IsEmpty())
  {
    m_InputLayoutMapping["POSITION"]  = xiiGALInputLayoutSemantic::Position;
    m_InputLayoutMapping["POSITION0"] = xiiGALInputLayoutSemantic::Position;

    m_InputLayoutMapping["TANGENT"]  = xiiGALInputLayoutSemantic::Tangent;
    m_InputLayoutMapping["TANGENT0"] = xiiGALInputLayoutSemantic::Tangent;

    m_InputLayoutMapping["NORMAL"]  = xiiGALInputLayoutSemantic::Normal;
    m_InputLayoutMapping["NORMAL0"] = xiiGALInputLayoutSemantic::Normal;

    m_InputLayoutMapping["COLOR0"] = xiiGALInputLayoutSemantic::Color0;
    m_InputLayoutMapping["COLOR1"] = xiiGALInputLayoutSemantic::Color1;
    m_InputLayoutMapping["COLOR2"] = xiiGALInputLayoutSemantic::Color2;
    m_InputLayoutMapping["COLOR3"] = xiiGALInputLayoutSemantic::Color3;
    m_InputLayoutMapping["COLOR4"] = xiiGALInputLayoutSemantic::Color4;
    m_InputLayoutMapping["COLOR5"] = xiiGALInputLayoutSemantic::Color5;
    m_InputLayoutMapping["COLOR6"] = xiiGALInputLayoutSemantic::Color6;
    m_InputLayoutMapping["COLOR7"] = xiiGALInputLayoutSemantic::Color7;

    m_InputLayoutMapping["TEXCOORD0"] = xiiGALInputLayoutSemantic::TexCoord0;
    m_InputLayoutMapping["TEXCOORD1"] = xiiGALInputLayoutSemantic::TexCoord1;
    m_InputLayoutMapping["TEXCOORD2"] = xiiGALInputLayoutSemantic::TexCoord2;
    m_InputLayoutMapping["TEXCOORD3"] = xiiGALInputLayoutSemantic::TexCoord3;
    m_InputLayoutMapping["TEXCOORD4"] = xiiGALInputLayoutSemantic::TexCoord4;
    m_InputLayoutMapping["TEXCOORD5"] = xiiGALInputLayoutSemantic::TexCoord5;
    m_InputLayoutMapping["TEXCOORD6"] = xiiGALInputLayoutSemantic::TexCoord6;
    m_InputLayoutMapping["TEXCOORD7"] = xiiGALInputLayoutSemantic::TexCoord7;
    m_InputLayoutMapping["TEXCOORD8"] = xiiGALInputLayoutSemantic::TexCoord8;
    m_InputLayoutMapping["TEXCOORD9"] = xiiGALInputLayoutSemantic::TexCoord9;

    m_InputLayoutMapping["BITANGENT"]  = xiiGALInputLayoutSemantic::BiTangent;
    m_InputLayoutMapping["BITANGENT0"] = xiiGALInputLayoutSemantic::BiTangent;

    m_InputLayoutMapping["BONEINDICES0"] = xiiGALInputLayoutSemantic::BoneIndices0;
    m_InputLayoutMapping["BONEINDICES1"] = xiiGALInputLayoutSemantic::BoneIndices1;

    m_InputLayoutMapping["BONEWEIGHTS0"] = xiiGALInputLayoutSemantic::BoneWeights0;
    m_InputLayoutMapping["BONEWEIGHTS1"] = xiiGALInputLayoutSemantic::BoneWeights1;
  }

  return XII_SUCCESS;
}

xiiResult xiiShaderCompilerProgram::ModifyShaderSource(xiiShaderProgramData& inout_data, xiiLogInterface* pLog)
{
  return XII_SUCCESS;
}

xiiResult xiiShaderCompilerProgram::Compile(xiiShaderProgramData& inout_Data, xiiLogInterface* pLog)
{
  XII_SUCCEED_OR_RETURN(Initialize(inout_Data.m_sPlatform));

  for (xiiUInt32 stage = 0; stage < xiiGALShaderStage::ENUM_COUNT; ++stage)
  {
    xiiBitflags<xiiGALShaderStage> stageFlag = xiiGALShaderStage::GetStageFlag(stage);

    if (inout_Data.m_uiSourceHash[stage] == 0)
      continue;

    if (inout_Data.m_bWriteToDisk[stage] == false)
    {
      xiiLog::Debug("Shader for stage '{}' is already compiled.", xiiGALShaderStage::Names[stage]);
      continue;
    }

    const xiiStringBuilder sShaderSource = inout_Data.m_sShaderSource[stage];

    if (!sShaderSource.IsEmpty() && sShaderSource.FindSubString("main") != nullptr)
    {
      const xiiStringBuilder sSourceFile = inout_Data.m_sSourceFile;

      xiiEnum<xiiGALGraphicsDeviceType> device = GetProfileNameDeviceType(inout_Data.m_sPlatform, GetProfileName(inout_Data.m_sPlatform, stageFlag));

      switch (device)
      {
        case xiiGALGraphicsDeviceType::Null:
          return XII_SUCCESS;

#if BUILDSYSTEM_ENABLE_D3D11_SUPPORT
        case xiiGALGraphicsDeviceType::Direct3D11:
        {
          xiiShaderCompilerD3D11 shaderCompilerD3D11;
          if (shaderCompilerD3D11.CompileShader(inout_Data.m_sSourceFile, sShaderSource, inout_Data.m_Flags.IsSet(xiiShaderCompilerFlags::Debug), GetProfileName(inout_Data.m_sPlatform, stageFlag), "main", inout_Data.m_ByteCode[stage]->m_ByteCode).Succeeded())
          {
            XII_SUCCEED_OR_RETURN(shaderCompilerD3D11.ReflectShaderStage(inout_Data, stageFlag, m_InputLayoutMapping));
          }
          else
          {
            return XII_FAILURE;
          }
        }
        break;
#endif
#if BUILDSYSTEM_ENABLE_D3D12_SUPPORT
        case xiiGALGraphicsDeviceType::Direct3D12:
        {
          xiiShaderCompilerD3D12 shaderCompilerD3D12(s_pDxcUtils.RawPtr(), s_pDxcCompiler.RawPtr());
          if (shaderCompilerD3D12.CompileShader(inout_Data.m_sSourceFile, sShaderSource, inout_Data.m_Flags.IsSet(xiiShaderCompilerFlags::Debug), GetProfileName(inout_Data.m_sPlatform, stageFlag), "main", inout_Data.m_ByteCode[stage]->m_ByteCode).Succeeded())
          {
            XII_SUCCEED_OR_RETURN(shaderCompilerD3D12.ReflectShaderStage(inout_Data, stageFlag, m_InputLayoutMapping));
          }
          else
          {
            return XII_FAILURE;
          }
        }
        break;
#endif
#if BUILDSYSTEM_ENABLE_VULKAN_SUPPORT && (XII_ENABLED(XII_PLATFORM_WINDOWS) || XII_ENABLED(XII_PLATFORM_LINUX))
        case xiiGALGraphicsDeviceType::Vulkan:
        {
          xiiShaderCompilerVulkan shaderCompilerVulkan(s_pDxcUtils.RawPtr(), s_pDxcCompiler.RawPtr());
          if (shaderCompilerVulkan.CompileShader(inout_Data.m_sSourceFile, sShaderSource, inout_Data.m_Flags.IsSet(xiiShaderCompilerFlags::Debug), GetProfileName(inout_Data.m_sPlatform, stageFlag), "main", inout_Data.m_ByteCode[stage]->m_ByteCode).Succeeded())
          {
            XII_SUCCEED_OR_RETURN(shaderCompilerVulkan.ReflectShaderStage(inout_Data, stageFlag, m_InputLayoutMapping));
          }
          else
          {
            return XII_FAILURE;
          }
        }
        break;
#endif

          XII_DEFAULT_CASE_NOT_IMPLEMENTED;
      }
    }
  }
  return XII_SUCCESS;
}
