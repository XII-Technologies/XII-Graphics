#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <Foundation/Containers/HashTable.h>
#include <Foundation/Containers/IdTable.h>
#include <Foundation/Memory/CommonAllocators.h>
#include <Foundation/Strings/HashedString.h>
#include <GraphicsFoundation/Declarations/Descriptors.h>
#include <GraphicsFoundation/Declarations/Object.h>
#include <GraphicsFoundation/Utilities/DescriptorHash.h>

/// \brief The xiiRenderDevice class is the primary interface for interactions with rendering APIs.
/// It contains a set of (non-virtual) functions to set state, create resources etc. which rely on API specific implementations provided by protected virtual functions.
/// Redundant state changes are prevented at the platform independent level in the non-virtual functions.
class XII_GRAPHICSFOUNDATION_DLL xiiGALDevice : public xiiGALObject
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALDevice, xiiGALObject);

public:
  /// \brief Initialize device.
  xiiResult Initialize();

  /// \brief Shutdown device.
  xiiResult Shutdown();


  /// \brief Begins a pipeline scope.
  void BeginPipeline(xiiStringView sName, xiiGALSwapChainHandle hSwapChain);

  /// \brief Ends a pipeline scope.
  void EndPipeline(xiiGALSwapChainHandle hSwapChain);


  /// \brief Begins a render frame.
  void BeginFrame(const xiiUInt64 uiRenderFrame = 0U);

  /// \brief Ends a render frame.
  void EndFrame();

