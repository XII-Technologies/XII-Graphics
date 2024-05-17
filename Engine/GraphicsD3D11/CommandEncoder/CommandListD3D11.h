#pragma once

#include <GraphicsD3D11/GraphicsD3D11DLL.h>

#include <GraphicsFoundation/CommandEncoder/CommandList.h>

#include <GraphicsD3D11/Resources/DisjointQueryPool.h>

struct ID3D11DeviceContext;
struct ID3D11CommandList;

class XII_GRAPHICSD3D11_DLL xiiGALCommandListD3D11 final : public xiiGALCommandList
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALCommandListD3D11, xiiGALCommandList);

public:
  ID3D11CommandList*    GetD3D11CommandList() const;
  ID3D11DeviceContext1* GetD3D11DeferredContext() const;

protected:
  virtual void BeginPlatform() override final;
  virtual void EndPlatform() override final;
  virtual void ResetPlatform() override final;

  virtual void SetPipelineStatePlatform(xiiGALPipelineState* pPipelineState) override final;

  virtual void SetStencilRefPlatform(xiiUInt32 uiStencilRef) override final;
  virtual void SetBlendFactorPlatform(const xiiColor& blendFactor) override final;

  virtual void SetViewportsPlatform(xiiArrayPtr<xiiGALViewport> pViewports, xiiUInt32 uiRenderTargetWidth, xiiUInt32 uiRenderTargetHeight) override final;
  virtual void SetScissorRectsPlatform(xiiArrayPtr<xiiRectU32> pRects, xiiUInt32 uiRenderTargetWidth, xiiUInt32 uiRenderTargetHeight) override final;

  virtual void SetIndexBufferPlatform(xiiGALBuffer* pIndexBuffer, xiiUInt64 uiByteOffset) override final;
  virtual void SetVertexBuffersPlatform(xiiUInt32 uiStartSlot, xiiArrayPtr<xiiGALBuffer*> pVertexBuffers, xiiArrayPtr<xiiUInt64> pByteOffsets, xiiBitflags<xiiGALSetVertexBufferFlags> flags) override final;

  virtual void ClearRenderTargetViewPlatform(xiiGALTextureView* pRenderTargetView, const xiiColor& clearColor) override final;
  virtual void ClearDepthStencilViewPlatform(xiiGALTextureView* pDepthStencilView, bool bClearDepth, bool bClearStencil, float fDepthClear, xiiUInt8 uiStencilClear) override final;

  virtual void BeginRenderPassPlatform(xiiGALRenderPass* pRenderPass, xiiGALFramebuffer* pFramebuffer, xiiArrayPtr<const xiiGALOptimizedClearValue> pOptimizedClearValues) override final;
  virtual void NextSubpassPlatform() override final;
  virtual void EndRenderPassPlatform() override final;

  virtual xiiResult DrawPlatform(xiiUInt32 uiVertexCount, xiiUInt32 uiStartVertex) override final;
  virtual xiiResult DrawIndexedPlatform(xiiUInt32 uiIndexCount, xiiUInt32 uiStartIndex, xiiUInt32 uiBaseVertex) override final;
  virtual xiiResult DrawIndexedInstancedPlatform(xiiUInt32 uiIndexCountPerInstance, xiiUInt32 uiInstanceCount, xiiUInt32 uiStartIndex, xiiUInt32 uiBaseVertex, xiiUInt32 uiFirstInstance) override final;
  virtual xiiResult DrawIndexedInstancedIndirectPlatform(xiiGALBuffer* pIndirectArgumentBuffer, xiiUInt32 uiArgumentOffsetInBytes) override final;
  virtual xiiResult DrawInstancedPlatform(xiiUInt32 uiVertexCountPerInstance, xiiUInt32 uiInstanceCount, xiiUInt32 uiStartVertex, xiiUInt32 uiFirstInstance) override final;
  virtual xiiResult DrawInstancedIndirectPlatform(xiiGALBuffer* pIndirectArgumentBuffer, xiiUInt32 uiArgumentOffsetInBytes) override final;
  virtual xiiResult DrawMeshPlatform(xiiUInt32 uiThreadGroupCountX, xiiUInt32 uiThreadGroupCountY, xiiUInt32 uiThreadGroupCountZ) override final;

  virtual xiiResult DispatchPlatform(xiiUInt32 uiThreadGroupCountX, xiiUInt32 uiThreadGroupCountY, xiiUInt32 uiThreadGroupCountZ) override final;
  virtual xiiResult DispatchIndirectPlatform(xiiGALBuffer* pIndirectArgumentBuffer, xiiUInt32 uiArgumentOffsetInBytes) override final;

  virtual void BeginQueryPlatform(xiiGALQuery* pQuery) override final;
  virtual void EndQueryPlatform(xiiGALQuery* pQuery) override final;

  virtual void      UpdateBufferPlatform(xiiGALBuffer* pBuffer, xiiUInt32 uiDestinationOffset, xiiArrayPtr<const xiiUInt8> pSourceData) override final;
  virtual void      UpdateBufferExtendedPlatform(xiiGALBuffer* pBuffer, xiiUInt32 uiDestinationOffset, xiiArrayPtr<const xiiUInt8> pSourceData, xiiBitflags<xiiGALMapFlags> mapFlags, bool bCopyToTemporaryStorage) override final;
  virtual void      CopyBufferPlatform(xiiGALBuffer* pSourceBuffer, xiiGALBuffer* pDestinationBuffer) override final;
  virtual void      CopyBufferRegionPlatform(xiiGALBuffer* pSourceBuffer, xiiUInt64 uiSourceOffset, xiiGALBuffer* pDestinationBuffer, xiiUInt64 uiDestinationOffset, xiiUInt64 uiSize) override final;
  virtual xiiResult MapBufferPlatform(xiiGALBuffer* pBuffer, xiiEnum<xiiGALMapType> mapType, xiiBitflags<xiiGALMapFlags> mapFlags, void*& pMappedData) override final;
  virtual xiiResult UnmapBufferPlatform(xiiGALBuffer* pBuffer, xiiEnum<xiiGALMapType> mapType) override final;

  virtual void      UpdateTexturePlatform(xiiGALTexture* pTexture, const xiiGALTextureMipLevelData& textureMiplevelData, const xiiBoundingBoxU32& textureBox, const xiiGALTextureSubResourceData& subresourceData) override final;
  virtual void      UpdateTextureExtendedPlatform(xiiGALTexture* pTexture, const xiiGALTextureMipLevelData& textureMiplevelData, const xiiBoundingBoxU32& textureBox, const xiiGALTextureSubResourceData& subresourceData) override final;
  virtual void      CopyTexturePlatform(xiiGALTexture* pSourceTexture, xiiGALTexture* pDestinationTexture) override final;
  virtual void      CopyTextureRegionPlatform(xiiGALTexture* pSourceTexture, const xiiGALTextureMipLevelData& sourceMipLevelData, const xiiBoundingBoxU32& box, xiiGALTexture* pDestinationTexture, const xiiGALTextureMipLevelData& destinationMipLevelData, const xiiVec3U32& vDestinationPoint) override final;
  virtual void      ResolveTextureSubResourcePlatform(xiiGALTexture* pSourceTexture, const xiiGALTextureMipLevelData& sourceMipLevelData, xiiGALTexture* pDestinationTexture, const xiiGALTextureMipLevelData& destinationMipLevelData) override final;
  virtual void      GenerateMipsPlatform(xiiGALTextureView* pTextureView) override final;
  virtual xiiResult MapTextureSubresourcePlatform(xiiGALTexture* pTexture, xiiGALTextureMipLevelData textureMipLevelData, xiiEnum<xiiGALMapType> mapType, xiiBitflags<xiiGALMapFlags> mapFlags, xiiBoundingBoxU32* pTextureBox, xiiGALMappedTextureSubresource& mappedData) override final;
  virtual xiiResult UnmapTextureSubresourcePlatform(xiiGALTexture* pTexture, xiiGALTextureMipLevelData textureMipLevelData) override final;

  virtual void BeginDebugGroupPlatform(xiiStringView sName, const xiiColor& color) override final;
  virtual void EndDebugGroupPlatform() override final;
  virtual void InsertDebugLabelPlatform(xiiStringView sName, const xiiColor& color) override final;

  virtual void FlushPlatform() override final;

  virtual void InvalidateStatePlatform() override final;
  void         InvalidateResources();

  void CommitRenderTargets();
  void ResetRenderTargets();

  void ReleaseInternalCommandList();

