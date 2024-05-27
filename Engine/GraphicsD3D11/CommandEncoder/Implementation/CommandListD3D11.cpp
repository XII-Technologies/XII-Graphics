#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <Foundation/Basics/Platform/Win/HResultUtils.h>
#include <Foundation/Memory/MemoryUtils.h>

#include <GraphicsFoundation/Utilities/GraphicsUtilities.h>

#include <GraphicsD3D11/CommandEncoder/CommandListD3D11.h>
#include <GraphicsD3D11/Device/DeviceD3D11.h>

#include <GraphicsD3D11/Resources/BottomLevelASD3D11.h>
#include <GraphicsD3D11/Resources/BufferD3D11.h>
#include <GraphicsD3D11/Resources/BufferViewD3D11.h>
#include <GraphicsD3D11/Resources/FenceD3D11.h>
#include <GraphicsD3D11/Resources/FramebufferD3D11.h>
#include <GraphicsD3D11/Resources/QueryD3D11.h>
#include <GraphicsD3D11/Resources/RenderPassD3D11.h>
#include <GraphicsD3D11/Resources/SamplerD3D11.h>
#include <GraphicsD3D11/Resources/TextureD3D11.h>
#include <GraphicsD3D11/Resources/TextureViewD3D11.h>
#include <GraphicsD3D11/Resources/TopLevelASD3D11.h>
#include <GraphicsD3D11/Shader/InputLayoutD3D11.h>
#include <GraphicsD3D11/Shader/ShaderD3D11.h>
#include <GraphicsD3D11/States/BlendStateD3D11.h>
#include <GraphicsD3D11/States/DepthStencilStateD3D11.h>
#include <GraphicsD3D11/States/PipelineResourceSignatureD3D11.h>
#include <GraphicsD3D11/States/PipelineStateD3D11.h>
#include <GraphicsD3D11/States/RasterizerStateD3D11.h>

#include <d3d11_1.h>

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALCommandListD3D11, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALCommandListD3D11::xiiGALCommandListD3D11(xiiGALDeviceD3D11* pDeviceD3D11, xiiGALCommandQueueD3D11* pCommandQueueD3D11, const xiiGALCommandListCreationDescription& creationDescription) :
  xiiGALCommandList(pDeviceD3D11, creationDescription), m_pCommandQueueD3D11(pCommandQueueD3D11)
{
  HRESULT hResult = pDeviceD3D11->GetD3D11Device()->CreateDeferredContext1(0U, &m_pCommandList);

  if (FAILED(hResult))
  {
    xiiLog::Error("Failed to create deferred context for recording commands: {}", xiiHRESULTtoString(hResult));
  }
}

xiiGALCommandListD3D11::~xiiGALCommandListD3D11()
{
  XII_GAL_D3D11_RELEASE(m_pSubmittedCommandList);
  XII_GAL_D3D11_RELEASE(m_pCommandList);
}

void xiiGALCommandListD3D11::SetDebugNamePlatform(xiiStringView sName)
{
  if (m_pCommandList != nullptr)
  {
    xiiStringBuilder sb;
    if (FAILED(m_pCommandList->SetPrivateData(WKPDID_D3DDebugObjectName, sName.GetElementCount(), sName.GetData(sb))))
    {
      xiiLog::Error("Failed to set the Direct3D11 deferred context debug name.");
    }
  }
}

void xiiGALCommandListD3D11::BeginPlatform()
{
  XII_GAL_D3D11_RELEASE(m_pSubmittedCommandList);

  m_RecordingState = RecordingState::Recording;
}

void xiiGALCommandListD3D11::EndPlatform()
{
  XII_ASSERT_DEV(m_pSubmittedCommandList == nullptr, "Submitted command list is not null.");

  XII_VERIFY(SUCCEEDED(m_pCommandList->FinishCommandList(0U, &m_pSubmittedCommandList)), "Failed to end command list.");

  m_RecordingState = RecordingState::Ended;
}

void xiiGALCommandListD3D11::ResetPlatform()
{
  XII_ASSERT_DEV(m_RecordingState == RecordingState::Ended, "Command list has not been ended by the GAL!");

  XII_GAL_D3D11_RELEASE(m_pSubmittedCommandList);

  m_RecordingState = RecordingState::Reset;

  InvalidateState();
}

void xiiGALCommandListD3D11::SetPipelineStatePlatform(xiiGALPipelineState* pPipelineState)
{
  auto pPipelineStateD3D11 = static_cast<xiiGALPipelineStateD3D11*>(pPipelineState);

  m_pPipelineState             = pPipelineStateD3D11;
  m_CommittedPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;

  if (m_pPipelineState != nullptr)
  {
    m_CommittedPrimitiveTopology = xiiD3D11TypeConversions::GetPrimitiveTopology(m_pPipelineState->GetDescription().m_GraphicsPipeline.m_PrimitiveTopology);
  }
}

void xiiGALCommandListD3D11::SetStencilRefPlatform(xiiUInt32 uiStencilRef)
{
  ID3D11DepthStencilState* pD3D11DepthStencilState = m_pPipelineState ? m_pPipelineState->GetD3D11DepthStencilState() : nullptr;

  m_pCommandList->OMSetDepthStencilState(pD3D11DepthStencilState, uiStencilRef);

  m_uiCommittedStencilReference = uiStencilRef;
}

void xiiGALCommandListD3D11::SetBlendFactorPlatform(const xiiColor& blendFactor)
{
  xiiUInt32         uiSampleMask     = 0xFFFFFFFFU;
  ID3D11BlendState* pD3D11BlendState = nullptr;

  if (m_pPipelineState != nullptr)
  {
    const auto& description = m_pPipelineState->GetDescription();

    if (description.IsAnyGraphicsPipeline())
    {
      uiSampleMask     = description.m_GraphicsPipeline.m_uiSampleMask;
      pD3D11BlendState = m_pPipelineState->GetD3D11BlendState();
    }
  }
  m_CommittedBlendFactors      = blendFactor;
  m_uiCommittedBlendSampleMask = uiSampleMask;
  m_pCommandList->OMSetBlendState(pD3D11BlendState, blendFactor.GetData(), uiSampleMask);
}

void xiiGALCommandListD3D11::SetViewportsPlatform(xiiArrayPtr<xiiGALViewport> pViewports, xiiUInt32 uiRenderTargetWidth, xiiUInt32 uiRenderTargetHeight)
{
  XII_CHECK_AT_COMPILETIME_MSG(XII_GAL_MAX_VIEWPORT_COUNT >= D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE, "The XII_GAL_MAX_VIEWPORT_COUNT must be greater than (or equal to) D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE.");

  XII_ASSERT_DEV(m_Viewports.GetCount() == pViewports.GetCount(), "Unexpected number of viewports.");

  D3D11_VIEWPORT d3d11Viewports[XII_GAL_MAX_VIEWPORT_COUNT];

  for (xiiUInt32 uiViewPortIndex = 0; uiViewPortIndex < pViewports.GetCount(); ++uiViewPortIndex)
  {
    d3d11Viewports[uiViewPortIndex].TopLeftX = pViewports[uiViewPortIndex].m_fTopLeftX;
    d3d11Viewports[uiViewPortIndex].TopLeftY = pViewports[uiViewPortIndex].m_fTopLeftY;
    d3d11Viewports[uiViewPortIndex].Width    = pViewports[uiViewPortIndex].m_fWidth;
    d3d11Viewports[uiViewPortIndex].Height   = pViewports[uiViewPortIndex].m_fHeight;
    d3d11Viewports[uiViewPortIndex].MinDepth = pViewports[uiViewPortIndex].m_fMinDepth;
    d3d11Viewports[uiViewPortIndex].MaxDepth = pViewports[uiViewPortIndex].m_fMaxDepth;
  }

  // All viewports must be set atomically as one operation.
  // Any viewports not defined by the call are disabled.
  m_pCommandList->RSSetViewports(pViewports.GetCount(), d3d11Viewports);
}

