#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Declarations/DeviceObject.h>
#include <GraphicsFoundation/Declarations/GraphicsTypes.h>
#include <GraphicsFoundation/Shader/ShaderResourceVariable.h>

/// \brief This describes the shader variable property flags.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALShaderVariableFlags
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    None             = 0U,         ///< Shader variable has no special properties.
    NoDynamicBuffers = XII_BIT(0), ///< This indicates that dynamic buffers will never be bound to the resource variable. This applies to uniform (constant) buffers, unordered access views, and shader resource views.
                                   ///<
                                   ///< \remarks This flag directly translates to the xiiGALPipelineResourceFlag::NoDynamicBuffers in the internal pipeline resource signature.
    InputAttachment = XII_BIT(1),  ///<          This indicates that the resource is an input attachment in general layout, which allows simultaneously reading from the resource through the input attachment and writing to it via color or depth-stencil attachment.
                                   ///<
                                   ///< \note This flag is only valid in Vulkan.

    ENUM_COUNT = 3U,

    Default = None
  };

  struct Bits
  {
    StorageType NoDynamicBuffers : 1;
    StorageType InputAttachment : 1;
  };
};

XII_DECLARE_FLAGS_OPERATORS(xiiGALShaderVariableFlags);

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALShaderVariableFlags);

/// \brief This describes the pipeline state shading rate flags.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALPipelineShadingRateFlags
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    None         = 0U,         ///< Shader variable has no special properties.
    PerPrimitive = XII_BIT(0), ///< This indicates that the pipeline state will be used with per draw or per primitive shading rate.
    TextureBased = XII_BIT(1), ///< This indicates that the pipeline state will be used with texture-based shading rate.

    Default = None
  };

  struct Bits
  {
    StorageType PerPrimitive : 1;
    StorageType TextureBased : 1;
  };
};

XII_DECLARE_FLAGS_OPERATORS(xiiGALPipelineShadingRateFlags);

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALPipelineShadingRateFlags);

/// \brief This describes the shader variable property flags.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALPipelineType
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    Graphics,   ///< Graphics pipeline.
    Compute,    ///< Compute pipeline.
    Mesh,       ///< Mesh pipeline.
    RayTracing, ///< Ray tracing pipeline.
    Tile,       ///< Tile pipeline.

    ENUM_COUNT,

    Invalid = 0xFF,

    Default = Graphics
  };
};

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALPipelineType);

/// \brief This describes the sample information.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALSampleDescription : public xiiHashableStruct<xiiGALSampleDescription>
{
  XII_DECLARE_POD_TYPE();

  xiiUInt8 m_uiCount   = 1U; ///< The sample count.
  xiiUInt8 m_uiQuality = 0U; ///< The sample quality.
};

/// \brief This describes the shader variable information.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALShaderResourceVariableDescription : public xiiHashableStruct<xiiGALShaderResourceVariableDescription>
{
  XII_DECLARE_POD_TYPE();

  xiiStringView                             m_sName;                                                   ///< The shader variable name.
  xiiBitflags<xiiGALShaderStage>            m_ShaderStages = xiiGALShaderStage::Unknown;               ///< The shader stages this resources variable applies to. If more than one shader stage is specified, the variable will be shared between these stages. Shader stages used by different variables with the same name must not overlap.
  xiiEnum<xiiGALShaderResourceVariableType> m_Type         = xiiGALShaderResourceVariableType::Static; ///< The shader variable type.
  xiiBitflags<xiiGALShaderVariableFlags>    m_Flags        = xiiGALShaderVariableFlags::None;          ///< The shader variable flags.
};

