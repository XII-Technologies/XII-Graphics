#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <Foundation/Math/Size.h>
#include <GraphicsFoundation/Declarations/DeviceObject.h>
#include <GraphicsFoundation/Declarations/GraphicsTypes.h>

/// \brief A special constant used to indicate that the render pass is unused.
#define XII_GAL_ATTACHMENT_UNUSED 0xFFFFFFFFU

/// \brief A special sub pass index value expanding synchronization scope outside a sub pass.
#define XII_GAL_SUBPASS_EXTERNAL 0xFFFFFFFFU

/// \brief This describes the render pass load operation.
///
/// Vulkan counterpart: [VkAttachmentLoadOp](https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VkAttachmentLoadOp).
/// D3D12 counterpart: [D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE](https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ne-d3d12-d3d12_render_pass_beginning_access_type).
struct XII_GRAPHICSFOUNDATION_DLL xiiGALAttachmentLoadOperation
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    Load = 0U, ///< The previous contents of the texture within the render area will be preserved.
    Clear,     ///< The contents within the render area will be cleared to a uniform value, which is specified when a render pass instance is begun.
    Discard,   ///< The previous contents within the area need not be preserved; the contents of the attachment will be undefined inside the render area.

    ENUM_COUNT,

    Default = Load
  };
};

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALAttachmentLoadOperation);

/// \brief This describes the render pass store operation.
///
/// Vulkan counterpart: [VkAttachmentStoreOp](https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VkAttachmentStoreOp).
/// D3D12 counterpart: [D3D12_RENDER_PASS_ENDING_ACCESS_TYPE](https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ne-d3d12-d3d12_render_pass_ending_access_type).
struct XII_GRAPHICSFOUNDATION_DLL xiiGALAttachmentStoreOperation
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    Store = 0U, ///< The contents generated during the render pass and within the render area are written to memory.
    Discard,    ///< The contents within the render area are not needed after rendering, and may be discarded; the contents of the attachment will be undefined inside the render area.

    ENUM_COUNT,

    Default = Store
  };
};

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALAttachmentStoreOperation);

/// \brief This describes the render pass attachment creation description.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALRenderPassAttachmentDescription : public xiiHashableStruct<xiiGALRenderPassAttachmentDescription>
{
  XII_DECLARE_POD_TYPE();

  xiiEnum<xiiGALTextureFormat>            m_Format                = xiiGALTextureFormat::Unknown;          ///< The format of the texture view that will be used for the attachment. The default is xiiGALTextureFormat::Unknown.
  xiiUInt8                                m_uiSampleCount         = 0U;                                    ///< The number of samples in the texture. The default is 0.
  xiiEnum<xiiGALAttachmentLoadOperation>  m_LoadOperation         = xiiGALAttachmentLoadOperation::Load;   ///< The load operation that specifies how the contents of color and depth components of the attachment are treated at the beginning of the sub pass where it is first used. The default is xiiGALAttachmentLoadOperation::Load.
  xiiEnum<xiiGALAttachmentStoreOperation> m_StoreOperation        = xiiGALAttachmentStoreOperation::Store; ///< The store operation how the contents of color and depth components of the attachment are treated at the end of the sub pass where it is last used. The default is xiiGALAttachmentStoreOperation::Store.
  xiiEnum<xiiGALAttachmentLoadOperation>  m_StencilLoadOperation  = xiiGALAttachmentLoadOperation::Load;   ///< The load operation that specifies how the contents of the stencil component of the attachment is treated at the beginning of the sub pass where it is first used. This value is ignored when the format does not have stencil component. The default is xiiGALAttachmentLoadOperation::Load.
  xiiEnum<xiiGALAttachmentStoreOperation> m_StencilStoreOperation = xiiGALAttachmentStoreOperation::Store; ///< The store operation how the contents of the stencil component of the attachment is treated at the end of the sub pass where it is last used. This value is ignored when the format does not have stencil component. The default is xiiGALAttachmentStoreOperation::Store.
  xiiBitflags<xiiGALResourceStateFlags>   m_InitialStateFlags     = xiiGALResourceStateFlags::Unknown;     ///< The state the attachment texture sub-resource will be in when a render pass instance begins. The default is xiiGALResourceStateFlags::Unknown.
  xiiBitflags<xiiGALResourceStateFlags>   m_FinalStateFlags       = xiiGALResourceStateFlags::Unknown;     ///< The state the attachment texture sub-resource will be transitioned to when a render pass instance ends. The default is xiiGALResourceStateFlags::Unknown.
};

/// \brief This describes the render pass attachment reference.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALAttachmentReferenceDescription : public xiiHashableStruct<xiiGALAttachmentReferenceDescription>
{
  XII_DECLARE_POD_TYPE();

  xiiUInt32                             m_uiAttachmentIndex  = XII_GAL_ATTACHMENT_UNUSED;         ///< Either an integer value identifying an attachment at the corresponding index in the xiiGALRenderPassCreationDescription, or XII_GAL_ATTACHMENT_UNUSED to signify that this attachment is not used. The default is 0.
  xiiBitflags<xiiGALResourceStateFlags> m_ResourceStateFlags = xiiGALResourceStateFlags::Unknown; ///< The state of the attachment during the sub pass.
};