void xiiGALCommandListD3D11::SetScissorRectsPlatform(xiiArrayPtr<xiiRectU32> pRects, xiiUInt32 uiRenderTargetWidth, xiiUInt32 uiRenderTargetHeight)
{
  XII_CHECK_AT_COMPILETIME_MSG(XII_GAL_MAX_VIEWPORT_COUNT >= D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE, "The XII_GAL_MAX_VIEWPORT_COUNT must be greater than (or equal to) D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE.");

  XII_ASSERT_DEV(m_ScissorRects.GetCount() == pRects.GetCount(), "Unexpected number of scissor rects.");

  D3D11_RECT d3d11ScissorRects[XII_GAL_MAX_VIEWPORT_COUNT];

  for (xiiUInt32 uiScissorRectIndex = 0; uiScissorRectIndex < pRects.GetCount(); ++uiScissorRectIndex)
  {
    d3d11ScissorRects[uiScissorRectIndex].left   = pRects[uiScissorRectIndex].Left();
    d3d11ScissorRects[uiScissorRectIndex].top    = pRects[uiScissorRectIndex].Top();
    d3d11ScissorRects[uiScissorRectIndex].right  = pRects[uiScissorRectIndex].Right();
    d3d11ScissorRects[uiScissorRectIndex].bottom = pRects[uiScissorRectIndex].Bottom();
  }

  // All scissor rects must be set atomically as one operation.
  // Any scissor rects not defined by the call are disabled.
  m_pCommandList->RSSetScissorRects(pRects.GetCount(), d3d11ScissorRects);
}

void xiiGALCommandListD3D11::SetIndexBufferPlatform(xiiGALBuffer* pIndexBuffer, xiiUInt64 uiByteOffset)
{
  auto          pIndexBufferD3D11 = static_cast<xiiGALBufferD3D11*>(pIndexBuffer);
  ID3D11Buffer* pD3D11IndexBuffer = pIndexBufferD3D11 ? pIndexBufferD3D11->GetBuffer() : nullptr;

  if (pD3D11IndexBuffer != m_pCommittedIndexBuffer || m_uiCommittedIndexDataStartOffset != uiByteOffset)
  {
    m_pCommittedIndexBuffer           = pD3D11IndexBuffer;
    m_CommittedIndexBufferFormat      = DXGI_FORMAT_UNKNOWN;
    m_uiCommittedIndexDataStartOffset = static_cast<xiiUInt32>(uiByteOffset);

    if (pIndexBufferD3D11 != nullptr)
    {
      const auto& indexFormat = pIndexBufferD3D11->GetIndexFormat();

      DXGI_FORMAT d3d11IndexFormat = DXGI_FORMAT_UNKNOWN;
      if (indexFormat == xiiGALValueType::UInt32)
      {
        d3d11IndexFormat = DXGI_FORMAT_R32_UINT;
      }
      else if (indexFormat == xiiGALValueType::UInt16)
      {
        d3d11IndexFormat = DXGI_FORMAT_R16_UINT;
      }
      else
      {
        xiiLog::Error("Unsupported index format, only xiiGALValueType::UInt16 or xiiGALValueType::UInt32 are supported.");
        return;
      }
      m_CommittedIndexBufferFormat = d3d11IndexFormat;
    }
  }
}

void xiiGALCommandListD3D11::SetVertexBuffersPlatform(xiiUInt32 uiStartSlot, xiiArrayPtr<xiiGALBuffer*> pVertexBuffers, xiiArrayPtr<xiiUInt64> pByteOffsets, xiiBitflags<xiiGALSetVertexBufferFlags> flags)
{
  XII_ASSERT_DEV((uiStartSlot + pVertexBuffers.GetCount()) <= XII_GAL_MAX_VERTEX_BUFFER_COUNT, "The number of vertex buffers to set, exceeds the maximum amount.");

  if (flags.IsSet(xiiGALSetVertexBufferFlags::Reset))
  {
    m_CommittedVertexBuffersRange.Reset();

    // Reset only the buffer slots that are not being set.
    for (xiiUInt32 i = 0; i < uiStartSlot; ++i)
    {
      m_pCommittedVertexBuffers[i]      = nullptr;
      m_CommittedVertexBufferOffsets[i] = 0U;
      m_CommittedVertexBufferStrides[i] = 0U;
    }
    for (xiiUInt32 i = uiStartSlot + pVertexBuffers.GetCount(); i < XII_GAL_MAX_VERTEX_BUFFER_COUNT; ++i)
    {
      m_pCommittedVertexBuffers[i]      = nullptr;
      m_CommittedVertexBufferOffsets[i] = 0U;
      m_CommittedVertexBufferStrides[i] = 0U;
    }
  }

  for (xiiUInt32 i = uiStartSlot; i < pVertexBuffers.GetCount(); ++i)
  {
    auto pVertexBufferD3D11 = static_cast<xiiGALBufferD3D11*>(pVertexBuffers[i]);

    ID3D11Buffer* pD3D11VertexBuffer   = pVertexBufferD3D11 ? pVertexBufferD3D11->GetBuffer() : nullptr;
    xiiUInt32     uiVertexBufferOffset = pByteOffsets.IsEmpty() ? 0U : static_cast<xiiUInt32>(pByteOffsets[i]);
    xiiUInt32     uiVertexBufferStride = pVertexBufferD3D11 ? pVertexBufferD3D11->GetDescription().m_uiElementByteStride : 0U;

    if (m_pCommittedVertexBuffers[i] != pD3D11VertexBuffer || m_CommittedVertexBufferOffsets[i] != uiVertexBufferOffset || m_CommittedVertexBufferStrides[i] != uiVertexBufferStride)
    {
      m_pCommittedVertexBuffers[i]      = pD3D11VertexBuffer;
      m_CommittedVertexBufferOffsets[i] = uiVertexBufferOffset;
      m_CommittedVertexBufferStrides[i] = uiVertexBufferStride;

      m_CommittedVertexBuffersRange.SetToIncludeValue(i);
    }
  }
}

void xiiGALCommandListD3D11::ClearRenderTargetViewPlatform(xiiGALTextureView* pRenderTargetView, const xiiColor& clearColor)
{
  auto pRenderTargetViewD3D11 = static_cast<xiiGALTextureViewD3D11*>(pRenderTargetView);

  XII_ASSERT_DEV(pRenderTargetViewD3D11 != nullptr, "Invalid resource.");

  // The full extent of the resource view is always cleared. Viewport and scissor settings are not applied.
  m_pCommandList->ClearRenderTargetView(static_cast<ID3D11RenderTargetView*>(pRenderTargetViewD3D11->GetTextureView()), clearColor.GetData());
}

void xiiGALCommandListD3D11::ClearDepthStencilViewPlatform(xiiGALTextureView* pDepthStencilView, bool bClearDepth, bool bClearStencil, float fDepthClear, xiiUInt8 uiStencilClear)
{
  auto pDepthStencilViewD3D11 = static_cast<xiiGALTextureViewD3D11*>(pDepthStencilView);

  XII_ASSERT_DEV(pDepthStencilViewD3D11 != nullptr, "Invalid resource.");

  xiiUInt32 uiClearFlags = 0;
  if (bClearDepth)
    uiClearFlags |= D3D11_CLEAR_DEPTH;
  if (bClearStencil)
    uiClearFlags |= D3D11_CLEAR_STENCIL;

  // The full extent of the resource view is always cleared. Viewport and scissor settings are not applied.
  m_pCommandList->ClearDepthStencilView(static_cast<ID3D11DepthStencilView*>(pDepthStencilViewD3D11->GetTextureView()), uiClearFlags, fDepthClear, uiStencilClear);
}

