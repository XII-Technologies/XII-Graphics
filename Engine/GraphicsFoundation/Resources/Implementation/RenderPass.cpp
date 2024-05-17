#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/Resources/RenderPass.h>

// clang-format off

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALAttachmentLoadOperation, 1)
  XII_ENUM_CONSTANT(xiiGALAttachmentLoadOperation::Load),
  XII_ENUM_CONSTANT(xiiGALAttachmentLoadOperation::Clear),
  XII_ENUM_CONSTANT(xiiGALAttachmentLoadOperation::Discard),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALAttachmentStoreOperation, 1)
  XII_ENUM_CONSTANT(xiiGALAttachmentStoreOperation::Store),
  XII_ENUM_CONSTANT(xiiGALAttachmentStoreOperation::Discard),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALRenderPass, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;

// clang-format on

xiiGALRenderPass::xiiGALRenderPass(xiiGALDevice* pDevice, const xiiGALRenderPassCreationDescription& creationDescription) :
  xiiGALDeviceObject(pDevice), m_Description(creationDescription)
{
}

xiiGALRenderPass::~xiiGALRenderPass() = default;

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_Resources_Implementation_RenderPass);
