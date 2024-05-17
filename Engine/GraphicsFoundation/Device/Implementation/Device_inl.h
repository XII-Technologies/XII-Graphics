
/// \brief Used to guard xiiGALDevice functions from multi-threaded access and to verify that executing them on non-main-threads is allowed.
#define XII_GAL_DEVICE_LOCK_AND_CHECK() \
  XII_LOCK(m_Mutex);                    \
  VerifyMultithreadedAccess()

XII_ALWAYS_INLINE xiiMutex& xiiGALDevice::GetMutex() const
{
  return m_Mutex;
}

template <typename IdTableType, typename ReturnType>
XII_ALWAYS_INLINE ReturnType* xiiGALDevice::Get(typename IdTableType::TypeOfId hHandle, const IdTableType& IdTable) const
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  ReturnType* pObject = nullptr;
  IdTable.TryGetValue(hHandle, pObject);
  return pObject;
}

XII_ALWAYS_INLINE const xiiGALDeviceCreationDescription& xiiGALDevice::GetDescription() const
{
  return m_Description;
}

inline xiiGALSwapChain* xiiGALDevice::GetSwapChain(xiiGALSwapChainHandle hSwapChain) const
{
  return Get<SwapChainTable, xiiGALSwapChain>(hSwapChain, m_SwapChains);
}

inline xiiGALCommandQueue* xiiGALDevice::GetCommandQueue(xiiGALCommandQueueHandle hCommandQueue) const
{
  return Get<CommandQueueTable, xiiGALCommandQueue>(hCommandQueue, m_CommandQueues);
}

inline xiiGALBlendState* xiiGALDevice::GetBlendState(xiiGALBlendStateHandle hBlendState) const
{
  return Get<BlendStateTable, xiiGALBlendState>(hBlendState, m_BlendStates);
}

inline xiiGALDepthStencilState* xiiGALDevice::GetDepthStencilState(xiiGALDepthStencilStateHandle hDepthStencilState) const
{
  return Get<DepthStencilStateTable, xiiGALDepthStencilState>(hDepthStencilState, m_DepthStencilStates);
}

inline xiiGALRasterizerState* xiiGALDevice::GetRasterizerState(xiiGALRasterizerStateHandle hRasterizerState) const
{
  return Get<RasterizerStateTable, xiiGALRasterizerState>(hRasterizerState, m_RasterizerStates);
}

inline xiiGALShader* xiiGALDevice::GetShader(xiiGALShaderHandle hShader) const
{
  return Get<ShaderTable, xiiGALShader>(hShader, m_Shaders);
}

inline xiiGALBuffer* xiiGALDevice::GetBuffer(xiiGALBufferHandle hBuffer) const
{
  return Get<BufferTable, xiiGALBuffer>(hBuffer, m_Buffers);
}

inline xiiGALTexture* xiiGALDevice::GetTexture(xiiGALTextureHandle hTexture) const
{
  return Get<TextureTable, xiiGALTexture>(hTexture, m_Textures);
}

inline xiiGALBufferView* xiiGALDevice::GetBufferView(xiiGALBufferViewHandle hBufferView) const
{
  return Get<BufferViewTable, xiiGALBufferView>(hBufferView, m_BufferViews);
}

inline xiiGALTextureView* xiiGALDevice::GetTextureView(xiiGALTextureViewHandle hTextureView) const
{
  return Get<TextureViewTable, xiiGALTextureView>(hTextureView, m_TextureViews);
}

inline xiiGALSampler* xiiGALDevice::GetSampler(xiiGALSamplerHandle hSampler) const
{
  return Get<SamplerTable, xiiGALSampler>(hSampler, m_Samplers);
}

inline xiiGALInputLayout* xiiGALDevice::GetInputLayout(xiiGALInputLayoutHandle hInputLayout) const
{
  return Get<InputLayoutTable, xiiGALInputLayout>(hInputLayout, m_InputLayouts);
}

inline xiiGALQuery* xiiGALDevice::GetQuery(xiiGALQueryHandle hQuery) const
{
  return Get<QueryTable, xiiGALQuery>(hQuery, m_Queries);
}

inline xiiGALFence* xiiGALDevice::GetFence(xiiGALFenceHandle hFence) const
{
  return Get<FenceTable, xiiGALFence>(hFence, m_Fences);
}