void xiiGALCommandListD3D11::BeginRenderPassPlatform(xiiGALRenderPass* pRenderPass, xiiGALFramebuffer* pFramebuffer, xiiArrayPtr<const xiiGALOptimizedClearValue> pOptimizedClearValues)
{
  ResetRenderTargets();

  m_AttachmentClearValues.SetCountUninitialized(pOptimizedClearValues.GetCount());
  m_AttachmentClearValues = pOptimizedClearValues;

  // Set viewport to match frame buffer size.
  const auto&    framebufferDescription = pFramebuffer->GetDescription();
  xiiGALViewport viewport               = {.m_fTopLeftX = 0.0f, .m_fTopLeftY = 0.0f, .m_fWidth = (float)framebufferDescription.m_FramebufferSize.width, .m_fHeight = (float)framebufferDescription.m_FramebufferSize.height};
  SetViewports(xiiMakeArrayPtr(&viewport, 1U), framebufferDescription.m_FramebufferSize.width, framebufferDescription.m_FramebufferSize.height);

  m_pRenderPass  = static_cast<xiiGALRenderPassD3D11*>(pRenderPass);
  m_pFramebuffer = static_cast<xiiGALFramebufferD3D11*>(pFramebuffer);

  // Set the active render targes.
  CommitRenderTargets();

  m_pCommandQueueD3D11->AddSwapChainCommandListReference(this);
}

void xiiGALCommandListD3D11::NextSubpassPlatform()
{
  XII_ASSERT_NOT_IMPLEMENTED;
}

void xiiGALCommandListD3D11::EndRenderPassPlatform()
{
  m_pCommandQueueD3D11->RemoveSwapChainCommandListReference(this);

  ResetRenderTargets();
}

xiiResult xiiGALCommandListD3D11::DrawPlatform(xiiUInt32 uiVertexCount, xiiUInt32 uiStartVertex)
{
  XII_SUCCEED_OR_RETURN(FlushDeferredStateChanges());

  m_pCommandList->Draw(uiVertexCount, uiStartVertex);

  return XII_SUCCESS;
}

xiiResult xiiGALCommandListD3D11::DrawIndexedPlatform(xiiUInt32 uiIndexCount, xiiUInt32 uiStartIndex, xiiUInt32 uiBaseVertex)
{
  XII_SUCCEED_OR_RETURN(FlushDeferredStateChanges());

  m_pCommandList->DrawIndexed(uiIndexCount, uiStartIndex, uiBaseVertex);

  return XII_SUCCESS;
}

xiiResult xiiGALCommandListD3D11::DrawIndexedInstancedPlatform(xiiUInt32 uiIndexCountPerInstance, xiiUInt32 uiInstanceCount, xiiUInt32 uiStartIndex, xiiUInt32 uiBaseVertex, xiiUInt32 uiFirstInstance)
{
  XII_SUCCEED_OR_RETURN(FlushDeferredStateChanges());

  m_pCommandList->DrawIndexedInstanced(uiIndexCountPerInstance, uiInstanceCount, uiStartIndex, uiBaseVertex, uiFirstInstance);

  return XII_SUCCESS;
}

xiiResult xiiGALCommandListD3D11::DrawIndexedInstancedIndirectPlatform(xiiGALBuffer* pIndirectArgumentBuffer, xiiUInt32 uiArgumentOffsetInBytes)
{
  auto pIndirectArgumentBufferD3D11 = static_cast<xiiGALBufferD3D11*>(pIndirectArgumentBuffer);

  XII_ASSERT_DEV(pIndirectArgumentBufferD3D11 != nullptr, "Invalid resource.");

  XII_SUCCEED_OR_RETURN(FlushDeferredStateChanges());

  m_pCommandList->DrawIndexedInstancedIndirect(pIndirectArgumentBufferD3D11->GetBuffer(), uiArgumentOffsetInBytes);

  return XII_SUCCESS;
}

xiiResult xiiGALCommandListD3D11::DrawInstancedPlatform(xiiUInt32 uiVertexCountPerInstance, xiiUInt32 uiInstanceCount, xiiUInt32 uiStartVertex, xiiUInt32 uiFirstInstance)
{
  XII_SUCCEED_OR_RETURN(FlushDeferredStateChanges());

  m_pCommandList->DrawInstanced(uiVertexCountPerInstance, uiInstanceCount, uiStartVertex, uiFirstInstance);

  return XII_SUCCESS;
}

xiiResult xiiGALCommandListD3D11::DrawInstancedIndirectPlatform(xiiGALBuffer* pIndirectArgumentBuffer, xiiUInt32 uiArgumentOffsetInBytes)
{
  auto pIndirectArgumentBufferD3D11 = static_cast<xiiGALBufferD3D11*>(pIndirectArgumentBuffer);

  XII_ASSERT_DEV(pIndirectArgumentBufferD3D11 != nullptr, "Invalid resource.");

  XII_SUCCEED_OR_RETURN(FlushDeferredStateChanges());

  m_pCommandList->DrawInstancedIndirect(pIndirectArgumentBufferD3D11->GetBuffer(), uiArgumentOffsetInBytes);

  return XII_SUCCESS;
}

xiiResult xiiGALCommandListD3D11::DrawMeshPlatform(xiiUInt32 uiThreadGroupCountX, xiiUInt32 uiThreadGroupCountY, xiiUInt32 uiThreadGroupCountZ)
{
  XII_REPORT_FAILURE("DrawMesh is not supported in Direct3D 11.");

  return XII_FAILURE;
}

xiiResult xiiGALCommandListD3D11::DispatchPlatform(xiiUInt32 uiThreadGroupCountX, xiiUInt32 uiThreadGroupCountY, xiiUInt32 uiThreadGroupCountZ)
{
  XII_SUCCEED_OR_RETURN(FlushDeferredStateChanges());

  m_pCommandList->Dispatch(uiThreadGroupCountX, uiThreadGroupCountY, uiThreadGroupCountZ);

  return XII_SUCCESS;
}

xiiResult xiiGALCommandListD3D11::DispatchIndirectPlatform(xiiGALBuffer* pIndirectArgumentBuffer, xiiUInt32 uiArgumentOffsetInBytes)
{
  auto pIndirectArgumentBufferD3D11 = static_cast<xiiGALBufferD3D11*>(pIndirectArgumentBuffer);

  XII_ASSERT_DEV(pIndirectArgumentBufferD3D11 != nullptr, "Invalid resource.");

  XII_SUCCEED_OR_RETURN(FlushDeferredStateChanges());

  m_pCommandList->DispatchIndirect(pIndirectArgumentBufferD3D11->GetBuffer(), uiArgumentOffsetInBytes);

  return XII_SUCCESS;
}

void xiiGALCommandListD3D11::BeginQueryPlatform(xiiGALQuery* pQuery)
{
  auto pQueryD3D11       = static_cast<xiiGALQueryD3D11*>(pQuery);
  auto pImmediateContext = static_cast<xiiGALDeviceD3D11*>(m_pDevice)->GetImmediateContext();

  XII_ASSERT_DEV(pQueryD3D11 != nullptr, "Invalid resource.");

  if (pQueryD3D11->GetDescription().m_Type == xiiGALQueryType::Duration)
  {
    pQueryD3D11->SetDisjointQuery(BeginDisjointQuery());

    pImmediateContext->End(pQueryD3D11->GetQuery(0));
  }
  else
  {
    pImmediateContext->Begin(pQueryD3D11->GetQuery(0));
  }
}

void xiiGALCommandListD3D11::EndQueryPlatform(xiiGALQuery* pQuery)
{
  auto pQueryD3D11 = static_cast<xiiGALQueryD3D11*>(pQuery);

  XII_ASSERT_DEV(pQueryD3D11 != nullptr, "Invalid resource.");

  xiiEnum<xiiGALQueryType> queryType = pQuery->GetDescription().m_Type;

  XII_ASSERT_DEV(queryType != xiiGALQueryType::Duration || m_pActiveDisjointQuery, "There is no active disjoint query. Did you forget to call BeginQuery for this duration query.");

  if (queryType == xiiGALQueryType::Timestamp)
  {
    pQueryD3D11->SetDisjointQuery(BeginDisjointQuery());
  }
  m_pCommandList->End(pQueryD3D11->GetQuery(queryType == xiiGALQueryType::Duration ? 1 : 0));
}