/// \brief This describes graphics pipeline information.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALGraphicsPipelineDescription : public xiiHashableStruct<xiiGALGraphicsPipelineDescription>
{
  XII_DECLARE_POD_TYPE();

  xiiGALBlendStateHandle                      m_hBlendState;                                               ///< The handle to the blend state object.
  xiiUInt32                                   m_uiSampleMask = 0xFFFFFFFF;                                 ///< A 32-bit sample mask that determines which samples get updated in all the active render targets. A sample mask is always applied; it is independent of whether multisampling is enabled, and does not depend on whether an application uses multisample render targets.
  xiiGALRasterizerStateHandle                 m_hRasterizerState;                                          ///< The handle to rasterizer state object.
  xiiGALDepthStencilStateHandle               m_hDepthStencilState;                                        ///< The handle to depth-stencil state object.
  xiiGALInputLayoutHandle                     m_hInputLayout;                                              ///< The handle to vertex input layout, ignored in a mesh pipeline.
  xiiEnum<xiiGALPrimitiveTopology>            m_PrimitiveTopology = xiiGALPrimitiveTopology::TriangleList; ///< The primitive topology type, ignored in a mesh pipeline. The default is xiiGALPrimitiveTopology::TriangleList.
  xiiUInt8                                    m_uiViewportCount   = 1U;                                    ///< The number of viewports used by this pipeline. The default is 1.
  xiiUInt8                                    m_uiSubpassIndex    = 0U;                                    ///< The subpass index within the render pass. The default is 0.
  xiiBitflags<xiiGALPipelineShadingRateFlags> m_ShadingRateFlags  = xiiGALPipelineShadingRateFlags::None;  ///< Shading rate flags that specify which type of the shading rate will be used with this pipeline. The default is xiiGALPipelineShadingRateFlags::None.
  xiiGALSampleDescription                     m_SampleDescription;                                         ///< Multi-sampling parameters.
  xiiGALRenderPassHandle                      m_hRenderPass;                                               ///< The handle to the render pass object.
};

/// \brief This describes the ray tracing pipeline information.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALRayTracingPipelineDescription : public xiiHashableStruct<xiiGALRayTracingPipelineDescription>
{
  XII_DECLARE_POD_TYPE();

  xiiUInt16 m_uiShaderRecordSize  = 0U; ///< Size of the additional data passed to the shader. Shader record size plus shader group size (32 bytes) must be aligned to 32 bytes. Shader record size plus shader group size (32 bytes) must not exceed 4096 bytes
  xiiUInt8  m_uiMaxRecursionDepth = 0U; ///< Number of recursive calls of TraceRay() in HLSL. Zero means no tracing of rays at all, only ray-gen shader will be executed. See Device MaxRayTracingRecursionDepth.
};

/// \brief This describes the tile pipeline information.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALTilePipelineDescription : public xiiHashableStruct<xiiGALTilePipelineDescription>
{
  xiiEnum<xiiGALSampleCount>                                                   m_SampleCount = xiiGALSampleCount::OneSample; ///< The number of samples in the render targets.
  xiiStaticArray<xiiEnum<xiiGALTextureFormat>, XII_GAL_MAX_RENDERTARGET_COUNT> m_RenderTargetFormats;                        ///< The render target formats.
};

/// \brief This describes the pipeline state creation description.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALPipelineStateCreationDescription : public xiiHashableStruct<xiiGALPipelineStateCreationDescription>
{
  XII_DECLARE_POD_TYPE();

  xiiStringView                         m_sName;                                       ///< Resource name. The default is an empty string view.
  xiiEnum<xiiGALPipelineType>           m_PipelineType = xiiGALPipelineType::Graphics; ///< The pipeline type. The default is xiiGALPipelineType::Graphics.
  xiiGALPipelineResourceSignatureHandle m_hPipelineResourceSignature;                  ///< The pipeline resource signature that contains the shader resource description.
  xiiGALShaderHandle                    m_hShader;                                     ///< The shader that contains the valid shader code for the specified pipeline type.
  xiiGALGraphicsPipelineDescription     m_GraphicsPipeline;                            ///< The graphics pipeline description, see xiiGALGraphicsPipelineDescription.
  xiiGALRayTracingPipelineDescription   m_RayTracingPipeline;                          ///< The ray tracing pipeline description, see xiiGALRayTracingPipelineDescription.
  xiiGALTilePipelineDescription         m_TilePipeline;                                ///< The tile pipeline description, see xiiGALTilePipelineDescription.
  xiiUInt32                             m_uiNodeMask             = 0x0;                ///< Node mask.
  xiiUInt64                             m_uiImmediateContextMask = XII_BIT(0);         ///< Defines which immediate contexts are allowed to execute commands that use this texture. The default is the main immediate context.
                                                                                       ///< Only specify the bits that indicate those immediate contexts where the resource will be used, setting unnecessary bits will result in extra overhead.

  /// \brief Returns true if this pipeline state is a graphics pipeline.
  bool IsAnyGraphicsPipeline() const;

  /// \brief Returns true if this pipeline state is a compute pipeline.
  bool IsComputePipeline() const;

  /// \brief Returns true if this pipeline state is a ray tracing pipeline.
  bool IsRayTracingPipeline() const;

  /// \brief Returns true if this pipeline state is a tile pipeline.
  bool IsTilePipeline() const;
};

