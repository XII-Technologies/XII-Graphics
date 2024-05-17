#pragma once

#include <ShaderCompiler/ShaderCompilerDLL.h>

#include <GraphicsCore/ShaderCompiler/ShaderCompiler.h>
#include <GraphicsFoundation/Shader/InputLayout.h>

class XII_SHADERCOMPILER_DLL xiiShaderCompilerProgram : public xiiShaderProgramCompiler
{
  XII_ADD_DYNAMIC_REFLECTION(xiiShaderCompilerProgram, xiiShaderProgramCompiler);

public:
  virtual void GetSupportedPlatforms(xiiHybridArray<xiiString, 4>& Platforms) override
  {
    Platforms.PushBack("NULL_SM");

#if BUILDSYSTEM_ENABLE_D3D11_SUPPORT
    Platforms.PushBack("D3D_SM40_93");
    Platforms.PushBack("D3D_SM40");
    Platforms.PushBack("D3D_SM41");
    Platforms.PushBack("D3D_SM50");
#endif

#if BUILDSYSTEM_ENABLE_D3D12_SUPPORT && XII_ENABLED(XII_PLATFORM_WINDOWS)
    Platforms.PushBack("D3D_SM51");
    Platforms.PushBack("D3D_SM60"); /// Wave intrinsics, 64-bit integers
    Platforms.PushBack("D3D_SM61"); /// SV_ViewID, SV_Barycentrics
    Platforms.PushBack("D3D_SM62"); /// 16-bit types, Denorm mode
    Platforms.PushBack("D3D_SM63"); /// Hardware accelerated ray tracing
    Platforms.PushBack("D3D_SM64"); /// Shader integer dot product, SV_ShadingRate
    Platforms.PushBack("D3D_SM65"); /// DXR1.1 (KHR ray tracing), Mesh and Amplification shaders, additional Wave intrinsics (Partial Support is available)
    Platforms.PushBack("D3D_SM66"); /// VK_NV_compute_shader_derivatives, VK_KHR_shader_atomic_int64 (Partial Support is available)
#endif

#if BUILDSYSTEM_ENABLE_VULKAN_SUPPORT && (XII_ENABLED(XII_PLATFORM_WINDOWS) || XII_ENABLED(XII_PLATFORM_LINUX))
    // Only supported with Vulkan
    Platforms.PushBack("VK_SM60"); /// Wave intrinsics, 64-bit integers
    Platforms.PushBack("VK_SM61"); /// SV_ViewID, SV_Barycentrics
    Platforms.PushBack("VK_SM62"); /// 16-bit types, Denorm mode
    Platforms.PushBack("VK_SM63"); /// Hardware accelerated ray tracing
    Platforms.PushBack("VK_SM64"); /// Shader integer dot product, SV_ShadingRate
    Platforms.PushBack("VK_SM65"); /// DXR1.1 (KHR ray tracing), Mesh and Amplification shaders, additional Wave intrinsics (Partial Support is available)
    Platforms.PushBack("VK_SM66"); /// VK_NV_compute_shader_derivatives, VK_KHR_shader_atomic_int64 (Partial Support is available)
#endif
  }

  virtual xiiResult ModifyShaderSource(xiiShaderProgramData& inout_data, xiiLogInterface* pLog) override;

  virtual xiiResult Compile(xiiShaderProgramData& inout_Data, xiiLogInterface* pLog) override;

  xiiStringView GetProfileName(xiiStringView sPlatform, xiiBitflags<xiiGALShaderStage> Stage);

  xiiEnum<xiiGALGraphicsDeviceType> GetProfileNameDeviceType(xiiStringView sPlatform, xiiStringView sProfileName);

private:
  xiiResult Initialize(xiiStringView sPlatformName);

  xiiMap<xiiStringView, xiiEnum<xiiGALInputLayoutSemantic>> m_InputLayoutMapping;
};