void xiiGALCommandListD3D11::UpdateBufferPlatform(xiiGALBuffer* pBuffer, xiiUInt32 uiDestinationOffset, xiiArrayPtr<const xiiUInt8> pSourceData)
{
  XII_CHECK_ALIGNMENT_16(pSourceData.GetPtr());

  auto pDestinationBufferD3D11 = static_cast<xiiGALBufferD3D11*>(pBuffer);

  XII_ASSERT_DEV(pDestinationBufferD3D11 != nullptr, "Invalid resource.");

  D3D11_BOX destinationBox = {};
  destinationBox.left      = uiDestinationOffset;
  destinationBox.right     = uiDestinationOffset + pSourceData.GetCount();
  destinationBox.top       = 0U;
  destinationBox.bottom    = 1U;
  destinationBox.front     = 0U;
  destinationBox.back      = 1U;

  D3D11_BOX* pDestinationBox = (uiDestinationOffset == 0 && pSourceData.GetCount() == pDestinationBufferD3D11->GetDescription().m_uiSize) ? nullptr : &destinationBox;

  m_pCommandList->UpdateSubresource(pDestinationBufferD3D11->GetBuffer(), 0U, pDestinationBox, pSourceData.GetPtr(), 0U, 0U);
}

void xiiGALCommandListD3D11::UpdateBufferExtendedPlatform(xiiGALBuffer* pBuffer, xiiUInt32 uiDestinationOffset, xiiArrayPtr<const xiiUInt8> pSourceData, xiiBitflags<xiiGALMapFlags> mapFlags, bool bCopyToTemporaryStorage)
{
  XII_CHECK_ALIGNMENT_16(pSourceData.GetPtr());

  xiiGALDeviceD3D11* pDeviceD3D11            = static_cast<xiiGALDeviceD3D11*>(m_pDevice);
  auto               pDestinationBufferD3D11 = static_cast<xiiGALBufferD3D11*>(pBuffer);

  XII_ASSERT_DEV(pDestinationBufferD3D11 != nullptr, "Invalid resource.");

  const auto& bufferDescription = pDestinationBufferD3D11->GetDescription();

  if (bufferDescription.m_BindFlags.IsSet(xiiGALBindFlags::UniformBuffer))
  {
    XII_ASSERT_DEV(uiDestinationOffset == 0 && pSourceData.GetCount() == bufferDescription.m_uiSize, "Constant buffers can't be updated partially (and we don't check for DX11.1)!");

    D3D11_MAPPED_SUBRESOURCE mapResult;
    if (SUCCEEDED(m_pCommandList->Map(pDestinationBufferD3D11->GetBuffer(), 0U, D3D11_MAP_WRITE_DISCARD, 0U, &mapResult)))
    {
      memcpy(mapResult.pData, pSourceData.GetPtr(), pSourceData.GetCount());

      m_pCommandList->Unmap(pDestinationBufferD3D11->GetBuffer(), 0);
    }
  }
  else
  {
    if (bCopyToTemporaryStorage)
    {
      if (ID3D11Resource* pD3D11TempBuffer = pDeviceD3D11->FindTemporaryBuffer(pSourceData.GetCount()))
      {
        auto pCommandList = pDeviceD3D11->GetImmediateContext(); // Used in buffer updates.

        D3D11_MAPPED_SUBRESOURCE MapResult;
        HRESULT                  hRes = pCommandList->Map(pD3D11TempBuffer, 0, D3D11_MAP_WRITE, 0, &MapResult);
        XII_ASSERT_DEV(SUCCEEDED(hRes), "Implementation error: {}", xiiHRESULTtoString(hRes));

        memcpy(MapResult.pData, pSourceData.GetPtr(), pSourceData.GetCount());

        pCommandList->Unmap(pD3D11TempBuffer, 0);

        // Schedule copy command using this command list.
        D3D11_BOX srcBox = {0, 0, 0, pSourceData.GetCount(), 1, 1};
        m_pCommandList->CopySubresourceRegion(pDestinationBufferD3D11->GetBuffer(), 0, uiDestinationOffset, 0, 0, pD3D11TempBuffer, 0, &srcBox);
      }
      else
      {
        XII_REPORT_FAILURE("Could not find a temp buffer for update.");
      }
    }
    else
    {
      D3D11_MAP mapType      = (mapFlags == xiiGALMapFlags::Discard) ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE;
      auto      pCommandList = (mapFlags == xiiGALMapFlags::Discard) ? m_pCommandList : pDeviceD3D11->GetImmediateContext(); // Used in buffer updates.

      D3D11_MAPPED_SUBRESOURCE mapResult;
      if (SUCCEEDED(pCommandList->Map(pDestinationBufferD3D11->GetBuffer(), 0, mapType, 0, &mapResult)))
      {
        memcpy(xiiMemoryUtils::AddByteOffset(mapResult.pData, uiDestinationOffset), pSourceData.GetPtr(), pSourceData.GetCount());

        pCommandList->Unmap(pDestinationBufferD3D11->GetBuffer(), 0);
      }
      else
      {
        xiiLog::Error("Could not map buffer to update content.");
      }
    }
  }
}

void xiiGALCommandListD3D11::CopyBufferPlatform(xiiGALBuffer* pSourceBuffer, xiiGALBuffer* pDestinationBuffer)
{
  auto pSourceBufferD3D11      = static_cast<xiiGALBufferD3D11*>(pSourceBuffer);
  auto pDestinationBufferD3D11 = static_cast<xiiGALBufferD3D11*>(pDestinationBuffer);

  XII_ASSERT_DEV(pSourceBufferD3D11 != nullptr, "Invalid resource.");
  XII_ASSERT_DEV(pDestinationBufferD3D11 != nullptr, "Invalid resource.");

  m_pCommandList->CopyResource(pDestinationBufferD3D11->GetBuffer(), pSourceBufferD3D11->GetBuffer());
}

void xiiGALCommandListD3D11::CopyBufferRegionPlatform(xiiGALBuffer* pSourceBuffer, xiiUInt64 uiSourceOffset, xiiGALBuffer* pDestinationBuffer, xiiUInt64 uiDestinationOffset, xiiUInt64 uiSize)
{
  auto pSourceBufferD3D11      = static_cast<xiiGALBufferD3D11*>(pSourceBuffer);
  auto pDestinationBufferD3D11 = static_cast<xiiGALBufferD3D11*>(pDestinationBuffer);

  XII_ASSERT_DEV(pSourceBufferD3D11 != nullptr, "Invalid resource.");
  XII_ASSERT_DEV(pDestinationBufferD3D11 != nullptr, "Invalid resource.");

  D3D11_BOX sourceBox = {};
  sourceBox.left      = static_cast<xiiUInt32>(uiSourceOffset);
  sourceBox.right     = static_cast<xiiUInt32>(uiSourceOffset + uiSize);
  sourceBox.top       = 0U;
  sourceBox.bottom    = 1U;
  sourceBox.front     = 0U;
  sourceBox.back      = 1U;

  m_pCommandList->CopySubresourceRegion(pDestinationBufferD3D11->GetBuffer(), 0, static_cast<xiiUInt32>(uiDestinationOffset), 0, 0, pSourceBufferD3D11->GetBuffer(), 0, &sourceBox);
}