protected:
  friend class xiiGALCommandQueueD3D11;
  friend class xiiGALDeviceD3D11;
  friend class xiiMemoryUtils;

  xiiGALCommandListD3D11(xiiGALDeviceD3D11* pDeviceD3D11, xiiGALCommandQueueD3D11* pCommandQueueD3D11, const xiiGALCommandListCreationDescription& creationDescription);

  virtual ~xiiGALCommandListD3D11();

  virtual void SetDebugNamePlatform(xiiStringView sName) override final;

protected:
  xiiSharedPtr<xiiDisjointQueryPool::DisjointQueryWrapper> BeginDisjointQuery();

  xiiResult FlushDeferredStateChanges();

protected:
  xiiGALCommandQueueD3D11* m_pCommandQueueD3D11 = nullptr;

  ID3D11DeviceContext1* m_pCommandList          = nullptr;
  ID3D11CommandList*    m_pSubmittedCommandList = nullptr;

  xiiGALPipelineStateD3D11* m_pPipelineState = nullptr;

  ID3D11Buffer*         m_pCommittedVertexBuffers[XII_GAL_MAX_VERTEX_BUFFER_COUNT]      = {};
  xiiUInt32             m_CommittedVertexBufferStrides[XII_GAL_MAX_VERTEX_BUFFER_COUNT] = {};
  xiiUInt32             m_CommittedVertexBufferOffsets[XII_GAL_MAX_VERTEX_BUFFER_COUNT] = {};
  xiiGAL::ModifiedRange m_CommittedVertexBuffersRange;

  ID3D11InputLayout* m_pCommittedInputLayout = nullptr;

  ID3D11Buffer* m_pCommittedIndexBuffer           = nullptr;
  DXGI_FORMAT   m_CommittedIndexBufferFormat      = DXGI_FORMAT_UNKNOWN;
  xiiUInt32     m_uiCommittedIndexDataStartOffset = 0;

  D3D11_PRIMITIVE_TOPOLOGY m_CommittedPrimitiveTopology  = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
  xiiColor                 m_CommittedBlendFactors       = xiiColor::White;
  xiiUInt32                m_uiCommittedBlendSampleMask  = 0xFFFFFFFFU;
  xiiUInt32                m_uiCommittedStencilReference = 0xFFU;

  ID3D11RenderTargetView* m_pCommittedRenderTargets[XII_GAL_MAX_RENDERTARGET_COUNT] = {};
  ID3D11DepthStencilView* m_pCommittedDepthStencilTarget                            = nullptr;
  xiiUInt32               m_uiBoundRenderTargetCount                                = 0U;

  xiiUInt32                                                                 m_uiSubpassIndex = 0U;
  xiiGALRenderPass*                                                         m_pRenderPass    = nullptr;
  xiiGALFramebuffer*                                                        m_pFramebuffer   = nullptr;
  xiiStaticArray<xiiGALOptimizedClearValue, XII_GAL_MAX_RENDERTARGET_COUNT> m_AttachmentClearValues;

  xiiMap<xiiGALBufferD3D11*, ID3D11DeviceContext*>  m_MappedBuffers;
  xiiMap<xiiGALTextureD3D11*, ID3D11DeviceContext*> m_MappedTextureSubresources;

  xiiDisjointQueryPool                                     m_DisjointQueryPool;
  xiiSharedPtr<xiiDisjointQueryPool::DisjointQueryWrapper> m_pActiveDisjointQuery;
};

#include <GraphicsD3D11/CommandEncoder/Implementation/CommandListD3D11_inl.h>