/// \brief This describes the shading rate attachment.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALShadingRateAttachmentDescription : public xiiHashableStruct<xiiGALShadingRateAttachmentDescription>
{
  XII_DECLARE_POD_TYPE();

  xiiGALAttachmentReferenceDescription m_AttachmentReference;           ///< The shading rate attachment reference.
  xiiSizeU32                           m_TileSize = xiiSizeU32(0U, 0U); ///< Each texel in the attachment contains shading rate for the whole tile. The size must be a power-of-two value between xiiGALShadingRateProperties::MinTileSize and xiiGALShadingRateProperties::MaxTileSize. Keep zero (default) to use the default tile size.
};

/// \brief This describes the render pass sub pass.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALSubPassDescription : public xiiHashableStruct<xiiGALSubPassDescription>
{
  xiiHybridArray<xiiGALAttachmentReferenceDescription, 16U>  m_InputAttachments;        ///< An array of input attachments.
  xiiHybridArray<xiiGALAttachmentReferenceDescription, 16U>  m_RenderTargetAttachments; ///< An array of color render target attachments. Each element of the m_RenderTargetAttachments array corresponds to an output in the pixel shader, i.e. if the shader declares an output variable decorated with a render target index X, then it uses the attachment provided in m_RenderTargetAttachments[X]. If the attachment index is XII_GAL_ATTACHMENT_UNUSED, writes to this render target are ignored.
  xiiHybridArray<xiiGALAttachmentReferenceDescription, 16U>  m_ResolveAttachments;      ///< An array of resolve attachments. If m_ResolveAttachments is not nullptr, each of its elements corresponds to a render target attachment (the element in m_RenderTargetAttachments at the same index), and a multisample resolve operation is defined for each attachment. At the end of each sub pass, multisample resolve operations read the sub pass's color attachments, and resolve the samples for each pixel within the render area to the same pixel location in the corresponding resolve attachments, unless the resolve attachment index is XII_GAL_ATTACHMENT_UNUSED.
  xiiStaticArray<xiiGALAttachmentReferenceDescription, 1U>   m_DepthStencilAttachment;  ///< An array of depth-stencil attachment. Note that this array can only hold a single depth stencil attachment reference.
  xiiHybridArray<xiiUInt32, 16U>                             m_PreserveAttachments;     ///< An array of preserve attachments.
  xiiStaticArray<xiiGALShadingRateAttachmentDescription, 1U> m_ShadingRateAttachment;   ///< An array of shading rate attachment. Note that this array can only hold a single shading rate attachment reference.
};

/// \brief This describes the sub pass dependency.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALSubPassDependencyDescription : public xiiHashableStruct<xiiGALSubPassDependencyDescription>
{
  XII_DECLARE_POD_TYPE();

  xiiUInt32                             m_uiSourceSubPass        = 0U;                                  ///< The sub pass index of the first sub pass in the dependency, or XII_GAL_SUBPASS_EXTERNAL. The default is 0.
  xiiUInt32                             m_uiDestinationSubPass   = 0U;                                  ///< The sub pass index of the second sub pass in the dependency, or XII_GAL_SUBPASS_EXTERNAL. The default is 0.
  xiiBitflags<xiiGALPipelineStageFlags> m_SourceStageFlags       = xiiGALPipelineStageFlags::Undefined; ///< The xiiGALPipelineStageFlags that specifies the source stage flag. The default is xiiGALPipelineStageFlags::Undefined.
  xiiBitflags<xiiGALPipelineStageFlags> m_DestinationStageFlags  = xiiGALPipelineStageFlags::Undefined; ///< The xiiGALPipelineStageFlags that specifies the destination stage flag. The default is xiiGALPipelineStageFlags::Undefined.
  xiiBitflags<xiiGALAccessFlags>        m_SourceAccessFlags      = xiiGALAccessFlags::None;             ///< The xiiGALAccessFlags that specifies the source access flag. The default is xiiGALAccessFlags::None.
  xiiBitflags<xiiGALAccessFlags>        m_DestinationAccessFlags = xiiGALAccessFlags::None;             ///< The xiiGALAccessFlags that specifies the destination access flag. The default is xiiGALAccessFlags::None.
};

/// \brief This describes the render pass creation description.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALRenderPassCreationDescription : public xiiHashableStruct<xiiGALRenderPassCreationDescription>
{
  xiiStringView                                              m_sName;        ///< Resource name. The default is an empty string view.
  xiiHybridArray<xiiGALRenderPassAttachmentDescription, 16U> m_Attachments;  ///< An array of sub pass attachments.
  xiiHybridArray<xiiGALSubPassDescription, 16U>              m_SubPasses;    ///< An array of sub pass descriptions.
  xiiHybridArray<xiiGALSubPassDependencyDescription, 16U>    m_Dependencies; ///< An array of sub pass dependencies.
};

/// \brief Interface that defines methods to manipulate a render pass object.
class XII_GRAPHICSFOUNDATION_DLL xiiGALRenderPass : public xiiGALDeviceObject
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALRenderPass, xiiGALDeviceObject);

public:
  /// \brief This returns the creation description for this object.
  XII_NODISCARD const xiiGALRenderPassCreationDescription& GetDescription() const;

protected:
  friend class xiiGALDevice;

  xiiGALRenderPass(xiiGALDevice* pDevice, const xiiGALRenderPassCreationDescription& creationDescription);

  virtual ~xiiGALRenderPass();

  virtual xiiResult InitPlatform() = 0;

  virtual xiiResult DeInitPlatform() = 0;

protected:
  xiiGALRenderPassCreationDescription m_Description;
};

#include <GraphicsFoundation/Resources/Implementation/RenderPass_inl.h>