xiiResult xiiGALCommandListD3D11::MapBufferPlatform(xiiGALBuffer* pBuffer, xiiEnum<xiiGALMapType> mapType, xiiBitflags<xiiGALMapFlags> mapFlags, void*& pMappedData)
{
  auto pBufferD3D11 = static_cast<xiiGALBufferD3D11*>(pBuffer);

  XII_ASSERT_DEV(pBuffer != nullptr, "Invalid resource.");

  D3D11_MAP bufferMapType = static_cast<D3D11_MAP>(0U);
  xiiUInt32 uiMapFlags    = 0U;
  xiiD3D11TypeConversions::GetMapTypeAndFlags(mapType, mapFlags, bufferMapType, uiMapFlags);

  // We need to use the immediate context to handle other map types.
  // If you call Map on a deferred context, you can only pass D3D11_MAP_WRITE_DISCARD, D3D11_MAP_WRITE_NO_OVERWRITE, or both to the MapType parameter.
  // Other D3D11_MAP-typed values are not supported for a deferred context.
  //
  // But also, there is an issue with needing to first map the buffer with discard before no overwrite, perhaps we can resolve this later.
  // D3D11 ERROR: ID3D11DeviceContext::Map: Returning D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD, meaning that MapType must be D3D11_MAP_WRITE_DISCARD when Map is called for the first time with a particular Resource on a Deferred Context. [ RESOURCE_MANIPULATION ERROR #2097216: RESOURCE_MAP_WITHOUT_INITIAL_DISCARD]
  auto pCommandList = m_pCommandList;
  if (bufferMapType != D3D11_MAP_WRITE_DISCARD /*&& bufferMapType != D3D11_MAP_WRITE_NO_OVERWRITE*/)
    pCommandList = static_cast<xiiGALDeviceD3D11*>(m_pDevice)->GetImmediateContext();

  D3D11_MAPPED_SUBRESOURCE mappedSubresource;
  if (FAILED(pCommandList->Map(pBufferD3D11->GetBuffer(), 0U, bufferMapType, uiMapFlags, &mappedSubresource)))
  {
    xiiLog::Error("Failed to map buffer '{0}'.", pBufferD3D11->GetDebugName());
    return XII_FAILURE;
  }

  pMappedData = mappedSubresource.pData;

  m_MappedBuffers.Insert(pBufferD3D11, pCommandList);

  return XII_SUCCESS;
}

xiiResult xiiGALCommandListD3D11::UnmapBufferPlatform(xiiGALBuffer* pBuffer, xiiEnum<xiiGALMapType> mapType)
{
  auto pBufferD3D11 = static_cast<xiiGALBufferD3D11*>(pBuffer);

  XII_ASSERT_DEV(pBuffer != nullptr, "Invalid resource.");

  auto pCommandList = *m_MappedBuffers.GetValue(pBufferD3D11);
  pCommandList->Unmap(pBufferD3D11->GetBuffer(), 0U);

  m_MappedBuffers.Remove(pBufferD3D11);

  return XII_SUCCESS;
}

void xiiGALCommandListD3D11::UpdateTexturePlatform(xiiGALTexture* pTexture, const xiiGALTextureMipLevelData& textureMiplevelData, const xiiBoundingBoxU32& textureBox, const xiiGALTextureSubResourceData& subresourceData)
{
  auto pTextureD3D11 = static_cast<xiiGALTextureD3D11*>(pTexture);

  XII_ASSERT_DEV(pTextureD3D11 != nullptr, "Invalid resource.");

  const auto& textureDescription = pTextureD3D11->GetDescription();

  XII_ASSERT_DEV(textureDescription.m_Usage == xiiGALResourceUsage::Default || textureDescription.m_Usage == xiiGALResourceUsage::Sparse, "Only xiiGALResourceUsage::Default or xiiGALResourceUsage::Default textures should be updated with this method.");

  if (!subresourceData.m_hSourceBuffer.IsInvalidated())
  {
    xiiLog::Error("Direct3D11 does not support texture updates using texture subresource from GPU buffer");
    return;
  }

  xiiUInt32 uiWidth  = xiiMath::Max(textureBox.m_vMax.x - textureBox.m_vMin.x, 1U);
  xiiUInt32 uiHeight = xiiMath::Max(textureBox.m_vMax.y - textureBox.m_vMin.y, 1U);
  xiiUInt32 uiDepth  = xiiMath::Max(textureBox.m_vMax.z - textureBox.m_vMin.z, 1U);

  D3D11_BOX destinationBox = {};
  destinationBox.left      = textureBox.m_vMin.x;
  destinationBox.top       = textureBox.m_vMin.y;
  destinationBox.front     = textureBox.m_vMin.z;
  destinationBox.right     = textureBox.m_vMax.x;
  destinationBox.bottom    = textureBox.m_vMax.y;
  destinationBox.back      = textureBox.m_vMax.z;

  const auto& formatProperties = xiiGALGraphicsUtilities::GetTextureFormatProperties(textureDescription.m_Format);

  if (formatProperties.m_ComponentType == xiiGALTextureFormatComponentType::Compressed)
  {
    // Align update region by the compressed block size.
    XII_ASSERT_DEV((destinationBox.left % formatProperties.m_uiBlockWidth) == 0, "The update region min X coordinate ({0}) must be a multiple of a compressed block width ({1}).", destinationBox.left, formatProperties.m_uiBlockWidth);
    XII_ASSERT_DEV((formatProperties.m_uiBlockWidth % (formatProperties.m_uiBlockWidth - 1)) == 0, "The compressed block width ({0}) is expected to be a power of 2.", formatProperties.m_uiBlockWidth);
    destinationBox.right = (destinationBox.right + formatProperties.m_uiBlockWidth - 1) & ~(formatProperties.m_uiBlockHeight - 1);

    XII_ASSERT_DEV((destinationBox.top % formatProperties.m_uiBlockHeight) == 0, "The update region min X coordinate ({0}) must be a multiple of a compressed block height ({1}).", destinationBox.top, formatProperties.m_uiBlockHeight);
    XII_ASSERT_DEV((formatProperties.m_uiBlockHeight % (formatProperties.m_uiBlockHeight - 1)) == 0, "The compressed block height ({0}) is expected to be a power of 2.", formatProperties.m_uiBlockHeight);
    destinationBox.bottom = (destinationBox.bottom + formatProperties.m_uiBlockHeight - 1) & ~(formatProperties.m_uiBlockHeight - 1);
  }

  xiiUInt32 uiDestinationSubresourceIndex = D3D11CalcSubresource(textureMiplevelData.m_uiMipLevel, textureMiplevelData.m_uiArraySlice, textureDescription.m_uiMipLevels);
  xiiUInt32 uiCopyFlags                   = D3D11_COPY_DISCARD;

  m_pCommandList->UpdateSubresource1(pTextureD3D11->GetTexture(), uiDestinationSubresourceIndex, &destinationBox, subresourceData.m_pData, static_cast<xiiUInt32>(subresourceData.m_uiStride), static_cast<xiiUInt32>(subresourceData.m_uiDepthStride), uiCopyFlags);
}