public:
  /// \brief This creates a swap chain object.
  ///
  /// \param description - The swap chain description. See xiiGALSwapChainCreationDescription.
  ///
  /// \return The handle to the created swap chain object. The function calls AddRef(), so that the new object will have one reference.
  XII_NODISCARD xiiGALSwapChainHandle CreateSwapChain(const xiiGALSwapChainCreationDescription& description);

  /// \brief This destroys the swap chain with the given handle.
  void DestroySwapChain(xiiGALSwapChainHandle hSwapChain);


  /// \brief This creates a new command queue object.
  ///
  /// \param description - The command queue description. See xiiGALCommandQueueCreationDescription.
  ///
  /// \return The handle to the created command queue object. The function calls AddRef(), so that the new object will have one reference.
  XII_NODISCARD xiiGALCommandQueueHandle CreateCommandQueue(const xiiGALCommandQueueCreationDescription& description);

  /// \brief This destroys the command queue with the given handle.
  void DestroyCommandQueue(xiiGALCommandQueueHandle hCommandQueue);


  /// \brief This creates a new blend state object.
  ///
  /// \param description - The blend state description. See xiiGALBlendStateCreationDescription.
  ///
  /// \return The handle to the created blend state object. The function calls AddRef(), so that the new object will have one reference.
  XII_NODISCARD xiiGALBlendStateHandle CreateBlendState(const xiiGALBlendStateCreationDescription& description);

  /// \brief This destroys the blend state with the given handle.
  void DestroyBlendState(xiiGALBlendStateHandle hBlendState);


  /// \brief This creates a new depth stencil state object.
  ///
  /// \param description - The depth stencil state description. See xiiGALDepthStencilStateCreationDescription.
  ///
  /// \return The handle to the created depth stencil state object. The function calls AddRef(), so that the new object will have one reference.
  XII_NODISCARD xiiGALDepthStencilStateHandle CreateDepthStencilState(const xiiGALDepthStencilStateCreationDescription& description);

  /// \brief This destroys the depth stencil state with the given handle.
  void DestroyDepthStencilState(xiiGALDepthStencilStateHandle hDepthStencilState);


  /// \brief This creates a new rasterizer state object.
  ///
  /// \param description - The rasterizer state description. See xiiGALRasterizerStateCreationDescription.
  ///
  /// \return The handle to the created rasterizer state object. The function calls AddRef(), so that the new object will have one reference.
  XII_NODISCARD xiiGALRasterizerStateHandle CreateRasterizerState(const xiiGALRasterizerStateCreationDescription& description);

  /// \brief This destroys the rasterizer state with the given handle.
  void DestroyRasterizerState(xiiGALRasterizerStateHandle hRasterizerState);


  /// \brief This creates a new shader object.
  ///
  /// \param description - The shader description. See xiiGALShaderCreationDescription.
  ///
  /// \return The handle to the created shader object. The function calls AddRef(), so that the new object will have one reference.
  XII_NODISCARD xiiGALShaderHandle CreateShader(const xiiGALShaderCreationDescription& description);

  /// \brief This destroys the shader with the given handle.
  void DestroyShader(xiiGALShaderHandle hShader);


  /// \brief This creates a new buffer object.
  ///
  /// \param description  - The buffer description. See xiiGALBufferCreationDescription.
  /// \param pInitialData - The pointer to the xiiGALBufferData structure that describes the initial buffer data or nullptr if no data is provided.
  ///                       Immutable buffers (xiiGALResourceUsage::Immutable) must be initialized during creation.
  ///
  /// \return The handle to the created buffer object. The function calls AddRef(), so that the new object will have one reference.
  ///
  /// \remarks Size of a uniform buffer (xiiGALBindFlags) must be multiple of 16.\n
  /// Stride of a formatted buffer will be computed automatically from the format if the m_uiElementByteStride member of buffer description is set to default value (0).
  XII_NODISCARD xiiGALBufferHandle CreateBuffer(const xiiGALBufferCreationDescription& description, const xiiGALBufferData* pInitialData = nullptr);

  /// \brief This destroys the buffer with the given handle.
  void DestroyBuffer(xiiGALBufferHandle hBuffer);


  /// \brief This creates a new buffer view.
  ///
  /// \param description - The buffer view description. See xiiGALBufferViewCreationDescription.
  ///
  /// \return The handle to the buffer view.
  ///
  /// \remarks To create a view addressing the entire buffer, set only xiiGALBufferViewCreationDescription::m_ViewType member of the ViewDesc structure and leave all other members in their default values.
  ///          The buffer view will contain strong reference to the buffer, so the buffer will not be destroyed until all views are released.
  ///          The function calls AddRef() for the created interface, so it must be released by a call to ReleaseRef() when it is no longer needed.
  XII_NODISCARD xiiGALBufferViewHandle CreateBufferView(xiiGALBufferViewCreationDescription& description);

  /// \brief This destroys the buffer view with the given handle.
  void DestroyBufferView(xiiGALBufferViewHandle hBufferView);


  /// \brief This creates a new texture object.
  ///
  /// \param description  - The texture description. See xiiGALTextureCreationDescription.
  /// \param pInitialData - The pointer to the xiiGALTextureData structure that describes the initial texture data or nullptr if no data is provided.
  ///                       Immutable textures (xiiGALResourceUsage::Immutable) must be initialized during creation.
  ///
  /// \return The handle to the created texture object. The function calls AddRef(), so that the new object will have one reference.
  ///
  /// \remarks
  /// To create all mip levels, set the description.m_uiMipLevels to zero.\n Multi-sampled resources cannot be initialized with data when they are created. \n
  /// If initial data is provided, number of sub-resources must exactly match the number of sub-resources in the texture (which is the number of mip levels times the number of array slices. For a 3D texture, this is just the number of mip levels).
  ///
  /// For example, for a 15 x 6 x 2 2D texture array, the following array of sub-resources should be provided: \n 15x6, 7x3, 3x1, 1x1, 15x6, 7x3, 3x1, 1x1.\n
  /// For a 15 x 6 x 4 3D texture, the following array of sub-resources should be provided:\n 15x6x4, 7x3x2, 3x1x1, 1x1x1
  XII_NODISCARD xiiGALTextureHandle CreateTexture(const xiiGALTextureCreationDescription& description, const xiiGALTextureData* pInitialData = nullptr);

  /// \brief This destroys the texture with the given handle.
  void DestroyTexture(xiiGALTextureHandle hTexture);


  /// \brief This creates a new texture view.
  ///
  /// \param description - The texture view description. See xiiGALTextureViewCreationDescription.
  ///
  /// \return The handle to the texture view.
  ///
  /// \remarks To create a shader resource view addressing the entire texture, set only xiiGALTextureViewCreationDescription::m_ViewType member of the description parameter to xiiGALTextureViewType::ShaderResource and leave all other
  ///          members in their default values. Using the same method, you can create render target or depth stencil view addressing the largest mip level.\n
  ///          If texture view format is xiiGALTextureFormat::Unknown, the view format will match the texture format.\n
  ///          If texture view type is xiiGALTextureViewType::Undefined, the type will match the texture type.\n
  ///          If the number of mip levels is 0, and the view type is shader resource, the view will address all mip levels. For other view types it will address one mip level.\n
  ///          If the number of slices is 0, all slices from m_uiFirstArraySlice or m_uiFirstDepthSlice will be referenced by the view.
  ///          For non-array textures, the only allowed values for the number of slices are 0 and 1.\n
  ///          Texture view will contain strong reference to the texture, so the texture will not be destroyed until all views are released.\n
  ///          The function calls AddRef() for the created interface, so it must be released by a call to ReleaseRef() when it is no longer needed.
  XII_NODISCARD xiiGALTextureViewHandle CreateTextureView(xiiGALTextureViewCreationDescription& description);

  /// \brief This destroys the texture view with the given handle.
  void DestroyTextureView(xiiGALTextureViewHandle hTextureView);


  /// \brief This creates a new sampler object.
  ///
  /// \param description - The sampler description. See xiiGALSamplerCreationDescription.
  ///
  /// \return The handle to the created sampler object. The function calls AddRef(), so that the new object will have one reference.
  ///
  /// \remark If an application attempts to create a sampler interface with the same attributes as an existing interface, the same interface will be returned.
  XII_NODISCARD xiiGALSamplerHandle CreateSampler(const xiiGALSamplerCreationDescription& description);

  /// \brief This destroys the sampler with the given handle.
  void DestroySampler(xiiGALSamplerHandle hSampler);


  /// \brief This creates a new input layout object.
  ///
  /// \param description - The input layout description. See xiiGALInputLayoutCreationDescription.
  ///
  /// \return The handle to the created input layout object. The function calls AddRef(), so that the new object will have one reference.
  XII_NODISCARD xiiGALInputLayoutHandle CreateInputLayout(const xiiGALInputLayoutCreationDescription& description);

  /// \brief This destroys the input layout with the given handle.
  void DestroyInputLayout(xiiGALInputLayoutHandle hInputLayout);


  /// \brief This creates a new query object.
  ///
  /// \param description - The query description. See xiiGALQueryCreationDescription.
  ///
  /// \return The handle to the created query object. The function calls AddRef(), so that the new object will have one reference.
  XII_NODISCARD xiiGALQueryHandle CreateQuery(const xiiGALQueryCreationDescription& description);

  /// \brief This destroys the sampler with the given handle.
  void DestroyQuery(xiiGALQueryHandle hQuery);


  /// \brief This creates a new fence object.
  ///
  /// \param description - The fence description. See xiiGALFenceCreationDescription.
  ///
  /// \return The handle to the created fence object. The function calls AddRef(), so that the new object will have one reference.
  XII_NODISCARD xiiGALFenceHandle CreateFence(const xiiGALFenceCreationDescription& description);

  /// \brief This destroys the fence with the given handle.
  void DestroyFence(xiiGALFenceHandle hFence);


  /// \brief This creates a new render pass object.
  ///
  /// \param description - The render pass description. See xiiGALRenderPassCreationDescription.
  ///
  /// \return The handle to the created render pass object. The function calls AddRef(), so that the new object will have one reference.
  XII_NODISCARD xiiGALRenderPassHandle CreateRenderPass(const xiiGALRenderPassCreationDescription& description);

  /// \brief This destroys the render pass with the given handle.
  void DestroyRenderPass(xiiGALRenderPassHandle hRenderPass);


  /// \brief This creates a new frame buffer object.
  ///
  /// \param description - The frame buffer description. See xiiGALFramebufferCreationDescription.
  ///
  /// \return The handle to the created frame buffer object. The function calls AddRef(), so that the new object will have one reference.
  XII_NODISCARD xiiGALFramebufferHandle CreateFramebuffer(const xiiGALFramebufferCreationDescription& description);

  /// \brief This destroys the frame buffer with the given handle.
  void DestroyFramebuffer(xiiGALFramebufferHandle hFramebuffer);


  /// \brief This creates a new bottom-level acceleration structure object.
  ///
  /// \param description - The bottom-level acceleration structure description. See xiiGALBottomLevelASCreationDescription.
  ///
  /// \return The handle to the created bottom-level acceleration structure object. The function calls AddRef(), so that the new object will have one reference.
  XII_NODISCARD xiiGALBottomLevelASHandle CreateBottomLevelAS(const xiiGALBottomLevelASCreationDescription& description);

  /// \brief This destroys the bottom-level acceleration structure with the given handle.
  void DestroyBottomLevelAS(xiiGALBottomLevelASHandle hBottomLevelAS);


  /// \brief This creates a new top-level acceleration structure object.
  ///
  /// \param description - The top-level acceleration structure description. See xiiGALTopLevelASCreationDescription.
  ///
  /// \return The handle to the created top-level acceleration structure object. The function calls AddRef(), so that the new object will have one reference.
  XII_NODISCARD xiiGALTopLevelASHandle CreateTopLevelAS(const xiiGALTopLevelASCreationDescription& description);

  /// \brief This destroys the top-level acceleration structure with the given handle.
  void DestroyTopLevelAS(xiiGALTopLevelASHandle hTopLevelAS);


  /// \brief This creates a new pipeline resource signature object.
  ///
  /// \param description - The pipeline resource signature description. See xiiGALPipelineResourceSignatureCreationDescription.
  ///
  /// \return The handle to the created pipeline resource signature object. The function calls AddRef(), so that the new object will have one reference.
  XII_NODISCARD xiiGALPipelineResourceSignatureHandle CreatePipelineResourceSignature(xiiGALPipelineResourceSignatureCreationDescription& description);

  /// \brief This destroys the pipeline resource signature with the given handle.
  void DestroyPipelineResourceSignature(xiiGALPipelineResourceSignatureHandle hPipelineResourceSignature);


  /// \brief This creates a new pipeline state object.
  ///
  /// \param description - The pipeline state description. See xiiGALPipelineStateCreationDescription.
  ///
  /// \return The handle to the created pipeline state object. The function calls AddRef(), so that the new object will have one reference.
  XII_NODISCARD xiiGALPipelineStateHandle CreatePipelineState(const xiiGALPipelineStateCreationDescription& description);

  /// \brief This destroys the pipeline state with the given handle.
  void DestroyPipelineState(xiiGALPipelineStateHandle hPipelineState);


  /// \brief Waits until all outstanding operations on the GPU are complete and destroys any pending resources and GPU objects.
  ///
  /// \note The method blocks the execution of the calling thread until the GPU is idle.
  ///
  /// \remarks The method does not flush immediate contexts, so it will only wait for commands that have been previously submitted for execution. An application should explicitly flush
  ///          the contexts using xiiGALCommandList::Flush() if it needs to make sure all recorded commands are complete when the method returns.
  void WaitIdle();

