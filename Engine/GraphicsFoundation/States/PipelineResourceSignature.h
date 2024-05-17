#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Declarations/DeviceObject.h>
#include <GraphicsFoundation/Resources/Sampler.h>
#include <GraphicsFoundation/Shader/Shader.h>
#include <GraphicsFoundation/Shader/ShaderResourceVariable.h>

/// \brief This describes the pipeline resource property flags.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALPipelineResourceFlags
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    None             = 0U,               ///< The resource has no special properties.
    NoDynamicBuffers = XII_BIT(0),       ///< This indicates that dynamic buffers will never be bound to the resource variable. This applies to xiiGALShaderResourceType::ConstantBuffer, xiiGALShaderResourceType::BufferUAV, and xiiGALShaderResourceType::BufferSRV resources.
                                         ///
                                         /// \remarks In Vulkan and Direct3D12 graphics implementations, dynamic buffers require extra work at run time. If an application knows it will never bind a dynamic buffer to the variable, it should use xiiGALPipelineResourceFlags::NoDynamicBuffers flag to improve performance.
                                         ///          This flag is not required and non-dynamic buffers will still work even if the flag is not used. It is an error to bind a dynamic buffer to resource that uses xiiGALPipelineResourceFlags::NoDynamicBuffers flag.
    CombinedSampler = XII_BIT(1),        ///< This indicates that a texture SRV will be combined with a sampler. It applies to xiiGALShaderResourceType::TextureSRV resources.
    Formattedbuffer = XII_BIT(2),        ///< This indicates this variable will be used to bind formatted buffers. It applies to xiiGALShaderResourceType::BufferUAV and xiiGALShaderResourceType::BufferSRV resources.
                                         ///
                                         /// \remarks In the Vulkan graphics implementation, formatted buffers require another descriptor type as opposed to structured buffers. If an application will be using formatted buffers with buffer UAVs and SRVs, it must specify the xiiGALPipelineResourceFlags::FormattedBuffer flag.
    RuntimeArray           = XII_BIT(3), ///< This indicates that the resource is a run-time sized array (e.g., an array without a specific size).
    GeneralInputAttachment = XII_BIT(4), ///< This indicates that the resource is an input attachment in general layout, which allows simultaneously reading from the resource through the input attachment and writing to it via color or depth-stencil attachment.
                                         ///
                                         /// \note This flag is only valid in the Vulkan graphics implementation.

    Default = None
  };

  struct Bits
  {
    StorageType NoDynamicBuffers : 1;
    StorageType CombinedSampler : 1;
    StorageType Formattedbuffer : 1;
    StorageType RuntimeArray : 1;
    StorageType GeneralInputAttachment : 1;
  };
};

XII_DECLARE_FLAGS_OPERATORS(xiiGALPipelineResourceFlags);

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALPipelineResourceFlags);

/// \brief This describes an immutable sampler used by a graphics pipeline.
///
/// An immutable sampler is compiled into the pipeline state and cannot be modified. It is generally more efficient than a regular sampler and should be used whenever possible.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALImmutableSamplerDescription : public xiiHashableStruct<xiiGALImmutableSamplerDescription>
{
  XII_DECLARE_POD_TYPE();

  xiiHashedString                  m_SamplerOrTextureName;                      ///< The name of the sampler itself or the name of the texture variable that this immutable sampler is assigned to if combined texture samplers are used.
  xiiBitflags<xiiGALShaderStage>   m_ShaderStages = xiiGALShaderStage::Unknown; ///< The shader stages that this immutable sampler applies to. More than one shader stage can be specified. The default is xiiGALShaderStage::Unknown.
  xiiGALSamplerCreationDescription m_SamplerDescription;                        ///< The sampler creation description. See xiiGALSamplerCreationDescription for details.
};