void xiiGALCommandListD3D11::UpdateTextureExtendedPlatform(xiiGALTexture* pTexture, const xiiGALTextureMipLevelData& textureMiplevelData, const xiiBoundingBoxU32& textureBox, const xiiGALTextureSubResourceData& subresourceData)
{
  xiiGALDeviceD3D11* pDeviceD3D11  = static_cast<xiiGALDeviceD3D11*>(m_pDevice);
  auto               pCommandList  = pDeviceD3D11->GetImmediateContext(); // Used in buffer updates.
  auto               pTextureD3D11 = static_cast<xiiGALTextureD3D11*>(pTexture);

  XII_ASSERT_DEV(pTextureD3D11 != nullptr, "Invalid resource.");

  const auto& textureDescription = pTextureD3D11->GetDescription();

  if (!subresourceData.m_hSourceBuffer.IsInvalidated())
  {
    xiiLog::Error("Direct3D11 does not support texture updates using texture subresource from GPU buffer");
    return;
  }

  xiiUInt32                    uiWidth  = xiiMath::Max(textureBox.m_vMax.x - textureBox.m_vMin.x, 1u);
  xiiUInt32                    uiHeight = xiiMath::Max(textureBox.m_vMax.y - textureBox.m_vMin.y, 1u);
  xiiUInt32                    uiDepth  = xiiMath::Max(textureBox.m_vMax.z - textureBox.m_vMin.z, 1u);
  xiiEnum<xiiGALTextureFormat> format   = pTextureD3D11->GetDescription().m_Format;

  if (ID3D11Resource* pDXTempTexture = pDeviceD3D11->FindTemporaryTexture(uiWidth, uiHeight, uiDepth, format))
  {
    D3D11_MAPPED_SUBRESOURCE MapResult;
    HRESULT                  hRes = pCommandList->Map(pDXTempTexture, 0, D3D11_MAP_WRITE, 0, &MapResult);
    XII_ASSERT_DEV(SUCCEEDED(hRes), "Implementation error: {}", xiiHRESULTtoString(hRes));

    xiiUInt32 uiRowPitch   = uiWidth * xiiGALGraphicsUtilities::GetTextureFormatProperties(format).GetElementSize();
    xiiUInt32 uiSlicePitch = uiRowPitch * uiHeight;
    XII_ASSERT_DEV(subresourceData.m_uiStride == uiRowPitch, "Invalid row pitch. Expected {0} got {1}", uiRowPitch, subresourceData.m_uiStride);
    XII_ASSERT_DEV(subresourceData.m_uiDepthStride == 0 || subresourceData.m_uiDepthStride == uiSlicePitch, "Invalid slice pitch. Expected {0} got {1}", uiSlicePitch, subresourceData.m_uiDepthStride);

    if (MapResult.RowPitch == uiRowPitch && MapResult.DepthPitch == uiSlicePitch)
    {
      memcpy(MapResult.pData, subresourceData.m_pData, uiSlicePitch * uiDepth);
    }
    else
    {
      // Copy row by row
      for (xiiUInt32 z = 0; z < uiDepth; ++z)
      {
        const void* pSource = xiiMemoryUtils::AddByteOffset(subresourceData.m_pData, z * uiSlicePitch);
        void*       pDest   = xiiMemoryUtils::AddByteOffset(MapResult.pData, z * MapResult.DepthPitch);

        for (xiiUInt32 y = 0; y < uiHeight; ++y)
        {
          memcpy(pDest, pSource, uiRowPitch);

          pSource = xiiMemoryUtils::AddByteOffset(pSource, uiRowPitch);
          pDest   = xiiMemoryUtils::AddByteOffset(pDest, MapResult.RowPitch);
        }
      }
    }

    pCommandList->Unmap(pDXTempTexture, 0);

    xiiUInt32 dstSubResource = D3D11CalcSubresource(textureMiplevelData.m_uiMipLevel, textureMiplevelData.m_uiArraySlice, pTextureD3D11->GetDescription().m_uiMipLevels);

    // Schedule copy command using this command list.
    D3D11_BOX srcBox = {0, 0, 0, uiWidth, uiHeight, uiDepth};
    m_pCommandList->CopySubresourceRegion(pTextureD3D11->GetTexture(), dstSubResource, textureBox.m_vMin.x, textureBox.m_vMin.y, textureBox.m_vMin.z, pDXTempTexture, 0, &srcBox);
  }
  else
  {
    XII_REPORT_FAILURE("Could not find a temp texture for update.");
  }
}

void xiiGALCommandListD3D11::CopyTexturePlatform(xiiGALTexture* pSourceTexture, xiiGALTexture* pDestinationTexture)
{
  auto pSourceTextureD3D11      = static_cast<xiiGALTextureD3D11*>(pSourceTexture);
  auto pDestinationTextureD3D11 = static_cast<xiiGALTextureD3D11*>(pDestinationTexture);

  XII_ASSERT_DEV(pSourceTextureD3D11 != nullptr, "Invalid resource.");
  XII_ASSERT_DEV(pDestinationTextureD3D11 != nullptr, "Invalid resource.");

  m_pCommandList->CopyResource(pDestinationTextureD3D11->GetTexture(), pSourceTextureD3D11->GetTexture());
}

void xiiGALCommandListD3D11::CopyTextureRegionPlatform(xiiGALTexture* pSourceTexture, const xiiGALTextureMipLevelData& sourceMipLevelData, const xiiBoundingBoxU32& box, xiiGALTexture* pDestinationTexture, const xiiGALTextureMipLevelData& destinationMipLevelData, const xiiVec3U32& vDestinationPoint)
{
  auto pSourceTextureD3D11      = static_cast<xiiGALTextureD3D11*>(pSourceTexture);
  auto pDestinationTextureD3D11 = static_cast<xiiGALTextureD3D11*>(pDestinationTexture);

  XII_ASSERT_DEV(pSourceTextureD3D11 != nullptr, "Invalid resource.");
  XII_ASSERT_DEV(pDestinationTextureD3D11 != nullptr, "Invalid resource.");

  D3D11_BOX sourceBox = {};
  sourceBox.left      = box.m_vMin.x;
  sourceBox.top       = box.m_vMin.y;
  sourceBox.front     = box.m_vMin.z;
  sourceBox.right     = box.m_vMax.x;
  sourceBox.bottom    = box.m_vMax.y;
  sourceBox.back      = box.m_vMax.z;

  xiiUInt32 uiSourceSubresource      = D3D11CalcSubresource(sourceMipLevelData.m_uiMipLevel, sourceMipLevelData.m_uiArraySlice, pSourceTextureD3D11->GetDescription().m_uiMipLevels);
  xiiUInt32 uiDestinationSubresource = D3D11CalcSubresource(destinationMipLevelData.m_uiMipLevel, destinationMipLevelData.m_uiArraySlice, pDestinationTextureD3D11->GetDescription().m_uiMipLevels);

  m_pCommandList->CopySubresourceRegion(pDestinationTextureD3D11->GetTexture(), uiDestinationSubresource, vDestinationPoint.x, vDestinationPoint.y, vDestinationPoint.z, pSourceTextureD3D11->GetTexture(), uiSourceSubresource, &sourceBox);
}

void xiiGALCommandListD3D11::ResolveTextureSubResourcePlatform(xiiGALTexture* pSourceTexture, const xiiGALTextureMipLevelData& sourceMipLevelData, xiiGALTexture* pDestinationTexture, const xiiGALTextureMipLevelData& destinationMipLevelData)
{
  auto pSourceTextureD3D11      = static_cast<xiiGALTextureD3D11*>(pSourceTexture);
  auto pDestinationTextureD3D11 = static_cast<xiiGALTextureD3D11*>(pDestinationTexture);

  XII_ASSERT_DEV(pSourceTextureD3D11 != nullptr, "Invalid resource.");
  XII_ASSERT_DEV(pDestinationTextureD3D11 != nullptr, "Invalid resource.");

  const auto& sourceTextureDescription      = pSourceTextureD3D11->GetDescription();
  const auto& destinationTextureDescription = pSourceTextureD3D11->GetDescription();

  DXGI_FORMAT textureFormat                 = xiiD3D11TypeConversions ::GetFormat(sourceTextureDescription.m_Format);
  xiiUInt32   uiSourceSubresourceIndex      = D3D11CalcSubresource(sourceMipLevelData.m_uiMipLevel, sourceMipLevelData.m_uiMipLevel, sourceTextureDescription.m_uiMipLevels);
  xiiUInt32   uiDestinationSubresourceIndex = D3D11CalcSubresource(destinationMipLevelData.m_uiMipLevel, destinationMipLevelData.m_uiMipLevel, destinationTextureDescription.m_uiMipLevels);

  m_pCommandList->ResolveSubresource(pDestinationTextureD3D11->GetTexture(), uiDestinationSubresourceIndex, pSourceTextureD3D11->GetTexture(), uiSourceSubresourceIndex, textureFormat);
}

void xiiGALCommandListD3D11::GenerateMipsPlatform(xiiGALTextureView* pTextureView)
{
  auto* pTextureViewD3D11 = static_cast<xiiGALTextureViewD3D11*>(pTextureView);

  XII_ASSERT_DEV(pTextureViewD3D11 != nullptr, "Invalid resource.");

  m_pCommandList->GenerateMips(static_cast<ID3D11ShaderResourceView*>(pTextureViewD3D11->GetTextureView()));
}