public:
  /// \brief Registers event handlers.
  static xiiEvent<const xiiGALDeviceEvent&> s_Events;

  /// \brief Returns the creation description for this device.
  XII_NODISCARD const xiiGALDeviceCreationDescription& GetDescription() const;

  /// \brief Retrieves a pointer to the compute queue if available, null otherwise.
  ///
  /// \param queueType - The queue type that has the required feature.
  ///
  /// \note The default graphics queue is guaranteed to exist, for a successful device initialization.
  XII_NODISCARD virtual xiiGALCommandQueue* GetDefaultCommandQueue(xiiBitflags<xiiGALCommandQueueType> queueType = xiiGALCommandQueueType::Graphics) const = 0;

  /// \brief Retrieves a pointer to the swap chain object with the given handle.
  XII_NODISCARD xiiGALSwapChain* GetSwapChain(xiiGALSwapChainHandle hSwapChain) const;

  /// \brief Retrieves a pointer to the command queue object with the given handle.
  XII_NODISCARD xiiGALCommandQueue* GetCommandQueue(xiiGALCommandQueueHandle hCommandQueue) const;

  /// \brief Retrieves a pointer to the blend state object with the given handle.
  XII_NODISCARD xiiGALBlendState* GetBlendState(xiiGALBlendStateHandle hBlendState) const;

  /// \brief Retrieves a pointer to the depth stencil state object with the given handle.
  XII_NODISCARD xiiGALDepthStencilState* GetDepthStencilState(xiiGALDepthStencilStateHandle hDepthStencilState) const;

  /// \brief Retrieves a pointer to the rasterizer state object with the given handle.
  XII_NODISCARD xiiGALRasterizerState* GetRasterizerState(xiiGALRasterizerStateHandle hRasterizerState) const;

  /// \brief Retrieves a pointer to the shader object with the given handle.
  XII_NODISCARD xiiGALShader* GetShader(xiiGALShaderHandle hShader) const;

  /// \brief Retrieves a pointer to the buffer object with the given handle.
  XII_NODISCARD xiiGALBuffer* GetBuffer(xiiGALBufferHandle hBuffer) const;

  /// \brief Retrieves a pointer to the texture object with the given handle.
  XII_NODISCARD xiiGALTexture* GetTexture(xiiGALTextureHandle hTexture) const;

  /// \brief Retrieves a pointer to the buffer view object with the given handle.
  XII_NODISCARD xiiGALBufferView* GetBufferView(xiiGALBufferViewHandle hBufferView) const;

  /// \brief Retrieves a pointer to the texture view object with the given handle.
  XII_NODISCARD xiiGALTextureView* GetTextureView(xiiGALTextureViewHandle hTextureView) const;

  /// \brief Retrieves a pointer to the sampler object with the given handle.
  XII_NODISCARD xiiGALSampler* GetSampler(xiiGALSamplerHandle hSampler) const;

  /// \brief Retrieves a pointer to the input layout object with the given handle.
  XII_NODISCARD xiiGALInputLayout* GetInputLayout(xiiGALInputLayoutHandle hInputLayout) const;

  /// \brief Retrieves a pointer to the query object with the given handle.
  XII_NODISCARD xiiGALQuery* GetQuery(xiiGALQueryHandle hQuery) const;

  /// \brief Retrieves a pointer to the fence object with the given handle.
  XII_NODISCARD xiiGALFence* GetFence(xiiGALFenceHandle hFence) const;

  /// \brief Retrieves a pointer to the render pass object with the given handle.
  XII_NODISCARD xiiGALRenderPass* GetRenderPass(xiiGALRenderPassHandle hRenderPass) const;

  /// \brief Retrieves a pointer to the framebuffer object with the given handle.
  XII_NODISCARD xiiGALFramebuffer* GetFramebuffer(xiiGALFramebufferHandle hFramebuffer) const;

  /// \brief Retrieves a pointer to the bottom-level acceleration structure object with the given handle.
  XII_NODISCARD xiiGALBottomLevelAS* GetBottomLevelAS(xiiGALBottomLevelASHandle hBottomLevelAS) const;

  /// \brief Retrieves a pointer to the top-level acceleration structure object with the given handle.
  XII_NODISCARD xiiGALTopLevelAS* GetTopLevelAS(xiiGALTopLevelASHandle hTopLevelAS) const;

  /// \brief Retrieves a pointer to the pipeline resource signature object with the given handle.
  XII_NODISCARD xiiGALPipelineResourceSignature* GetPipelineResourceSignature(xiiGALPipelineResourceSignatureHandle hPipelineResourceSignature) const;

  /// \brief Retrieves a pointer to the pipeline state object with the given handle.
  XII_NODISCARD xiiGALPipelineState* GetPipelineState(xiiGALPipelineStateHandle hPipelineState) const;

  /// \brief This retrieves the device properties. See xiiGraphicsDeviceAdapterDescription.
  XII_NODISCARD const xiiGALGraphicsDeviceAdapterDescription& GetGraphicsDeviceAdapterProperties() const;

  /// \brief This retrieves the device feature states. See xiiGALDeviceFeatures.
  XII_NODISCARD const xiiGALDeviceFeatures& GetFeatures() const;

  /// \brief This retrieves the device graphics API type. See xiiGALGraphicsDeviceType.
  XII_NODISCARD xiiEnum<xiiGALGraphicsDeviceType> GetGraphicsDeviceType() const;

  /// \brief This returns critical section lock.
  XII_NODISCARD xiiMutex& GetMutex() const;

  /// \brief Sets a default graphics device.
  ///
  /// \remarks This does not increase the reference count on the device.
  XII_NODISCARD static void SetDefaultDevice(xiiGALDevice* pDefaultDevice);

  /// \brief Retrieves the default device. This will be nullptr if none is set.
  XII_NODISCARD static xiiGALDevice* GetDefaultDevice();

  /// \brief This returns true if there is a set default device.
  XII_NODISCARD static bool HasDefaultDevice();

  /// \brief Returns the calculated memory consumption for texture.
  virtual xiiUInt64 GetMemoryConsumptionForTexture(const xiiGALTextureCreationDescription& description) const;

  /// \brief Returns the calculated memory consumption for buffer.
  virtual xiiUInt64 GetMemoryConsumptionForBuffer(const xiiGALBufferCreationDescription& description) const;