inline xiiGALRenderPass* xiiGALDevice::GetRenderPass(xiiGALRenderPassHandle hRenderPass) const
{
  return Get<RenderPassTable, xiiGALRenderPass>(hRenderPass, m_RenderPasses);
}

inline xiiGALFramebuffer* xiiGALDevice::GetFramebuffer(xiiGALFramebufferHandle hFramebuffer) const
{
  return Get<FramebufferTable, xiiGALFramebuffer>(hFramebuffer, m_Framebuffers);
}

inline xiiGALBottomLevelAS* xiiGALDevice::GetBottomLevelAS(xiiGALBottomLevelASHandle hBottomLevelAS) const
{
  return Get<BottomLevelASTable, xiiGALBottomLevelAS>(hBottomLevelAS, m_BottomLevelAccelerationStructures);
}

inline xiiGALTopLevelAS* xiiGALDevice::GetTopLevelAS(xiiGALTopLevelASHandle hTopLevelAS) const
{
  return Get<TopLevelASTable, xiiGALTopLevelAS>(hTopLevelAS, m_TopLevelAccelerationStructures);
}

inline xiiGALPipelineResourceSignature* xiiGALDevice::GetPipelineResourceSignature(xiiGALPipelineResourceSignatureHandle hPipelineResourceSignature) const
{
  return Get<PipelineResourceSignatureTable, xiiGALPipelineResourceSignature>(hPipelineResourceSignature, m_PipelineResourceSignatures);
}

inline xiiGALPipelineState* xiiGALDevice::GetPipelineState(xiiGALPipelineStateHandle hPipelineState) const
{
  return Get<PipelineStateTable, xiiGALPipelineState>(hPipelineState, m_PipelineStates);
}

XII_ALWAYS_INLINE const xiiGALGraphicsDeviceAdapterDescription& xiiGALDevice::GetGraphicsDeviceAdapterProperties() const
{
  return m_AdapterDescription;
}

XII_ALWAYS_INLINE const xiiGALDeviceFeatures& xiiGALDevice::GetFeatures() const
{
  return m_AdapterDescription.m_Features;
}

XII_ALWAYS_INLINE xiiEnum<xiiGALGraphicsDeviceType> xiiGALDevice::GetGraphicsDeviceType() const
{
  return m_Description.m_GraphicsDeviceType;
}

// static
XII_ALWAYS_INLINE void xiiGALDevice::SetDefaultDevice(xiiGALDevice* pDefaultDevice)
{
  s_pDefaultDevice = pDefaultDevice;
}

// static
XII_ALWAYS_INLINE xiiGALDevice* xiiGALDevice::GetDefaultDevice()
{
  XII_ASSERT_DEBUG(s_pDefaultDevice != nullptr, "Default device not set.");

  return s_pDefaultDevice;
}

// static
XII_ALWAYS_INLINE bool xiiGALDevice::HasDefaultDevice()
{
  return s_pDefaultDevice != nullptr;
}

template <typename HandleType>
XII_FORCE_INLINE void xiiGALDevice::AddDestroyedObject(xiiUInt32 uiType, HandleType handle)
{
  auto& destroyedObject      = m_DestroyedObjects.ExpandAndGetRef();
  destroyedObject.m_uiType   = uiType;
  destroyedObject.m_uiHandle = handle.GetInternalID().m_Data;
}

template <typename HandleType>
void xiiGALDevice::ReviveDestroyedObject(xiiUInt32 uiType, HandleType handle)
{
  xiiUInt32 uiHandle = handle.GetInternalID().m_Data;

  for (xiiUInt32 i = 0; i < m_DestroyedObjects.GetCount(); ++i)
  {
    const auto& destroyedObject = m_DestroyedObjects[i];

    if (destroyedObject.m_uiType == uiType && destroyedObject.m_uiHandle == uiHandle)
    {
      m_DestroyedObjects.RemoveAtAndCopy(i);
      return;
    }
  }
}

XII_ALWAYS_INLINE void xiiGALDevice::VerifyMultithreadedAccess() const
{
#if XII_ENABLED(XII_COMPILE_FOR_DEVELOPMENT)
  XII_ASSERT_DEV(m_AdapterDescription.m_Features.m_MultithreadedResourceCreation == xiiGALDeviceFeatureState::Enabled || xiiThreadUtils::IsMainThread(),
                 "This device does not support multi-threaded resource creation, therefore this function can only be executed on the main thread.");
#endif
}