xiiResult xiiGALCommandListD3D11::MapTextureSubresourcePlatform(xiiGALTexture* pTexture, xiiGALTextureMipLevelData textureMipLevelData, xiiEnum<xiiGALMapType> mapType, xiiBitflags<xiiGALMapFlags> mapFlags, xiiBoundingBoxU32* pTextureBox, xiiGALMappedTextureSubresource& mappedData)
{
  auto pTextureD3D11 = static_cast<xiiGALTextureD3D11*>(pTexture);

  XII_ASSERT_DEV(pTextureD3D11 != nullptr, "Invalid resource.");

  const auto& textureDescription = pTextureD3D11->GetDescription();
  D3D11_MAP   textureMapType     = static_cast<D3D11_MAP>(0U);
  xiiUInt32   uiMapFlags         = 0U;
  xiiD3D11TypeConversions::GetMapTypeAndFlags(mapType, mapFlags, textureMapType, uiMapFlags);

  xiiUInt32 uiSubresource = D3D11CalcSubresource(textureMipLevelData.m_uiMipLevel, textureMipLevelData.m_uiArraySlice, textureDescription.m_uiMipLevels);

  // We need to use the immediate context to handle other map types.
  // If you call Map on a deferred context, you can only pass D3D11_MAP_WRITE_DISCARD, D3D11_MAP_WRITE_NO_OVERWRITE, or both to the MapType parameter.
  // Other D3D11_MAP-typed values are not supported for a deferred context.
  auto pCommandList = m_pCommandList;
  if (textureMapType != D3D11_MAP_WRITE_DISCARD && textureMapType != D3D11_MAP_WRITE_NO_OVERWRITE)
    pCommandList = static_cast<xiiGALDeviceD3D11*>(m_pDevice)->GetImmediateContext();

  D3D11_MAPPED_SUBRESOURCE mappedSubresource;
  HRESULT                  hr = pCommandList->Map(pTextureD3D11->GetTexture(), uiSubresource, textureMapType, uiMapFlags, &mappedSubresource);
  if (FAILED(hr))
  {
    // XII_ASSERT_DEV(hResult == DXGI_ERROR_WAS_STILL_DRAWING, "");

    xiiLog::Error("Failed to map texture subresource: {0}", xiiHRESULTtoString(hr));

    mappedData = xiiGALMappedTextureSubresource();
    return XII_FAILURE;
  }

  mappedData.m_pData         = mappedSubresource.pData;
  mappedData.m_uiStride      = mappedSubresource.RowPitch;
  mappedData.m_uiDepthStride = mappedSubresource.DepthPitch;

  m_MappedTextureSubresources.Insert(pTextureD3D11, pCommandList);

  return XII_SUCCESS;
}

xiiResult xiiGALCommandListD3D11::UnmapTextureSubresourcePlatform(xiiGALTexture* pTexture, xiiGALTextureMipLevelData textureMipLevelData)
{
  auto pTextureD3D11 = static_cast<xiiGALTextureD3D11*>(pTexture);

  XII_ASSERT_DEV(pTextureD3D11 != nullptr, "Invalid resource.");

  const auto& textureDescription = pTextureD3D11->GetDescription();
  xiiUInt32   uiSubresource      = D3D11CalcSubresource(textureMipLevelData.m_uiMipLevel, textureMipLevelData.m_uiArraySlice, textureDescription.m_uiMipLevels);

  auto pCommandList = *m_MappedTextureSubresources.GetValue(pTextureD3D11);
  pCommandList->Unmap(pTextureD3D11->GetTexture(), uiSubresource);

  m_MappedTextureSubresources.Remove(pTextureD3D11);

  return XII_SUCCESS;
}

void xiiGALCommandListD3D11::BeginDebugGroupPlatform(xiiStringView sName, const xiiColor& color)
{
  ID3DUserDefinedAnnotation* pAnnotationD3D11 = nullptr;
  XII_SCOPE_EXIT(XII_GAL_D3D11_RELEASE(pAnnotationD3D11));

  if (SUCCEEDED(m_pCommandList->QueryInterface(_uuidof(ID3DUserDefinedAnnotation), (void**)&pAnnotationD3D11)))
  {
    xiiStringBuilder sb;
    xiiStringWChar   wsMarker(sName.GetData(sb));
    pAnnotationD3D11->BeginEvent(wsMarker.GetData());
  }
}

void xiiGALCommandListD3D11::EndDebugGroupPlatform()
{
  ID3DUserDefinedAnnotation* pAnnotationD3D11 = nullptr;
  XII_SCOPE_EXIT(XII_GAL_D3D11_RELEASE(pAnnotationD3D11));

  if (SUCCEEDED(m_pCommandList->QueryInterface(_uuidof(ID3DUserDefinedAnnotation), (void**)&pAnnotationD3D11)))
  {
    pAnnotationD3D11->EndEvent();
  }
}

void xiiGALCommandListD3D11::InsertDebugLabelPlatform(xiiStringView sName, const xiiColor& color)
{
  ID3DUserDefinedAnnotation* pAnnotationD3D11 = nullptr;
  XII_SCOPE_EXIT(XII_GAL_D3D11_RELEASE(pAnnotationD3D11));

  if (SUCCEEDED(m_pCommandList->QueryInterface(_uuidof(ID3DUserDefinedAnnotation), (void**)&pAnnotationD3D11)))
  {
    xiiStringBuilder sb;
    xiiStringWChar   wsMarker(sName.GetData(sb));
    pAnnotationD3D11->SetMarker(wsMarker.GetData());
  }
}

void xiiGALCommandListD3D11::FlushPlatform()
{
  // Though D3D11 does not care about render passes, we call flush when we unset resource views that are set as render targets.
  // XII_ASSERT_DEV(m_hRenderPass.IsInvalidated(), "Flushing commandlist inside an active render pass is not allowed.");

  m_pCommandList->Flush();
}

void xiiGALCommandListD3D11::InvalidateStatePlatform()
{
  m_pCommandList->ClearState();

  InvalidateResources();
}

void xiiGALCommandListD3D11::InvalidateResources()
{
  m_pPipelineState = nullptr;

  for (xiiUInt32 i = 0; i < XII_ARRAY_SIZE(m_pCommittedVertexBuffers); ++i)
  {
    m_pCommittedVertexBuffers[i] = nullptr;
  }
  for (xiiUInt32 i = 0; i < XII_ARRAY_SIZE(m_CommittedVertexBufferStrides); ++i)
  {
    m_CommittedVertexBufferStrides[i] = 0U;
  }
  for (xiiUInt32 i = 0; i < XII_ARRAY_SIZE(m_CommittedVertexBufferOffsets); ++i)
  {
    m_CommittedVertexBufferOffsets[i] = 0U;
  }
  m_CommittedVertexBuffersRange.Reset();

  m_pCommittedInputLayout           = nullptr;
  m_pCommittedIndexBuffer           = nullptr;
  m_CommittedIndexBufferFormat      = {};
  m_uiCommittedIndexDataStartOffset = 0U;

  m_CommittedPrimitiveTopology  = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
  m_CommittedBlendFactors       = xiiColor::White;
  m_uiCommittedBlendSampleMask  = 0xFFFFFFFFU;
  m_uiCommittedStencilReference = 0xFFU;

  for (xiiUInt32 i = 0; i < XII_ARRAY_SIZE(m_pCommittedRenderTargets); ++i)
  {
    m_pCommittedRenderTargets[i] = nullptr;
  }
  m_uiBoundRenderTargetCount     = 0U;
  m_pCommittedDepthStencilTarget = nullptr;
}

