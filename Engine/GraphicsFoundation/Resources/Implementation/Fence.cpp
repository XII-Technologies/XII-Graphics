#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/Resources/Fence.h>

// clang-format off

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALFenceType, 1)
  XII_ENUM_CONSTANT(xiiGALFenceType::CpuWaitOnly),
  XII_ENUM_CONSTANT(xiiGALFenceType::General),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALFence, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;

// clang-format on

xiiGALFence::xiiGALFence(xiiGALDevice* pDevice, const xiiGALFenceCreationDescription& creationDescription) :
  xiiGALDeviceObject(pDevice), m_Description(creationDescription)
{
}

xiiGALFence::~xiiGALFence() = default;

void xiiGALFence::UpdateLastCompletedFenceValue(xiiUInt64 uiValue)
{
  m_uiLastCompletedFenceValue.Max(uiValue);
}

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_Resources_Implementation_Fence);
