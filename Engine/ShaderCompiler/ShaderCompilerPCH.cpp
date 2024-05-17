#include <ShaderCompiler/ShaderCompilerPCH.h>

XII_STATICLINK_LIBRARY(ShaderCompiler)
{
  if (bReturn)
    return;

  XII_STATICLINK_REFERENCE(ShaderCompiler_Implementation_ShaderCompiler);
  XII_STATICLINK_REFERENCE(ShaderCompiler_Implementation_D3D_Implementation_ShaderCompilerD3D12);
  XII_STATICLINK_REFERENCE(ShaderCompiler_Implementation_Vulkan_Implementation_ShaderCompilerVulkan);
}
