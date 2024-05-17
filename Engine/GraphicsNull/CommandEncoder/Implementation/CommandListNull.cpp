#include <GraphicsNull/GraphicsNullPCH.h>

#include <GraphicsNull/CommandEncoder/CommandListNull.h>
#include <GraphicsNull/Device/DeviceNull.h>

xiiGALCommandListNull::xiiGALCommandListNull(xiiGALDeviceNull* pDeviceNull, const xiiGALCommandListCreationDescription& creationDescription) :
  xiiGALCommandList(pDeviceNull, creationDescription)
{
}

xiiGALCommandListNull::~xiiGALCommandListNull() = default;

void xiiGALCommandListNull::BeginPlatform()
{
}

void xiiGALCommandListNull::EndPlatform()
{
}

void xiiGALCommandListNull::ResetPlatform()
{
}

void xiiGALCommandListNull::SetPipelineStatePlatform(xiiGALPipelineState* pPipelineState)
{
}

void xiiGALCommandListNull::SetStencilRefPlatform(xiiUInt32 uiStencilRef)
{
}

void xiiGALCommandListNull::SetBlendFactorPlatform(const xiiColor& blendFactor)
{
}

void xiiGALCommandListNull::SetViewportsPlatform(xiiArrayPtr<xiiGALViewport> pViewports, xiiUInt32 uiRenderTargetWidth, xiiUInt32 uiRenderTargetHeight)
{
}

void xiiGALCommandListNull::SetScissorRectsPlatform(xiiArrayPtr<xiiRectU32> pRects, xiiUInt32 uiRenderTargetWidth, xiiUInt32 uiRenderTargetHeight)
{
}

void xiiGALCommandListNull::SetIndexBufferPlatform(xiiGALBuffer* pIndexBuffer, xiiUInt64 uiByteOffset)
{
}

void xiiGALCommandListNull::SetVertexBuffersPlatform(xiiUInt32 uiStartSlot, xiiArrayPtr<xiiGALBuffer*> pVertexBuffers, xiiArrayPtr<xiiUInt64> pByteOffsets, xiiBitflags<xiiGALSetVertexBufferFlags> flags)
{
}

void xiiGALCommandListNull::ClearRenderTargetViewPlatform(xiiGALTextureView* pRenderTargetView, const xiiColor& clearColor)
{
}

void xiiGALCommandListNull::ClearDepthStencilViewPlatform(xiiGALTextureView* pDepthStencilView, bool bClearDepth, bool bClearStencil, float fDepthClear, xiiUInt8 uiStencilClear)
{
}

void xiiGALCommandListNull::BeginRenderPassPlatform(xiiGALRenderPass* pRenderPass, xiiGALFramebuffer* pFramebuffer, xiiArrayPtr<const xiiGALOptimizedClearValue> pOptimizedClearValues)
{
}

void xiiGALCommandListNull::NextSubpassPlatform()
{
}

void xiiGALCommandListNull::EndRenderPassPlatform()
{
}

xiiResult xiiGALCommandListNull::DrawPlatform(xiiUInt32 uiVertexCount, xiiUInt32 uiStartVertex)
{
  return XII_SUCCESS;
}

xiiResult xiiGALCommandListNull::DrawIndexedPlatform(xiiUInt32 uiIndexCount, xiiUInt32 uiStartIndex, xiiUInt32 uiBaseVertex)
{
  return XII_SUCCESS;
}

xiiResult xiiGALCommandListNull::DrawIndexedInstancedPlatform(xiiUInt32 uiIndexCountPerInstance, xiiUInt32 uiInstanceCount, xiiUInt32 uiStartIndex, xiiUInt32 uiBaseVertex, xiiUInt32 uiFirstInstance)
{
  return XII_SUCCESS;
}

xiiResult xiiGALCommandListNull::DrawIndexedInstancedIndirectPlatform(xiiGALBuffer* pIndirectArgumentBuffer, xiiUInt32 uiArgumentOffsetInBytes)
{
  return XII_SUCCESS;
}

xiiResult xiiGALCommandListNull::DrawInstancedPlatform(xiiUInt32 uiVertexCountPerInstance, xiiUInt32 uiInstanceCount, xiiUInt32 uiStartVertex, xiiUInt32 uiFirstInstance)
{
  return XII_SUCCESS;
}

xiiResult xiiGALCommandListNull::DrawInstancedIndirectPlatform(xiiGALBuffer* pIndirectArgumentBuffer, xiiUInt32 uiArgumentOffsetInBytes)
{
  return XII_SUCCESS;
}

xiiResult xiiGALCommandListNull::DrawMeshPlatform(xiiUInt32 uiThreadGroupCountX, xiiUInt32 uiThreadGroupCountY, xiiUInt32 uiThreadGroupCountZ)
{
  return XII_SUCCESS;
}