/// \brief This describes the pipeline resource information.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALPipelineResourceDescription : public xiiHashableStruct<xiiGALPipelineResourceDescription>
{
  XII_DECLARE_POD_TYPE();

  xiiHashedString                m_sName;                                                                        ///< The resource name in the shader.
  xiiBitflags<xiiGALShaderStage> m_ShaderStages = xiiGALShaderStage::Unknown;                                    ///< The shader stages that this resource applies to. When multiple shader stages are specified, all stages will share the same resource. The default is xiiGALShaderStage::Unknown.
                                                                                                                 ///
                                                                                                                 ///  \remarks There may be multiple resources with the same name in different shader stages, but the stages specified for different resources with the same name must not overlap.
  xiiUInt32                                 m_uiArraySize           = 0U;                                        ///< The resource array size (must be set to 1 for non-array resources). The default is 0.
  xiiUInt32                                 m_uiBindSet             = 0U;                                        ///< \brief The resource bind set, for descriptor sets.
  xiiUInt32                                 m_uiBindSlot            = 0U;                                        ///< \brief The resource bind slot in the bind set.
  xiiEnum<xiiGALShaderResourceType>         m_ResourceType          = xiiGALShaderResourceType::Unknown;         ///< The resource type, see xiiGALShaderResourceType. The default is xiiGALShaderResourceType::Unknown.
  xiiEnum<xiiGALShaderResourceVariableType> m_ResourceVariableType  = xiiGALShaderResourceVariableType::Mutable; ///< The resource variable type, see xiiGALShaderResourceVariableType. The default is xiiGALShaderResourceVariableType::Mutable.
  xiiBitflags<xiiGALPipelineResourceFlags>  m_PipelineResourceFlags = xiiGALPipelineResourceFlags::None;         ///< Special resource flags, see xiiGALPipelineResourceFlags. The default is xiiGALPipelineResourceFlags::None.
};

/// \brief This describes the pipeline resource signature creation description.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALPipelineResourceSignatureCreationDescription : public xiiHashableStruct<xiiGALPipelineResourceSignatureCreationDescription>
{
  xiiStringView                                         m_sName;                                        ///< Resource name. The default is an empty string view.
  xiiUInt8                                              m_uiBindingIndex = 0U;                          ///< The binding index that this resource signature uses. Every resource signature must be assign to one signature slot. The total number of slots is given by XII_GAL_MAX_RESOURCE_SIGNATURES_COUNT constant. All resource signatures used by a pipeline state must be assigned to different slots.
  xiiHybridArray<xiiGALPipelineResourceDescription, 2U> m_Resources;                                    ///< An array of resource descriptions, see xiiGALPipelineResourceDescription for details.
  xiiHybridArray<xiiGALImmutableSamplerDescription, 2U> m_ImmutableSamplers;                            ///< An array of immutable samplers, see xiiGALImmutableSamplerDescription for details.
  bool                                                  m_bUseCombinedTextureSamplers = false;          ///< If set to true, textures will be combined with texture samplers. The CombinedSamplerSuffix member defines the suffix added to the texture variable name to get corresponding sampler name. When using combined samplers, the sampler assigned to the shader resource view is automatically set when the view is bound. Otherwise, samplers need to be explicitly set similar to other shader variables. The default is false.
  xiiStringView                                         m_sCombinedSamplerSuffix      = "_AutoSampler"; ///< If combined texture sampler usage is enabled, defines the suffix added to the texture variable name to get corresponding sampler name.  For example, for default value "_AutoSampler", a texture named "Tex" will be combined with sampler named "Tex_AutoSampler". If UseCombinedTextureSamplers is false, this member is ignored. The default is "_AutoSampler".
};

/// \brief Interface that defines methods of the pipeline resource signature.
class XII_GRAPHICSFOUNDATION_DLL xiiGALPipelineResourceSignature : public xiiGALDeviceObject
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALPipelineResourceSignature, xiiGALDeviceObject);

public:
  /// \brief This returns the creation description for this object.
  XII_NODISCARD const xiiGALPipelineResourceSignatureCreationDescription& GetDescription() const;

  /// \brief Returns true if this pipeline resource signature is compatible with the given pipeline resource signature.
  virtual XII_NODISCARD bool IsCompatibleWith(const xiiGALPipelineResourceSignature* pPipelineResourceSignature) const = 0;

protected:
  friend class xiiGALDevice;

  xiiGALPipelineResourceSignature(xiiGALDevice* pDevice, const xiiGALPipelineResourceSignatureCreationDescription& creationDescription);

  virtual ~xiiGALPipelineResourceSignature();

  virtual xiiResult InitPlatform() = 0;

  virtual xiiResult DeInitPlatform() = 0;

protected:
  xiiGALPipelineResourceSignatureCreationDescription m_Description;
};

#include <GraphicsFoundation/States/Implementation/PipelineResourceSignature_inl.h>
