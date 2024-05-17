#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/Resources/BottomLevelAS.h>

// clang-format off

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALRayTracingBuildASFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALRayTracingBuildASFlags::None),
  XII_BITFLAGS_CONSTANT(xiiGALRayTracingBuildASFlags::AllowUpdate),
  XII_BITFLAGS_CONSTANT(xiiGALRayTracingBuildASFlags::AllowCompaction),
  XII_BITFLAGS_CONSTANT(xiiGALRayTracingBuildASFlags::PreferFastTrace),
  XII_BITFLAGS_CONSTANT(xiiGALRayTracingBuildASFlags::PreferFastBuild),
  XII_BITFLAGS_CONSTANT(xiiGALRayTracingBuildASFlags::LowMemory),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALBottomLevelAS, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;

// clang-format on

xiiGALBottomLevelAS::xiiGALBottomLevelAS(xiiGALDevice* pDevice, const xiiGALBottomLevelASCreationDescription& creationDescription) :
  xiiGALDeviceObject(pDevice), m_Description(creationDescription)
{
}

xiiGALBottomLevelAS::~xiiGALBottomLevelAS() = default;

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_Resources_Implementation_BottomLevelAS);