xiiResult xiiGALCommandListNull::DispatchPlatform(xiiUInt32 uiThreadGroupCountX, xiiUInt32 uiThreadGroupCountY, xiiUInt32 uiThreadGroupCountZ)
{
  return XII_SUCCESS;
}

xiiResult xiiGALCommandListNull::DispatchIndirectPlatform(xiiGALBuffer* pIndirectArgumentBuffer, xiiUInt32 uiArgumentOffsetInBytes)
{
  return XII_SUCCESS;
}

void xiiGALCommandListNull::BeginQueryPlatform(xiiGALQuery* pQuery)
{
}

void xiiGALCommandListNull::EndQueryPlatform(xiiGALQuery* pQuery)
{
}

void xiiGALCommandListNull::UpdateBufferPlatform(xiiGALBuffer* pBuffer, xiiUInt32 uiDestinationOffset, xiiArrayPtr<const xiiUInt8> pSourceData)
{
}

void xiiGALCommandListNull::UpdateBufferExtendedPlatform(xiiGALBuffer* pBuffer, xiiUInt32 uiDestinationOffset, xiiArrayPtr<const xiiUInt8> pSourceData, xiiBitflags<xiiGALMapFlags> mapFlags, bool bCopyToTemporaryStorage)
{
}

void xiiGALCommandListNull::CopyBufferPlatform(xiiGALBuffer* pSourceBuffer, xiiGALBuffer* pDestinationBuffer)
{
}

void xiiGALCommandListNull::CopyBufferRegionPlatform(xiiGALBuffer* pSourceBuffer, xiiUInt64 uiSourceOffset, xiiGALBuffer* pDestinationBuffer, xiiUInt64 uiDestinationOffset, xiiUInt64 uiSize)
{
}

xiiResult xiiGALCommandListNull::MapBufferPlatform(xiiGALBuffer* pBuffer, xiiEnum<xiiGALMapType> mapType, xiiBitflags<xiiGALMapFlags> mapFlags, void*& pMappedData)
{
  return XII_SUCCESS;
}

xiiResult xiiGALCommandListNull::UnmapBufferPlatform(xiiGALBuffer* pBuffer, xiiEnum<xiiGALMapType> mapType)
{
  return XII_SUCCESS;
}

void xiiGALCommandListNull::UpdateTexturePlatform(xiiGALTexture* pTexture, const xiiGALTextureMipLevelData& textureMiplevelData, const xiiBoundingBoxU32& textureBox, const xiiGALTextureSubResourceData& subresourceData)
{
}

void xiiGALCommandListNull::UpdateTextureExtendedPlatform(xiiGALTexture* pTexture, const xiiGALTextureMipLevelData& textureMiplevelData, const xiiBoundingBoxU32& textureBox, const xiiGALTextureSubResourceData& subresourceData)
{
}

void xiiGALCommandListNull::CopyTexturePlatform(xiiGALTexture* pSourceTexture, xiiGALTexture* pDestinationTexture)
{
}

void xiiGALCommandListNull::CopyTextureRegionPlatform(xiiGALTexture* pSourceTexture, const xiiGALTextureMipLevelData& sourceMipLevelData, const xiiBoundingBoxU32& box, xiiGALTexture* pDestinationTexture, const xiiGALTextureMipLevelData& destinationMipLevelData, const xiiVec3U32& vDestinationPoint)
{
}

void xiiGALCommandListNull::ResolveTextureSubResourcePlatform(xiiGALTexture* pSourceTexture, const xiiGALTextureMipLevelData& sourceMipLevelData, xiiGALTexture* pDestinationTexture, const xiiGALTextureMipLevelData& destinationMipLevelData)
{
}

void xiiGALCommandListNull::GenerateMipsPlatform(xiiGALTextureView* pTextureView)
{
}

xiiResult xiiGALCommandListNull::MapTextureSubresourcePlatform(xiiGALTexture* pTexture, xiiGALTextureMipLevelData textureMipLevelData, xiiEnum<xiiGALMapType> mapType, xiiBitflags<xiiGALMapFlags> mapFlags, xiiBoundingBoxU32* pTextureBox, xiiGALMappedTextureSubresource& mappedData)
{
  return XII_SUCCESS;
}

xiiResult xiiGALCommandListNull::UnmapTextureSubresourcePlatform(xiiGALTexture* pTexture, xiiGALTextureMipLevelData textureMipLevelData)
{
  return XII_SUCCESS;
}

void xiiGALCommandListNull::BeginDebugGroupPlatform(xiiStringView sName, const xiiColor& color)
{
}

void xiiGALCommandListNull::EndDebugGroupPlatform()
{
}

void xiiGALCommandListNull::InsertDebugLabelPlatform(xiiStringView sName, const xiiColor& color)
{
}

void xiiGALCommandListNull::FlushPlatform()
{
}

void xiiGALCommandListNull::InvalidateStatePlatform()
{
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_CommandEncoder_Implementation_CommandListNull);
