#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/States/PipelineResourceSignature.h>

// clang-format off

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALPipelineResourceFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALPipelineResourceFlags::None),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineResourceFlags::NoDynamicBuffers),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineResourceFlags::CombinedSampler),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineResourceFlags::Formattedbuffer),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineResourceFlags::RuntimeArray),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineResourceFlags::GeneralInputAttachment),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALPipelineResourceSignature, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;

// clang-format on

xiiGALPipelineResourceSignature::xiiGALPipelineResourceSignature(xiiGALDevice* pDevice, const xiiGALPipelineResourceSignatureCreationDescription& creationDescription) :
  xiiGALDeviceObject(pDevice), m_Description(creationDescription)
{
}

xiiGALPipelineResourceSignature::~xiiGALPipelineResourceSignature() = default;

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_States_Implementation_PipelineResourceSignature);