protected:
  xiiGALDevice(const xiiGALDeviceCreationDescription& creationDescription);

  virtual ~xiiGALDevice();

  template <typename IdTableType, typename ReturnType>
  ReturnType* Get(typename IdTableType::TypeOfId hHandle, const IdTableType& IdTable) const;

  template <typename HandleType>
  void AddDestroyedObject(xiiUInt32 uiType, HandleType handle);

  template <typename HandleType>
  void ReviveDestroyedObject(xiiUInt32 uiType, HandleType handle);

  void FlushDestroyedObjects();

  void DestroyViews(xiiGALResource* pResource);

  /// \brief Asserts that either this device supports multi-threaded resource creation, or that this function is executed on the main thread.
  void VerifyMultithreadedAccess() const;

  xiiGALDeviceCreationDescription m_Description;

  xiiProxyAllocator        m_Allocator;
  xiiLocalAllocatorWrapper m_AllocatorWrapper;

  mutable xiiMutex m_Mutex;

  using SwapChainTable                 = xiiIdTable<xiiGALSwapChainHandle::IdType, xiiGALSwapChain*, xiiLocalAllocatorWrapper>;
  using CommandQueueTable              = xiiIdTable<xiiGALCommandQueueHandle::IdType, xiiGALCommandQueue*, xiiLocalAllocatorWrapper>;
  using BlendStateTable                = xiiIdTable<xiiGALBlendStateHandle::IdType, xiiGALBlendState*, xiiLocalAllocatorWrapper>;
  using DepthStencilStateTable         = xiiIdTable<xiiGALDepthStencilStateHandle::IdType, xiiGALDepthStencilState*, xiiLocalAllocatorWrapper>;
  using RasterizerStateTable           = xiiIdTable<xiiGALRasterizerStateHandle::IdType, xiiGALRasterizerState*, xiiLocalAllocatorWrapper>;
  using ShaderTable                    = xiiIdTable<xiiGALShaderHandle::IdType, xiiGALShader*, xiiLocalAllocatorWrapper>;
  using BufferTable                    = xiiIdTable<xiiGALBufferHandle::IdType, xiiGALBuffer*, xiiLocalAllocatorWrapper>;
  using TextureTable                   = xiiIdTable<xiiGALTextureHandle::IdType, xiiGALTexture*, xiiLocalAllocatorWrapper>;
  using BufferViewTable                = xiiIdTable<xiiGALBufferViewHandle::IdType, xiiGALBufferView*, xiiLocalAllocatorWrapper>;
  using TextureViewTable               = xiiIdTable<xiiGALTextureViewHandle::IdType, xiiGALTextureView*, xiiLocalAllocatorWrapper>;
  using SamplerTable                   = xiiIdTable<xiiGALSamplerHandle::IdType, xiiGALSampler*, xiiLocalAllocatorWrapper>;
  using InputLayoutTable               = xiiIdTable<xiiGALInputLayoutHandle::IdType, xiiGALInputLayout*, xiiLocalAllocatorWrapper>;
  using QueryTable                     = xiiIdTable<xiiGALQueryHandle::IdType, xiiGALQuery*, xiiLocalAllocatorWrapper>;
  using FenceTable                     = xiiIdTable<xiiGALFenceHandle::IdType, xiiGALFence*, xiiLocalAllocatorWrapper>;
  using RenderPassTable                = xiiIdTable<xiiGALRenderPassHandle::IdType, xiiGALRenderPass*, xiiLocalAllocatorWrapper>;
  using FramebufferTable               = xiiIdTable<xiiGALFramebufferHandle::IdType, xiiGALFramebuffer*, xiiLocalAllocatorWrapper>;
  using BottomLevelASTable             = xiiIdTable<xiiGALBottomLevelASHandle::IdType, xiiGALBottomLevelAS*, xiiLocalAllocatorWrapper>;
  using TopLevelASTable                = xiiIdTable<xiiGALTopLevelASHandle::IdType, xiiGALTopLevelAS*, xiiLocalAllocatorWrapper>;
  using PipelineStateTable             = xiiIdTable<xiiGALPipelineStateHandle::IdType, xiiGALPipelineState*, xiiLocalAllocatorWrapper>;
  using PipelineResourceSignatureTable = xiiIdTable<xiiGALPipelineResourceSignatureHandle::IdType, xiiGALPipelineResourceSignature*, xiiLocalAllocatorWrapper>;

  SwapChainTable                 m_SwapChains;
  CommandQueueTable              m_CommandQueues;
  BlendStateTable                m_BlendStates;
  DepthStencilStateTable         m_DepthStencilStates;
  RasterizerStateTable           m_RasterizerStates;
  ShaderTable                    m_Shaders;
  BufferTable                    m_Buffers;
  TextureTable                   m_Textures;
  BufferViewTable                m_BufferViews;
  TextureViewTable               m_TextureViews;
  SamplerTable                   m_Samplers;
  InputLayoutTable               m_InputLayouts;
  QueryTable                     m_Queries;
  FenceTable                     m_Fences;
  RenderPassTable                m_RenderPasses;
  FramebufferTable               m_Framebuffers;
  BottomLevelASTable             m_BottomLevelAccelerationStructures;
  TopLevelASTable                m_TopLevelAccelerationStructures;
  PipelineStateTable             m_PipelineStates;
  PipelineResourceSignatureTable m_PipelineResourceSignatures;

  // Deduplication Contexts: Hash tables used to prevent state object duplication.
  xiiHashTable<xiiUInt32, xiiGALBlendStateHandle, xiiHashHelper<xiiUInt32>, xiiLocalAllocatorWrapper>                m_BlendStateTable;
  xiiHashTable<xiiUInt32, xiiGALDepthStencilStateHandle, xiiHashHelper<xiiUInt32>, xiiLocalAllocatorWrapper>         m_DepthStencilStateTable;
  xiiHashTable<xiiUInt32, xiiGALRasterizerStateHandle, xiiHashHelper<xiiUInt32>, xiiLocalAllocatorWrapper>           m_RasterizerStateTable;
  xiiHashTable<xiiUInt32, xiiGALSamplerHandle, xiiHashHelper<xiiUInt32>, xiiLocalAllocatorWrapper>                   m_SamplerTable;
  xiiHashTable<xiiUInt32, xiiGALInputLayoutHandle, xiiHashHelper<xiiUInt32>, xiiLocalAllocatorWrapper>               m_InputLayoutTable;
  xiiHashTable<xiiUInt32, xiiGALPipelineStateHandle, xiiHashHelper<xiiUInt32>, xiiLocalAllocatorWrapper>             m_PipelineStateTable;
  xiiHashTable<xiiUInt32, xiiGALPipelineResourceSignatureHandle, xiiHashHelper<xiiUInt32>, xiiLocalAllocatorWrapper> m_PipelineResourceSignatureTable;

  struct DestroyedObject
  {
    XII_DECLARE_POD_TYPE();

    xiiUInt32 m_uiType;
    xiiUInt32 m_uiHandle;
  };

  xiiDynamicArray<DestroyedObject, xiiLocalAllocatorWrapper> m_DestroyedObjects;

  xiiGALGraphicsDeviceAdapterDescription m_AdapterDescription;

  // Deactivate Doxygen document generation for the following block. (API abstraction only)
  /// \cond

  // These functions need to be implemented by a graphics API abstraction.