void xiiGALCommandListD3D11::CommitRenderTargets()
{
  xiiUInt32 uiOldRenderTargetCount = m_uiBoundRenderTargetCount;

  ResetRenderTargets();

  if (!m_pRenderPass || !m_pFramebuffer)
    return;

  const auto& renderPassDescription  = m_pRenderPass->GetDescription();
  const auto& framebufferDescription = m_pFramebuffer->GetDescription();
  const auto& currentSubpass         = renderPassDescription.m_SubPasses[m_uiSubpassIndex];

  const xiiGALTextureViewD3D11* pDepthStencilView = nullptr;
  if (!currentSubpass.m_DepthStencilAttachment.IsEmpty())
  {
    const auto& attachmentDescription = currentSubpass.m_DepthStencilAttachment.PeekBack();

    if (attachmentDescription.m_uiAttachmentIndex != XII_GAL_ATTACHMENT_UNUSED)
    {
      pDepthStencilView              = static_cast<xiiGALTextureViewD3D11*>(m_pDevice->GetTextureView(framebufferDescription.m_Attachments[attachmentDescription.m_uiAttachmentIndex]));
      m_pCommittedDepthStencilTarget = static_cast<ID3D11DepthStencilView*>(pDepthStencilView->GetTextureView());

      XII_ASSERT_DEV((pDepthStencilView->GetDescription().m_ViewType == xiiGALTextureViewType::DepthStencil || pDepthStencilView->GetDescription().m_ViewType == xiiGALTextureViewType::ReadOnlyDepthStencil), "Expected xiiGALTextureViewType::DepthStencil or xiiGALTextureViewType::ReadOnlyDepthStencil at the subpass depth attachment render target index.");
    }
  }

  const xiiGALTextureViewD3D11* pAttachmentViews[XII_GAL_MAX_RENDERTARGET_COUNT] = {};
  xiiGAL::ModifiedRange         boundRenderTargetsRange;

  for (xiiUInt32 i = 0; i < currentSubpass.m_RenderTargetAttachments.GetCount(); ++i)
  {
    const auto& attachmentDescription = currentSubpass.m_RenderTargetAttachments[i];

    if (attachmentDescription.m_uiAttachmentIndex == XII_GAL_ATTACHMENT_UNUSED)
      continue;

    auto pRenderTargetView = static_cast<xiiGALTextureViewD3D11*>(m_pDevice->GetTextureView(framebufferDescription.m_Attachments[attachmentDescription.m_uiAttachmentIndex]));

    XII_ASSERT_DEV((pRenderTargetView->GetDescription().m_ViewType == xiiGALTextureViewType::RenderTarget), "Expected xiiGALTextureViewType::RenderTarget at the subpass color attachment render target index.");

    pAttachmentViews[i]          = pRenderTargetView;
    m_pCommittedRenderTargets[i] = static_cast<ID3D11RenderTargetView*>(pRenderTargetView->GetTextureView());

    boundRenderTargetsRange.SetToIncludeValue(i);
  }
  m_uiBoundRenderTargetCount = boundRenderTargetsRange.GetCount();

  // Unbind these attachments that will be used for output by the subpass.
  // There is no need to unbind textures from output as the new subpass attachments
  // will be committed as render target/depth stencil anyway, so these that can be used for
  // input will be unbound.
  {
    bool bFlushNeeded = false;

    if (m_pPipelineState != nullptr && pDepthStencilView != nullptr)
    {
      bFlushNeeded |= m_pPipelineState->UnsetResourceViews(pDepthStencilView->GetTexture());
      bFlushNeeded |= m_pPipelineState->UnsetUnorderedAccessViews(pDepthStencilView->GetTexture());
    }

    for (xiiUInt32 i = boundRenderTargetsRange.m_uiMin; i < boundRenderTargetsRange.GetCount(); ++i)
    {
      if (m_pPipelineState != nullptr && pAttachmentViews[i] != nullptr)
      {
        bFlushNeeded |= m_pPipelineState->UnsetResourceViews(pAttachmentViews[i]->GetTexture());
        bFlushNeeded |= m_pPipelineState->UnsetUnorderedAccessViews(pAttachmentViews[i]->GetTexture());
      }
    }

    if (bFlushNeeded)
    {
      FlushDeferredStateChanges().IgnoreResult();
      FlushPlatform();
    }
  }

  m_pCommandList->OMSetRenderTargets(xiiMath::Max(boundRenderTargetsRange.GetCount(), uiOldRenderTargetCount), m_pCommittedRenderTargets, m_pCommittedDepthStencilTarget);

  // Clear render targets.
  for (xiiUInt32 i = 0; i < renderPassDescription.m_Attachments.GetCount(); ++i)
  {
    const auto& attachmentDescription = renderPassDescription.m_Attachments[i];
    const auto& hAttachment           = framebufferDescription.m_Attachments[i];

    if (attachmentDescription.m_LoadOperation == xiiGALAttachmentLoadOperation::Clear)
    {
      xiiGALTextureView* pTextureView = m_pDevice->GetTextureView(hAttachment);

      if (pTextureView->GetDescription().m_ViewType == xiiGALTextureViewType::DepthStencil || pTextureView->GetDescription().m_ViewType == xiiGALTextureViewType::ReadOnlyDepthStencil)
      {
        bool bClearStencil = attachmentDescription.m_StencilLoadOperation == xiiGALAttachmentLoadOperation::Clear;

        ClearDepthStencilViewPlatform(pTextureView, true, bClearStencil, m_AttachmentClearValues[i].m_DepthStencil.m_fDepth, m_AttachmentClearValues[i].m_DepthStencil.m_uiStencil);
      }
      else if (pTextureView->GetDescription().m_ViewType == xiiGALTextureViewType::RenderTarget)
      {
        ClearRenderTargetViewPlatform(pTextureView, m_AttachmentClearValues[i].m_ClearColor);
      }
    }
  }
}

void xiiGALCommandListD3D11::ResetRenderTargets()
{
  for (xiiUInt32 i = 0; i < XII_ARRAY_SIZE(m_pCommittedRenderTargets); ++i)
  {
    m_pCommittedRenderTargets[i] = nullptr;
  }
  m_pCommittedDepthStencilTarget = nullptr;
  m_uiBoundRenderTargetCount     = 0U;

  m_pCommandList->OMSetRenderTargets(0, nullptr, nullptr);
}

void xiiGALCommandListD3D11::ReleaseInternalCommandList()
{
  XII_GAL_D3D11_RELEASE(m_pSubmittedCommandList);
}

xiiSharedPtr<xiiDisjointQueryPool::DisjointQueryWrapper> xiiGALCommandListD3D11::BeginDisjointQuery()
{
  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);

  if (!m_pActiveDisjointQuery)
  {
    m_pActiveDisjointQuery = m_DisjointQueryPool.GetDisjointQuery(pDeviceD3D11->GetD3D11Device());

    // Disjoint timestamp queries should be only invoked once per frame or less.
    m_pCommandList->Begin(m_pActiveDisjointQuery->m_pQueryD3D11);

    m_pActiveDisjointQuery->m_bIsEnded = false;
  }
  return m_pActiveDisjointQuery;
}

xiiResult xiiGALCommandListD3D11::FlushDeferredStateChanges()
{
  // Commit primitive topology.
  m_pCommandList->IASetPrimitiveTopology(m_CommittedPrimitiveTopology);

  // Commit vertex buffers.
  if (m_CommittedVertexBuffersRange.IsValid())
  {
    const xiiUInt32 uiStartSlot = m_CommittedVertexBuffersRange.m_uiMin;
    const xiiUInt32 uiNumSlots  = m_CommittedVertexBuffersRange.GetCount();

    m_pCommandList->IASetVertexBuffers(uiStartSlot, uiNumSlots, m_pCommittedVertexBuffers + uiStartSlot, m_CommittedVertexBufferStrides + uiStartSlot, m_CommittedVertexBufferOffsets + uiStartSlot);
  }

  // Commit index buffer
  m_pCommandList->IASetIndexBuffer(m_pCommittedIndexBuffer, m_CommittedIndexBufferFormat, m_uiCommittedIndexDataStartOffset);

  // Commit graphics pipeline states.
  m_pCommandList->IASetInputLayout(m_pPipelineState->GetD3D11InputLayout());
  m_pCommandList->OMSetBlendState(m_pPipelineState->GetD3D11BlendState(), m_CommittedBlendFactors.GetData(), m_uiCommittedBlendSampleMask);
  m_pCommandList->RSSetState(m_pPipelineState->GetD3D11RasterizerState());
  m_pCommandList->OMSetDepthStencilState(m_pPipelineState->GetD3D11DepthStencilState(), m_uiCommittedStencilReference);

  // Commit shader resources.
  XII_SUCCEED_OR_RETURN(m_pPipelineState->CommitShaderResources(this));

  return XII_SUCCESS;
}

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_CommandEncoder_Implementation_CommandListD3D11);
