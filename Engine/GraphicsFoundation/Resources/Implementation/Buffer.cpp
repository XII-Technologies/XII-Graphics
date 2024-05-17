#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/Device/Device.h>
#include <GraphicsFoundation/Resources/Buffer.h>

// clang-format off

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALBufferMode, 1)
  XII_ENUM_CONSTANT(xiiGALBufferMode::Undefined),
  XII_ENUM_CONSTANT(xiiGALBufferMode::Formatted),
  XII_ENUM_CONSTANT(xiiGALBufferMode::Structured),
  XII_ENUM_CONSTANT(xiiGALBufferMode::Raw),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALMiscBufferFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALMiscBufferFlags::None),
  XII_BITFLAGS_CONSTANT(xiiGALMiscBufferFlags::SparseAlias),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALBuffer, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;

// clang-format on

xiiGALBuffer::xiiGALBuffer(xiiGALDevice* pDevice, const xiiGALBufferCreationDescription& creationDescription) :
  xiiGALResource(pDevice), m_Description(creationDescription)
{
}

xiiGALBuffer::~xiiGALBuffer() = default;

void xiiGALBuffer::CreateDefaultResourceViews(xiiGALBufferHandle hBuffer)
{
  // Cannot create default views for formatted buffers, since the view format is unknown at creation time.
  if (m_Description.m_Mode == xiiGALBufferMode::Formatted)
    return;

  xiiGALBufferViewCreationDescription viewDescription;
  viewDescription.m_hBuffer      = hBuffer;
  viewDescription.m_ViewType     = xiiGALBufferViewType::ShaderResource;
  viewDescription.m_uiByteOffset = 0;
  viewDescription.m_uiByteWidth  = 0;

  if (m_Description.m_BindFlags.IsSet(xiiGALBindFlags::ShaderResource))
  {
    m_DefaultBufferViews[xiiGALBufferViewType::ShaderResource] = m_pDevice->CreateBufferView(viewDescription);
  }
  if (m_Description.m_BindFlags.IsSet(xiiGALBindFlags::UnorderedAccess))
  {
    m_DefaultBufferViews[xiiGALBufferViewType::UnorderedAccess] = m_pDevice->CreateBufferView(viewDescription);
  }
}

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_Resources_Implementation_Buffer);