protected:
  friend class xiiMemoryUtils;

  virtual xiiResult InitializePlatform() = 0;
  virtual xiiResult ShutdownPlatform()   = 0;

  virtual xiiResult CreateCommandQueuesPlatform() = 0;

  virtual void BeginPipelinePlatform(xiiStringView sName, xiiGALSwapChain* pSwapChain) = 0;
  virtual void EndPipelinePlatform(xiiGALSwapChain* pSwapChain)                        = 0;

  virtual void BeginFramePlatform(const xiiUInt64 uiRenderFrame = 0U) = 0;
  virtual void EndFramePlatform()                                     = 0;

  virtual xiiGALSwapChain* CreateSwapChainPlatform(const xiiGALSwapChainCreationDescription& description) = 0;
  virtual void             DestroySwapChainPlatform(xiiGALSwapChain* pSwapChain)                          = 0;

  virtual xiiGALCommandQueue* CreateCommandQueuePlatform(const xiiGALCommandQueueCreationDescription& description) = 0;
  virtual void                DestroyCommandQueuePlatform(xiiGALCommandQueue* pCommandQueue)                       = 0;

  virtual xiiGALBlendState* CreateBlendStatePlatform(const xiiGALBlendStateCreationDescription& description) = 0;
  virtual void              DestroyBlendStatePlatform(xiiGALBlendState* pBlendState)                         = 0;

  virtual xiiGALDepthStencilState* CreateDepthStencilStatePlatform(const xiiGALDepthStencilStateCreationDescription& description) = 0;
  virtual void                     DestroyDepthStencilStatePlatform(xiiGALDepthStencilState* pDepthStencilState)                  = 0;

  virtual xiiGALRasterizerState* CreateRasterizerStatePlatform(const xiiGALRasterizerStateCreationDescription& description) = 0;
  virtual void                   DestroyRasterizerStatePlatform(xiiGALRasterizerState* pRasterizerState)                    = 0;

  virtual xiiGALShader* CreateShaderPlatform(const xiiGALShaderCreationDescription& description) = 0;
  virtual void          DestroyShaderPlatform(xiiGALShader* pShader)                             = 0;

  virtual xiiGALBuffer* CreateBufferPlatform(const xiiGALBufferCreationDescription& description, const xiiGALBufferData* pInitialData = nullptr) = 0;
  virtual void          DestroyBufferPlatform(xiiGALBuffer* pBuffer)                                                                             = 0;

  virtual xiiGALBufferView* CreateBufferViewPlatform(xiiGALBuffer* pBuffer, const xiiGALBufferViewCreationDescription& description) = 0;
  virtual void              DestroyBufferViewPlatform(xiiGALBufferView* pBufferView)                                                = 0;

  virtual xiiGALTexture* CreateTexturePlatform(const xiiGALTextureCreationDescription& description, const xiiGALTextureData* pInitialData = nullptr) = 0;
  virtual void           DestroyTexturePlatform(xiiGALTexture* pTexture)                                                                             = 0;

  virtual xiiGALTextureView* CreateTextureViewPlatform(xiiGALTexture* pTexture, const xiiGALTextureViewCreationDescription& description) = 0;
  virtual void               DestroyTextureViewPlatform(xiiGALTextureView* pTextureView)                                                 = 0;

  virtual xiiGALSampler* CreateSamplerPlatform(const xiiGALSamplerCreationDescription& description) = 0;
  virtual void           DestroySamplerPlatform(xiiGALSampler* pSampler)                            = 0;

  virtual xiiGALInputLayout* CreateInputLayoutPlatform(const xiiGALInputLayoutCreationDescription& description) = 0;
  virtual void               DestroyInputLayoutPlatform(xiiGALInputLayout* pInputLayout)                        = 0;

  virtual xiiGALQuery* CreateQueryPlatform(const xiiGALQueryCreationDescription& description) = 0;
  virtual void         DestroyQueryPlatform(xiiGALQuery* pQuery)                              = 0;

  virtual xiiGALFence* CreateFencePlatform(const xiiGALFenceCreationDescription& description) = 0;
  virtual void         DestroyFencePlatform(xiiGALFence* pFence)                              = 0;

  virtual xiiGALRenderPass* CreateRenderPassPlatform(const xiiGALRenderPassCreationDescription& description) = 0;
  virtual void              DestroyRenderPassPlatform(xiiGALRenderPass* pRenderPass)                         = 0;

  virtual xiiGALFramebuffer* CreateFramebufferPlatform(const xiiGALFramebufferCreationDescription& description) = 0;
  virtual void               DestroyFramebufferPlatform(xiiGALFramebuffer* pFramebuffer)                        = 0;

  virtual xiiGALBottomLevelAS* CreateBottomLevelASPlatform(const xiiGALBottomLevelASCreationDescription& description) = 0;
  virtual void                 DestroyBottomLevelASPlatform(xiiGALBottomLevelAS* pBottomLevelAS)                      = 0;

  virtual xiiGALTopLevelAS* CreateTopLevelASPlatform(const xiiGALTopLevelASCreationDescription& description) = 0;
  virtual void              DestroyTopLevelASPlatform(xiiGALTopLevelAS* pTopLevelAS)                         = 0;

  virtual xiiGALPipelineResourceSignature* CreatePipelineResourceSignaturePlatform(const xiiGALPipelineResourceSignatureCreationDescription& description) = 0;
  virtual void                             DestroyPipelineResourceSignaturePlatform(xiiGALPipelineResourceSignature* pPipelineResourceSignature)          = 0;

  virtual xiiGALPipelineState* CreatePipelineStatePlatform(const xiiGALPipelineStateCreationDescription& description) = 0;
  virtual void                 DestroyPipelineStatePlatform(xiiGALPipelineState* pPipelineState)                      = 0;

  virtual void WaitIdlePlatform() = 0;

  virtual void FillCapabilitiesPlatform() = 0;

  /// \endcond

protected:
  xiiGALTextureHandle FinalizeTextureInternal(const xiiGALTextureCreationDescription& description, xiiGALTexture* pTexture);
  xiiGALBufferHandle  FinalizeBufferInternal(const xiiGALBufferCreationDescription& description, xiiGALBuffer* pBuffer);

private:
  static xiiGALDevice* s_pDefaultDevice;

private:
  bool m_bBeginFrameCalled    = false;
  bool m_bBeginPipelineCalled = false;
};

#include <GraphicsFoundation/Device/Implementation/Device_inl.h>