/// \brief Interface that defines methods to manipulate a pipeline state object.
class XII_GRAPHICSFOUNDATION_DLL xiiGALPipelineState : public xiiGALDeviceObject
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALPipelineState, xiiGALDeviceObject);

public:
  /// \brief This returns the creation description for this object.
  XII_NODISCARD const xiiGALPipelineStateCreationDescription& GetDescription() const;

  /// \brief This is used to set the constant (uniform) buffer for a shader resource.
  ///
  /// \param bindingInformation - This describes the binding information for the shader resource, see xiiGALPipelineResourceDescription for details.
  /// \param hConstantBuffer    - The handle to the constant (uniform) buffer object to set.
  void SetConstantBuffer(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALBufferHandle hConstantBuffer);

  /// \brief This is used to set the buffer view for a shader resource.
  ///
  /// \param bindingInformation - This describes the binding information for the shader resource, see xiiGALPipelineResourceDescription for details.
  /// \param hBufferView        - The handle to the buffer view object to set.
  void SetShaderResourceBufferView(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALBufferViewHandle hBufferView);

  /// \brief This is used to set the texture view for a shader resource.
  ///
  /// \param bindingInformation - This describes the binding information for the shader resource, see xiiGALPipelineResourceDescription for details.
  /// \param hTextureView       - The handle to the texture view object to set.
  void SetShaderResourceTextureView(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALTextureViewHandle hTextureView);

  /// This is used to set the buffer view for an unordered access.
  ///
  /// \param bindingInformation - This describes the binding information for the shader resource, see xiiGALPipelineResourceDescription for details.
  /// \param hBufferView        - The handle to the buffer view object to set.
  void SetUnorderedAccessBufferView(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALBufferViewHandle hBufferView);

  /// \brief This is used to set the texture view for an unordered access.
  ///
  /// \param bindingInformation - This describes the binding information for the shader resource, see xiiGALPipelineResourceDescription for details.
  /// \param hTextureView       - The handle to the texture view object to set.
  void SetUnorderedAccessTextureView(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALTextureViewHandle hTextureView);

  /// \brief This is used to set the sampler for a shader resource.
  ///
  /// \param bindingInformation - This describes the binding information for the shader resource, see xiiGALPipelineResourceDescription for details.
  /// \param hSampler           - The handle to the sampler object to set.
  void SetSampler(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALSamplerHandle hSampler);

  /// \brief Resets the bound resources.
  virtual void ResetBoundResources() = 0;

protected:
  friend class xiiGALDevice;

  xiiGALPipelineState(xiiGALDevice* pDevice, const xiiGALPipelineStateCreationDescription& creationDescription);

  virtual ~xiiGALPipelineState();

  virtual xiiResult InitPlatform() = 0;

  virtual xiiResult DeInitPlatform() = 0;

  // Deactivate Doxygen document generation for the following block. (API abstraction only)
  /// \cond

  // These functions need to be implemented by a graphics API abstraction.

protected:
  virtual void SetConstantBufferPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALBuffer* pConstantBuffer)               = 0;
  virtual void SetShaderResourceBufferViewPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALBufferView* pBufferView)     = 0;
  virtual void SetShaderResourceTextureViewPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALTextureView* pTextureView)  = 0;
  virtual void SetUnorderedAccessBufferViewPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALBufferView* pBufferView)    = 0;
  virtual void SetUnorderedAccessTextureViewPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALTextureView* pTextureView) = 0;
  virtual void SetSamplerPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALSampler* pSampler)                            = 0;

  /// \endcond

protected:
  xiiGALPipelineStateCreationDescription m_Description;
};

#include <GraphicsFoundation/States/Implementation/PipelineState_inl.h>
