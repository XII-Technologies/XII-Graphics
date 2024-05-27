#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/Device/Device.h>

#include <Foundation/Profiling/Profiling.h>

#include <GraphicsFoundation/CommandEncoder/CommandQueue.h>
#include <GraphicsFoundation/Device/SwapChain.h>
#include <GraphicsFoundation/Resources/BottomLevelAS.h>
#include <GraphicsFoundation/Resources/Buffer.h>
#include <GraphicsFoundation/Resources/BufferView.h>
#include <GraphicsFoundation/Resources/Fence.h>
#include <GraphicsFoundation/Resources/Framebuffer.h>
#include <GraphicsFoundation/Resources/Query.h>
#include <GraphicsFoundation/Resources/RenderPass.h>
#include <GraphicsFoundation/Resources/Sampler.h>
#include <GraphicsFoundation/Resources/Texture.h>
#include <GraphicsFoundation/Resources/TextureView.h>
#include <GraphicsFoundation/Resources/TopLevelAS.h>
#include <GraphicsFoundation/Shader/InputLayout.h>
#include <GraphicsFoundation/Shader/Shader.h>
#include <GraphicsFoundation/States/BlendState.h>
#include <GraphicsFoundation/States/DepthStencilState.h>
#include <GraphicsFoundation/States/PipelineResourceSignature.h>
#include <GraphicsFoundation/States/PipelineState.h>
#include <GraphicsFoundation/States/RasterizerState.h>
#include <GraphicsFoundation/Utilities/GraphicsUtilities.h>

namespace
{
  static constexpr xiiUInt32 s_uiMaxResourcesInSignature = XII_BIT(16) - 1U;
  static constexpr xiiUInt32 s_uiMaxCommandQueues        = 64U;

  struct GALObjectType
  {
    using StorageType = xiiUInt8;

    enum Enum : xiiUInt8
    {
      SwapChain = 0U,
      CommandQueue,
      BottomLevelAS,
      Buffer,
      BufferView,
      Fence,
      Framebuffer,
      Query,
      RenderPass,
      Sampler,
      Texture,
      TextureView,
      TopLevelAS,
      InputLayout,
      Shader,
      BlendState,
      DepthStencilState,
      RasterizerState,
      PipelineResourceSignature,
      PipelineState
    };
  };

  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALSwapChainHandle) == sizeof(xiiUInt32));
  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALCommandQueueHandle) == sizeof(xiiUInt32));
  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALBottomLevelASHandle) == sizeof(xiiUInt32));
  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALBufferHandle) == sizeof(xiiUInt32));
  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALBufferViewHandle) == sizeof(xiiUInt32));
  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALFenceHandle) == sizeof(xiiUInt32));
  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALFramebufferHandle) == sizeof(xiiUInt32));
  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALQueryHandle) == sizeof(xiiUInt32));
  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALRenderPassHandle) == sizeof(xiiUInt32));
  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALSamplerHandle) == sizeof(xiiUInt32));
  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALTextureHandle) == sizeof(xiiUInt32));
  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALTextureViewHandle) == sizeof(xiiUInt32));
  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALTopLevelASHandle) == sizeof(xiiUInt32));
  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALInputLayoutHandle) == sizeof(xiiUInt32));
  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALShaderHandle) == sizeof(xiiUInt32));
  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALBlendStateHandle) == sizeof(xiiUInt32));
  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALDepthStencilStateHandle) == sizeof(xiiUInt32));
  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALRasterizerStateHandle) == sizeof(xiiUInt32));
  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALPipelineResourceSignatureHandle) == sizeof(xiiUInt32));
  XII_CHECK_AT_COMPILETIME(sizeof(xiiGALPipelineStateHandle) == sizeof(xiiUInt32));
} // namespace

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALDevice, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;

xiiGALDevice*                      xiiGALDevice::s_pDefaultDevice = nullptr;
xiiEvent<const xiiGALDeviceEvent&> xiiGALDevice::s_Events;

xiiGALDevice::xiiGALDevice(const xiiGALDeviceCreationDescription& creationDescription) :
  xiiGALObject(), m_Description(creationDescription), m_Allocator("GALDevice", xiiFoundation::GetDefaultAllocator()), m_AllocatorWrapper(&m_Allocator)
{
}

xiiGALDevice::~xiiGALDevice()
{
  // Check for object leaks.
  {
    XII_LOG_BLOCK("xiiGALDevice object leak report.");

    if (!m_SwapChains.IsEmpty())
      xiiLog::Warning("{0} swap chains have not been cleaned up.", m_SwapChains.GetCount());

    if (!m_BottomLevelAccelerationStructures.IsEmpty())
      xiiLog::Warning("{0} bottom-level acceleration structures have not been cleaned up.", m_BottomLevelAccelerationStructures.GetCount());

    if (!m_Buffers.IsEmpty())
      xiiLog::Warning("{0} buffers have not been cleaned up.", m_Buffers.GetCount());

    if (!m_BufferViews.IsEmpty())
      xiiLog::Warning("{0} buffer views have not been cleaned up.", m_BufferViews.GetCount());

    if (!m_Fences.IsEmpty())
      xiiLog::Warning("{0} fences have not been cleaned up.", m_Fences.GetCount());

    if (!m_Framebuffers.IsEmpty())
      xiiLog::Warning("{0} framebuffers have not been cleaned up.", m_Framebuffers.GetCount());

    if (!m_Queries.IsEmpty())
      xiiLog::Warning("{0} queries have not been cleaned up.", m_Queries.GetCount());

    if (!m_RenderPasses.IsEmpty())
      xiiLog::Warning("{0} render passes have not been cleaned up.", m_RenderPasses.GetCount());

    if (!m_Samplers.IsEmpty())
      xiiLog::Warning("{0} samplers have not been cleaned up.", m_Samplers.GetCount());

    if (!m_Textures.IsEmpty())
      xiiLog::Warning("{0} textures have not been cleaned up.", m_Textures.GetCount());

    if (!m_TopLevelAccelerationStructures.IsEmpty())
      xiiLog::Warning("{0} top-level acceleration structures have not been cleaned up.", m_TopLevelAccelerationStructures.GetCount());

    if (!m_InputLayouts.IsEmpty())
      xiiLog::Warning("{0} input layouts have not been cleaned up.", m_InputLayouts.GetCount());

    if (!m_Shaders.IsEmpty())
      xiiLog::Warning("{0} shaders have not been cleaned up.", m_Shaders.GetCount());

    if (!m_BlendStates.IsEmpty())
      xiiLog::Warning("{0} blend states have not been cleaned up.", m_BlendStates.GetCount());

    if (!m_DepthStencilStates.IsEmpty())
      xiiLog::Warning("{0} depth stencil states have not been cleaned up.", m_DepthStencilStates.GetCount());

    if (!m_RasterizerStates.IsEmpty())
      xiiLog::Warning("{0} rasterizer states have not been cleaned up.", m_RasterizerStates.GetCount());

    if (!m_PipelineResourceSignatures.IsEmpty())
      xiiLog::Warning("{0} pipeline resource signatures have not been cleaned up.", m_PipelineResourceSignatures.GetCount());

    if (!m_PipelineStates.IsEmpty())
      xiiLog::Warning("{0} pipeline states have not been cleaned up.", m_PipelineStates.GetCount());
  }
}

xiiResult xiiGALDevice::Initialize()
{
  XII_LOG_BLOCK("xiiGALDevice::Initialize");

  XII_SUCCEED_OR_RETURN(InitializePlatform());

  // Fill the device capabilities
  FillCapabilitiesPlatform();

  // Create command queues.
  XII_SUCCEED_OR_RETURN(CreateCommandQueuesPlatform());

  xiiLog::Info("Adapter: '{}' - {} VRAM, {} Sys RAM, {} Shared RAM.", m_AdapterDescription.m_sAdapterName, xiiArgFileSize(m_AdapterDescription.m_MemoryProperties.m_uiLocalMemory),
               xiiArgFileSize(m_AdapterDescription.m_MemoryProperties.m_uiHostVisibleMemory), xiiArgFileSize(m_AdapterDescription.m_MemoryProperties.m_uiUnifiedMemory));

  if (m_AdapterDescription.m_Type != xiiGALDeviceAdapterType::Discrete)
  {
    xiiLog::Warning("Selected graphics adapter has no hardware acceleration.");
  }

  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiProfilingSystem::InitializeGPUData();

  {
    xiiGALDeviceEvent e;
    e.m_pDevice = this;
    e.m_Type    = xiiGALDeviceEventType::AfterInitialization;
    s_Events.Broadcast(e);
  }

  return XII_SUCCESS;
}

xiiResult xiiGALDevice::Shutdown()
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  XII_LOG_BLOCK("xiiGALDevice::Shutdown");

  {
    xiiGALDeviceEvent e;
    e.m_pDevice = this;
    e.m_Type    = xiiGALDeviceEventType::BeforeShutdown;
    s_Events.Broadcast(e);
  }

  FlushDestroyedObjects();

  // Ensure we are not listed as the default device.
  if (xiiGALDevice::HasDefaultDevice() && xiiGALDevice::GetDefaultDevice() == this)
  {
    xiiGALDevice::SetDefaultDevice(nullptr);
  }

  return ShutdownPlatform();
}

void xiiGALDevice::BeginPipeline(xiiStringView sName, xiiGALSwapChainHandle hSwapChain)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  XII_ASSERT_DEV(!m_bBeginPipelineCalled, "Nested Pipelines are not allowed: You must call xiiGALDevice::EndPipeline before you can call xiiGALDevice::BeginPipeline again.");
  m_bBeginPipelineCalled = true;

  xiiGALSwapChain* pSwapChain = nullptr;
  m_SwapChains.TryGetValue(hSwapChain, pSwapChain);
  BeginPipelinePlatform(sName, pSwapChain);
}

void xiiGALDevice::EndPipeline(xiiGALSwapChainHandle hSwapChain)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  XII_ASSERT_DEV(m_bBeginPipelineCalled, "You must have called xiiGALDevice::BeginPipeline before you can call xiiGALDevice::EndPipeline.");
  m_bBeginPipelineCalled = false;

  xiiGALSwapChain* pSwapChain = nullptr;
  m_SwapChains.TryGetValue(hSwapChain, pSwapChain);
  EndPipelinePlatform(pSwapChain);
}

void xiiGALDevice::BeginFrame(const xiiUInt64 uiRenderFrame)
{
  {
    XII_PROFILE_SCOPE("BeforeBeginFrame");

    xiiGALDeviceEvent e;
    e.m_pDevice = this;
    e.m_Type    = xiiGALDeviceEventType::BeforeBeginFrame;
    s_Events.Broadcast(e);
  }

  {
    XII_GAL_DEVICE_LOCK_AND_CHECK();

    XII_ASSERT_DEV(!m_bBeginFrameCalled, "You must call xiiGALDevice::EndFrame before you can call xiiGALDevice::BeginFrame again");
    m_bBeginFrameCalled = true;

    BeginFramePlatform(uiRenderFrame);
  }

  {
    xiiGALDeviceEvent e;
    e.m_pDevice = this;
    e.m_Type    = xiiGALDeviceEventType::AfterBeginFrame;
    s_Events.Broadcast(e);
  }
}

void xiiGALDevice::EndFrame()
{
  {
    xiiGALDeviceEvent e;
    e.m_pDevice = this;
    e.m_Type    = xiiGALDeviceEventType::BeforeEndFrame;
    s_Events.Broadcast(e);
  }

  {
    XII_GAL_DEVICE_LOCK_AND_CHECK();

    XII_ASSERT_DEV(m_bBeginFrameCalled, "You must have called xiiGALDevice::Begin before you can call xiiGALDevice::EndFrame");

    FlushDestroyedObjects();

    EndFramePlatform();

    m_bBeginFrameCalled = false;
  }

  {
    xiiGALDeviceEvent e;
    e.m_pDevice = this;
    e.m_Type    = xiiGALDeviceEventType::AfterEndFrame;
    s_Events.Broadcast(e);
  }
}

xiiGALSwapChainHandle xiiGALDevice::CreateSwapChain(const xiiGALSwapChainCreationDescription& description)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  /// \todo GraphicsFoundation: Add swap chain description validation.

  xiiGALSwapChain* pSwapChain = CreateSwapChainPlatform(description);

  if (pSwapChain == nullptr)
  {
    return xiiGALSwapChainHandle();
  }
  else
  {
    return xiiGALSwapChainHandle(m_SwapChains.Insert(pSwapChain));
  }
}

void xiiGALDevice::DestroySwapChain(xiiGALSwapChainHandle hSwapChain)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALSwapChain* pSwapChain = nullptr;

  if (m_SwapChains.TryGetValue(hSwapChain, pSwapChain))
  {
    AddDestroyedObject(GALObjectType::SwapChain, hSwapChain);
  }
  else
  {
    xiiLog::Warning("DestroySwapChain called on invalid handle (double free?).");
  }
}

xiiGALCommandQueueHandle xiiGALDevice::CreateCommandQueue(const xiiGALCommandQueueCreationDescription& description)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  /// \todo GraphicsFoundation: Add command queue description validation.

  xiiGALCommandQueue* pCommandQueue = CreateCommandQueuePlatform(description);

  if (pCommandQueue == nullptr)
  {
    return xiiGALCommandQueueHandle();
  }
  else
  {
    return xiiGALCommandQueueHandle(m_CommandQueues.Insert(pCommandQueue));
  }
}

void xiiGALDevice::DestroyCommandQueue(xiiGALCommandQueueHandle hCommandQueue)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALCommandQueue* pCommandQueue = nullptr;

  if (m_CommandQueues.TryGetValue(hCommandQueue, pCommandQueue))
  {
    AddDestroyedObject(GALObjectType::CommandQueue, hCommandQueue);
  }
  else
  {
    xiiLog::Warning("DestroyCommandQueue called on invalid handle (double free?).");
  }
}

#define XII_VERIFY_BLEND_STATE(expression, ...)             \
  do                                                        \
  {                                                         \
    XII_ASSERT_DEV((expression), __VA_ARGS__);              \
    if (!(expression)) { return xiiGALBlendStateHandle(); } \
  } while (false)

xiiGALBlendStateHandle xiiGALDevice::CreateBlendState(const xiiGALBlendStateCreationDescription& description)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  for (xiiUInt32 i = 0U; i < description.m_RenderTargets.GetCount(); ++i)
  {
    const auto& rtDescription = description.m_RenderTargets[i];

    const bool bBlendEnable = rtDescription.m_bBlendEnable && (i == 0U || (description.m_bIndependentBlend && i > 0U));

    if (bBlendEnable)
    {
      XII_VERIFY_BLEND_STATE(rtDescription.m_SourceBlend != xiiGALBlendFactor::Undefined, "The source blend must not be xiiGALBlendFactor::Undefined.");
      XII_VERIFY_BLEND_STATE(rtDescription.m_DestinationBlend != xiiGALBlendFactor::Undefined, "The destination blend must not be xiiGALBlendFactor::Undefined.");
      XII_VERIFY_BLEND_STATE(rtDescription.m_BlendOperation != xiiGALBlendOperation::Undefined, "The blend operation must not be xiiGALBlendOperation::Undefined.");

      XII_VERIFY_BLEND_STATE(rtDescription.m_SourceBlendAlpha != xiiGALBlendFactor::Undefined, "The alpha source blend must not be xiiGALBlendFactor::Undefined.");
      XII_VERIFY_BLEND_STATE(rtDescription.m_DestinationBlendAlpha != xiiGALBlendFactor::Undefined, "The alpha destination blend must not be xiiGALBlendFactor::Undefined.");
      XII_VERIFY_BLEND_STATE(rtDescription.m_BlendOperationAlpha != xiiGALBlendOperation::Undefined, "The alpha blend operation must not be xiiGALBlendOperation::Undefined.");
    }
  }

  // Hash description and return any existing one (including increasing the refcount).
  xiiUInt32 uiHash = description.CalculateHash();

  {
    xiiGALBlendStateHandle hBlendState;
    if (m_BlendStateTable.TryGetValue(uiHash, hBlendState))
    {
      xiiGALBlendState* pBlendState = m_BlendStates[hBlendState];
      if (pBlendState->GetRefCount() == 0)
      {
        ReviveDestroyedObject(GALObjectType::BlendState, hBlendState);
      }

      pBlendState->AddRef();
      return hBlendState;
    }
  }

  xiiGALBlendState* pBlendState = CreateBlendStatePlatform(description);

  if (pBlendState != nullptr)
  {
    XII_ASSERT_DEBUG(pBlendState->GetDescription().CalculateHash() == uiHash, "BlendState hash does not match.");

    pBlendState->AddRef();

    xiiGALBlendStateHandle hBlendState(m_BlendStates.Insert(pBlendState));
    m_BlendStateTable.Insert(uiHash, hBlendState);

    return hBlendState;
  }

  return xiiGALBlendStateHandle();
}

void xiiGALDevice::DestroyBlendState(xiiGALBlendStateHandle hBlendState)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALBlendState* pBlendState = nullptr;

  if (m_BlendStates.TryGetValue(hBlendState, pBlendState))
  {
    pBlendState->ReleaseRef();

    if (pBlendState->GetRefCount() == 0)
    {
      AddDestroyedObject(GALObjectType::BlendState, hBlendState);
    }
  }
  else
  {
    xiiLog::Warning("DestroyBlendState called on an invalid handle (double free?).");
  }
}

#undef XII_VERIFY_BLEND_STATE

#define XII_VERIFY_DEPTH_STENCIL_STATE(expression, ...)            \
  do                                                               \
  {                                                                \
    XII_ASSERT_DEV((expression), __VA_ARGS__);                     \
    if (!(expression)) { return xiiGALDepthStencilStateHandle(); } \
  } while (false)

xiiGALDepthStencilStateHandle xiiGALDevice::CreateDepthStencilState(const xiiGALDepthStencilStateCreationDescription& description)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  XII_VERIFY_DEPTH_STENCIL_STATE(!(description.m_bDepthEnable && description.m_ComparisonDepthFunction == xiiGALComparisonFunction::Unknown), "The depth comparison function must not be xiiGALComparisonFunction::Unknown when depth is enabled.");

  if (description.m_bStencilEnable)
  {
    XII_VERIFY_DEPTH_STENCIL_STATE(description.m_FrontFace.m_StencilFailOperation != xiiGALStencilOperation::Undefined, "The front face stencil fail operation must not be xiiGALStencilOperation::Undefined when stencil is enabled.");
    XII_VERIFY_DEPTH_STENCIL_STATE(description.m_FrontFace.m_StencilDepthFailOperation != xiiGALStencilOperation::Undefined, "The front face stencil depth fail operation must not be xiiGALStencilOperation::Undefined when stencil is enabled.");
    XII_VERIFY_DEPTH_STENCIL_STATE(description.m_FrontFace.m_StencilPassOperation != xiiGALStencilOperation::Undefined, "The front face stencil pass operation must not be xiiGALStencilOperation::Undefined when stencil is enabled.");
    XII_VERIFY_DEPTH_STENCIL_STATE(description.m_FrontFace.m_ComparisonFunction != xiiGALComparisonFunction::Unknown, "The front face stencil comparison function must not be xiiGALComparisonFunction::Unknown when stencil is enabled.");

    XII_VERIFY_DEPTH_STENCIL_STATE(description.m_BackFace.m_StencilFailOperation != xiiGALStencilOperation::Undefined, "The back face stencil fail operation must not be xiiGALStencilOperation::Undefined when stencil is enabled.");
    XII_VERIFY_DEPTH_STENCIL_STATE(description.m_BackFace.m_StencilDepthFailOperation != xiiGALStencilOperation::Undefined, "The back face stencil depth fail operation must not be xiiGALStencilOperation::Undefined when stencil is enabled.");
    XII_VERIFY_DEPTH_STENCIL_STATE(description.m_BackFace.m_StencilPassOperation != xiiGALStencilOperation::Undefined, "The back face stencil pass operation must not be xiiGALStencilOperation::Undefined when stencil is enabled.");
    XII_VERIFY_DEPTH_STENCIL_STATE(description.m_BackFace.m_ComparisonFunction != xiiGALComparisonFunction::Unknown, "The back face stencil comparison function must not be xiiGALComparisonFunction::Unknown when stencil is enabled.");
  }

  // Hash description and return any existing one (including increasing the refcount).
  xiiUInt32 uiHash = description.CalculateHash();

  {
    xiiGALDepthStencilStateHandle hDepthStencilState;
    if (m_DepthStencilStateTable.TryGetValue(uiHash, hDepthStencilState))
    {
      xiiGALDepthStencilState* pDepthStencilState = m_DepthStencilStates[hDepthStencilState];
      if (pDepthStencilState->GetRefCount() == 0)
      {
        ReviveDestroyedObject(GALObjectType::DepthStencilState, hDepthStencilState);
      }

      pDepthStencilState->AddRef();
      return hDepthStencilState;
    }
  }

  xiiGALDepthStencilState* pDepthStencilState = CreateDepthStencilStatePlatform(description);

  if (pDepthStencilState != nullptr)
  {
    XII_ASSERT_DEBUG(pDepthStencilState->GetDescription().CalculateHash() == uiHash, "DepthStencilState hash does not match.");

    pDepthStencilState->AddRef();

    xiiGALDepthStencilStateHandle hDepthStencilState(m_DepthStencilStates.Insert(pDepthStencilState));
    m_DepthStencilStateTable.Insert(uiHash, hDepthStencilState);

    return hDepthStencilState;
  }

  return xiiGALDepthStencilStateHandle();
}

void xiiGALDevice::DestroyDepthStencilState(xiiGALDepthStencilStateHandle hDepthStencilState)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALDepthStencilState* pDepthStencilState = nullptr;

  if (m_DepthStencilStates.TryGetValue(hDepthStencilState, pDepthStencilState))
  {
    pDepthStencilState->ReleaseRef();

    if (pDepthStencilState->GetRefCount() == 0)
    {
      AddDestroyedObject(GALObjectType::DepthStencilState, hDepthStencilState);
    }
  }
  else
  {
    xiiLog::Warning("DestroyDepthStencilState called on an invalid handle (double free?).");
  }
}

#undef XII_VERIFY_DEPTH_STENCIL_STATE

#define XII_VERIFY_RASTERIZER_STATE(expression, ...)             \
  do                                                             \
  {                                                              \
    XII_ASSERT_DEV((expression), __VA_ARGS__);                   \
    if (!(expression)) { return xiiGALRasterizerStateHandle(); } \
  } while (false)

xiiGALRasterizerStateHandle xiiGALDevice::CreateRasterizerState(const xiiGALRasterizerStateCreationDescription& description)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  XII_VERIFY_RASTERIZER_STATE(description.m_FillMode != xiiGALFillMode::Undefined, "The fill mode cannot be xiiGALFillMode::Undefined.");
  XII_VERIFY_RASTERIZER_STATE(description.m_CullMode != xiiGALCullMode::Undefined, "The cull mode cannot be xiiGALCullMode::Undefined.");

  // Hash description and return any existing one (including increasing the refcount).
  xiiUInt32 uiHash = description.CalculateHash();

  {
    xiiGALRasterizerStateHandle hRasterizerState;
    if (m_RasterizerStateTable.TryGetValue(uiHash, hRasterizerState))
    {
      xiiGALRasterizerState* pRasterizerState = m_RasterizerStates[hRasterizerState];
      if (pRasterizerState->GetRefCount() == 0)
      {
        ReviveDestroyedObject(GALObjectType::RasterizerState, hRasterizerState);
      }

      pRasterizerState->AddRef();
      return hRasterizerState;
    }
  }

  xiiGALRasterizerState* pRasterizerState = CreateRasterizerStatePlatform(description);

  if (pRasterizerState != nullptr)
  {
    XII_ASSERT_DEBUG(pRasterizerState->GetDescription().CalculateHash() == uiHash, "RasterizerState hash does not match.");

    pRasterizerState->AddRef();

    xiiGALRasterizerStateHandle hRasterizerState(m_RasterizerStates.Insert(pRasterizerState));
    m_RasterizerStateTable.Insert(uiHash, hRasterizerState);

    return hRasterizerState;
  }

  return xiiGALRasterizerStateHandle();
}

void xiiGALDevice::DestroyRasterizerState(xiiGALRasterizerStateHandle hRasterizerState)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALRasterizerState* pRasterizerState = nullptr;

  if (m_RasterizerStates.TryGetValue(hRasterizerState, pRasterizerState))
  {
    pRasterizerState->ReleaseRef();

    if (pRasterizerState->GetRefCount() == 0)
    {
      AddDestroyedObject(GALObjectType::RasterizerState, hRasterizerState);
    }
  }
  else
  {
    xiiLog::Warning("DestroyRasterizerState called on an invalid handle (double free?).");
  }
}

#undef XII_VERIFY_RASTERIZER_STATE

#define XII_VERIFY_SHADER(expression, ...)              \
  do                                                    \
  {                                                     \
    XII_ASSERT_DEV((expression), __VA_ARGS__);          \
    if (!(expression)) { return xiiGALShaderHandle(); } \
  } while (false)

xiiGALShaderHandle xiiGALDevice::CreateShader(const xiiGALShaderCreationDescription& description)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  bool bHasByteCodes = false;

  for (xiiUInt32 uiStage = 0; uiStage < xiiGALShaderStage::ENUM_COUNT; ++uiStage)
  {
    if (description.HasByteCodeForStage(xiiGALShaderStage::GetStageFlag(uiStage)))
    {
      bHasByteCodes = true;
      break;
    }
  }

  if (!bHasByteCodes)
  {
    xiiLog::Error("A shader cannot be created with no shader bytecode.");

    return xiiGALShaderHandle();
  }

  if (description.m_ShaderStage.IsSet(xiiGALShaderStage::Geometry) && m_AdapterDescription.m_Features.m_GeometryShaders != xiiGALDeviceFeatureState::Enabled)
  {
    XII_VERIFY_SHADER(false, "Geometry shaders are not supported by this device.");
  }
  if (description.m_ShaderStage.IsAnySet(xiiGALShaderStage::Domain | xiiGALShaderStage::Hull) && m_AdapterDescription.m_Features.m_Tessellation != xiiGALDeviceFeatureState::Enabled)
  {
    XII_VERIFY_SHADER(false, "Tessellation shaders are not supported by this device.");
  }
  if (description.m_ShaderStage.IsSet(xiiGALShaderStage::Compute) && m_AdapterDescription.m_Features.m_ComputeShaders != xiiGALDeviceFeatureState::Enabled)
  {
    XII_VERIFY_SHADER(false, "Compute shaders are not supported by this device.");
  }
  if (description.m_ShaderStage.IsAnySet(xiiGALShaderStage::Amplification | xiiGALShaderStage::Mesh) && m_AdapterDescription.m_Features.m_MeshShaders != xiiGALDeviceFeatureState::Enabled)
  {
    XII_VERIFY_SHADER(false, "Mesh shaders are not supported by this device.");
  }
  if (description.m_ShaderStage.IsAnySet(xiiGALShaderStage::AllRayTracing))
  {
    if (m_AdapterDescription.m_Features.m_RayTracing != xiiGALDeviceFeatureState::Enabled || m_AdapterDescription.m_RayTracingProperties.m_CapabilityFlags.AreNoneSet(xiiGALRayTracingCapabilityFlags::StandaloneShaders))
    {
      XII_VERIFY_SHADER(false, "Standalone ray tracing shaders are not supported by this device.");
    }
  }
  if (description.m_ShaderStage.IsSet(xiiGALShaderStage::Tile) && m_AdapterDescription.m_Features.m_TileShaders != xiiGALDeviceFeatureState::Enabled)
  {
    XII_VERIFY_SHADER(false, "Tile shaders are not supported by this device.");
  }

  xiiGALShader* pShader = CreateShaderPlatform(description);

  if (pShader == nullptr)
  {
    return xiiGALShaderHandle();
  }
  else
  {
    return xiiGALShaderHandle(m_Shaders.Insert(pShader));
  }
}

void xiiGALDevice::DestroyShader(xiiGALShaderHandle hShader)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALShader* pShader = nullptr;

  if (m_Shaders.TryGetValue(hShader, pShader))
  {
    AddDestroyedObject(GALObjectType::Shader, hShader);
  }
  else
  {
    xiiLog::Warning("DestroyShader called on an invalid handle (double free?).");
  }
}

#undef XII_VERIFY_SHADER

#define XII_VERIFY_BUFFER(expression, ...)              \
  do                                                    \
  {                                                     \
    XII_ASSERT_DEV((expression), __VA_ARGS__);          \
    if (!(expression)) { return xiiGALBufferHandle(); } \
  } while (false)

xiiGALBufferHandle xiiGALDevice::CreateBuffer(const xiiGALBufferCreationDescription& description, const xiiGALBufferData* pInitialData /* = nullptr*/)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  // Validate buffer description.

  auto allowedBindFlags = xiiGALBindFlags::VertexBuffer | xiiGALBindFlags::IndexBuffer | xiiGALBindFlags::UniformBuffer | xiiGALBindFlags::ShaderResource | xiiGALBindFlags::StreamOutput | xiiGALBindFlags::UnorderedAccess | xiiGALBindFlags::IndirectDrawArguments | xiiGALBindFlags::RayTracing;

  XII_VERIFY_BUFFER(description.m_BindFlags.IsStrictlyAnySet(allowedBindFlags), "The buffer description bind flags contain unsupported bind flags.");

  if (description.m_BindFlags.IsAnySet(xiiGALBindFlags::ShaderResource | xiiGALBindFlags::UnorderedAccess))
  {
    XII_VERIFY_BUFFER(description.m_Mode.GetValue() > xiiGALBufferMode::Undefined && description.m_Mode.GetValue() < xiiGALBufferMode::ENUM_COUNT, "The given buffer mode is not a valid buffer mode for a buffer created with xiiGALBindFlags::ShaderResource or xiiGALBindFlags::UnorderedAccess.");

    if (description.m_Mode == xiiGALBufferMode::Structured || description.m_Mode == xiiGALBufferMode::Formatted)
    {
      XII_VERIFY_BUFFER(description.m_uiElementByteStride != 0U, "The element stride must not be zero for structured and formatted buffers.");
    }
    else if (description.m_Mode == xiiGALBufferMode::Raw)
    {
      // Nothing to do.
    }
  }

  if (description.m_BindFlags.IsSet(xiiGALBindFlags::RayTracing))
  {
    XII_VERIFY_BUFFER(m_AdapterDescription.m_Features.m_RayTracing == xiiGALDeviceFeatureState::Enabled, "xiiGALBindFlags::RayTracing flag cannot be used when the Ray Tracing feature is disabled.");
  }
  if (description.m_BindFlags.IsSet(xiiGALBindFlags::IndirectDrawArguments))
  {
    XII_VERIFY_BUFFER(m_AdapterDescription.m_DrawCommandProperties.m_CapabilityFlags.IsSet(xiiGALDrawCommandCapabilityFlags::DrawIndirect), "xiiGALBindFlags::IndirectDrawArguments flag cannot be used when the xiiGALDrawCommandCapabilityFlags::DrawIndirect capability is not supported.");
  }

  switch (description.m_ResourceUsage)
  {
    case xiiGALResourceUsage::Immutable:
    case xiiGALResourceUsage::Default:
    {
      XII_VERIFY_BUFFER(description.m_CPUAccessFlags.IsNoFlagSet(), "Static and default buffers cannot have any CPU flags set.");
    }
    break;
    case xiiGALResourceUsage::Dynamic:
    {
      XII_VERIFY_BUFFER(description.m_CPUAccessFlags == xiiGALCPUAccessFlag::Write, "Dynamic buffers require the xiiGALCPUAccessFlag::Write flag.");
    }
    break;
    case xiiGALResourceUsage::Staging:
    {
      XII_VERIFY_BUFFER(description.m_CPUAccessFlags == xiiGALCPUAccessFlag::Write || description.m_CPUAccessFlags == xiiGALCPUAccessFlag::Read, "Exactly one of xiiGALCPUAccessFlag::Write or xiiGALCPUAccessFlag::Read must be specified for a staging buffer.");
      XII_VERIFY_BUFFER(description.m_BindFlags.IsNoFlagSet(), "Staging buffers cannot be bound to any part of the graphics pipeline and cannot have any bind flags set.");
    }
    break;
    case xiiGALResourceUsage::Unified:
    {
      XII_VERIFY_BUFFER(m_AdapterDescription.m_MemoryProperties.m_uiUnifiedMemory != 0U, "Unified memory is not present in this device. Check the amount of unified memory in the device adapter information before creating unified buffers.");
      XII_VERIFY_BUFFER(description.m_CPUAccessFlags.IsAnyFlagSet(), "At least one of xiiGALCPUAccessFlag::Write or xiiGALCPUAccessFlag::Read must be specified for a unified buffer.");

      if (description.m_CPUAccessFlags.IsSet(xiiGALCPUAccessFlag::Write))
      {
        XII_VERIFY_BUFFER(m_AdapterDescription.m_MemoryProperties.m_UnifiedMemoryCPUAccessFlags.IsAnySet(xiiGALCPUAccessFlag::Write), "Unified memory on this device does not support write access. Check for available access flags in the device properties before creating unified buffers.");
      }
      if (description.m_CPUAccessFlags.IsSet(xiiGALCPUAccessFlag::Read))
      {
        XII_VERIFY_BUFFER(m_AdapterDescription.m_MemoryProperties.m_UnifiedMemoryCPUAccessFlags.IsAnySet(xiiGALCPUAccessFlag::Read), "Unified memory on this device does not support read access. Check for available access flags in the device properties before creating unified buffers.");
      }
    }
    break;
    case xiiGALResourceUsage::Sparse:
    {
      XII_VERIFY_BUFFER(m_AdapterDescription.m_Features.m_SparseResources == xiiGALDeviceFeatureState::Enabled, "Sparse buffer requires the Sparse Resources device feature to be enabled.");
      XII_VERIFY_BUFFER(description.m_CPUAccessFlags.IsNoFlagSet(), "Sparse buffers cannot have any CPU access flags set.");
      XII_VERIFY_BUFFER(description.m_uiSize <= m_AdapterDescription.m_SparseResourceProperties.m_uiResourceSpaceSize, "Sparse buffer size ({0}), must not exceed the Resource Space Size ({1}).", description.m_uiSize, m_AdapterDescription.m_SparseResourceProperties.m_uiResourceSpaceSize);
      XII_VERIFY_BUFFER(m_AdapterDescription.m_SparseResourceProperties.m_CapabilityFlags.IsSet(xiiGALSparseResourceCapabilityFlags::Buffer), "Sparse buffer requires the xiiGALSparseResourceCapabilityFlags::Buffer capability.");

      if (description.m_MiscFlags.IsSet(xiiGALMiscBufferFlags::SparseAlias))
      {
        XII_VERIFY_BUFFER(m_AdapterDescription.m_SparseResourceProperties.m_CapabilityFlags.IsSet(xiiGALSparseResourceCapabilityFlags::Aliased), "xiiGALMiscBufferFlags::SparseAlias flag requires the xiiGALSparseResourceCapabilityFlags::Aliased capability.");
      }
      XII_VERIFY_BUFFER(description.m_BindFlags.IsStrictlyAnySet(m_AdapterDescription.m_SparseResourceProperties.m_BindFlags), "The buffer description bind flags contain unsupported bind flags.");
    }
    break;

    default:
      xiiLog::Error("Unknown resource usage given.");
      return xiiGALBufferHandle();
  }

  if (description.m_ResourceUsage == xiiGALResourceUsage::Dynamic && xiiMath::CountBits(description.m_uiImmediateContextMask) > 1U)
  {
    const bool bNeedsBackingResource = (description.m_BindFlags.IsSet(xiiGALBindFlags::UnorderedAccess) || description.m_Mode == xiiGALBufferMode::Formatted);
    XII_VERIFY_BUFFER(!bNeedsBackingResource, "xiiGALResourceUsage::Dynamic buffers that use the Unordered Access flag or Formatted mode requires an internal backing resource. "
                                              "This resource is implicitly transitioned by the device context and thus cannot be relied upon to be safely used in multiple contexts. Create a xiiGALResourceUsage::Dynamic buffer "
                                              "without the xiiGALResourceUsage::UnorderedAccess flag and use xiiGALResourceUsage::Undefined mode and copy the contents to a xiiGALResourceUsage::Default buffer with required flags, "
                                              "which can be shared between device contexts.");
  }

  if (description.m_ResourceUsage != xiiGALResourceUsage::Sparse)
  {
    XII_VERIFY_BUFFER(m_AdapterDescription.m_MemoryProperties.m_uiMaxMemoryAllocation == 0U || description.m_uiSize <= m_AdapterDescription.m_MemoryProperties.m_uiMaxMemoryAllocation, "Non-sparse buffer size ({0}) must not exceed the maximum allocation size ({1}).", description.m_uiSize, m_AdapterDescription.m_MemoryProperties.m_uiMaxMemoryAllocation);
    XII_VERIFY_BUFFER(description.m_MiscFlags.AreNoneSet(xiiGALMiscBufferFlags::SparseAlias), "Miscellaneous flags must not have xiiGALMiscBufferFlags::SparseAlias if the buffer usage is not xiiGALResourceUsage::Sparse.");
  }

  // Validate buffer initial data.

  const bool bHasInitialData = (pInitialData != nullptr && pInitialData->m_pData != nullptr);

  if (description.m_ResourceUsage == xiiGALResourceUsage::Immutable && !bHasInitialData)
  {
    XII_VERIFY_BUFFER(false, "The initial data must not be nullptr, as immutable buffers must be initialized at creation time.");
  }
  if (description.m_ResourceUsage == xiiGALResourceUsage::Dynamic && bHasInitialData)
  {
    XII_VERIFY_BUFFER(false, "The initial data must be nullptr for dynamic buffers.");
  }
  if (description.m_ResourceUsage == xiiGALResourceUsage::Sparse && bHasInitialData)
  {
    XII_VERIFY_BUFFER(false, "The initial data must be nullptr for sparse buffers.");
  }

  if (description.m_ResourceUsage == xiiGALResourceUsage::Staging)
  {
    if (description.m_CPUAccessFlags.IsSet(xiiGALCPUAccessFlag::Write))
    {
      XII_VERIFY_BUFFER(bHasInitialData, "Staging buffers with CPU write access must be updated via map.");
    }
  }
  else if (description.m_ResourceUsage == xiiGALResourceUsage::Unified)
  {
    XII_VERIFY_BUFFER(description.m_CPUAccessFlags.IsSet(xiiGALCPUAccessFlag::Write) && bHasInitialData, "xiiGALCPUAccessFlag::Write is required to initialize a unified buffer.");
  }

  if (pInitialData != nullptr /* && pInitialData->m_pCommandEncoder != nullptr */)
  {
    /// \todo GraphicsFoundation: Assert that the command encoder used to initialize the resource is not a deferred context, as those cannot be used to initialize resources.
  }

  if (bHasInitialData)
  {
    XII_VERIFY_BUFFER(pInitialData->m_uiDataSize >= description.m_uiSize, "The buffer initial data size ({0}) must be larger than the buffer size ({1}).", pInitialData->m_uiDataSize, description.m_uiSize);
  }

  xiiGALBuffer* pBuffer = CreateBufferPlatform(description, pInitialData);

  return FinalizeBufferInternal(description, pBuffer);
}

xiiGALBufferHandle xiiGALDevice::FinalizeBufferInternal(const xiiGALBufferCreationDescription& description, xiiGALBuffer* pBuffer)
{
  if (pBuffer != nullptr)
  {
    xiiGALBufferHandle hBuffer(m_Buffers.Insert(pBuffer));

    pBuffer->CreateDefaultResourceViews(hBuffer);

    return hBuffer;
  }

  return xiiGALBufferHandle();
}

void xiiGALDevice::DestroyBuffer(xiiGALBufferHandle hBuffer)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALBuffer* pBuffer = nullptr;

  if (m_Buffers.TryGetValue(hBuffer, pBuffer))
  {
    AddDestroyedObject(GALObjectType::Buffer, hBuffer);
  }
  else
  {
    xiiLog::Warning("DestroyBuffer called on an invalid handle (double free?).");
  }
}

#undef XII_VERIFY_BUFFER

#define XII_VERIFY_BUFFER_VIEW(expression, ...)             \
  do                                                        \
  {                                                         \
    XII_ASSERT_DEV((expression), __VA_ARGS__);              \
    if (!(expression)) { return xiiGALBufferViewHandle(); } \
  } while (false)

xiiGALBufferViewHandle xiiGALDevice::CreateBufferView(xiiGALBufferViewCreationDescription& description)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALBuffer* pBuffer = Get<BufferTable, xiiGALBuffer>(description.m_hBuffer, m_Buffers);

  XII_VERIFY_BUFFER_VIEW(pBuffer != nullptr, "The buffer handle given for buffer view creation is invalid.");

  const auto& bufferDescription = pBuffer->GetDescription();

  if (description.m_uiByteWidth == 0U)
  {
    XII_VERIFY_BUFFER_VIEW(bufferDescription.m_uiSize > description.m_uiByteOffset, "The byte offset ({0}) exceeds the buffer size ({1}).", description.m_uiByteOffset, bufferDescription.m_uiSize);

    description.m_uiByteWidth = bufferDescription.m_uiSize - description.m_uiByteOffset;
  }

  XII_VERIFY_BUFFER_VIEW((description.m_uiByteOffset + description.m_uiByteWidth) <= bufferDescription.m_uiSize, "The buffer view range [{0}, {1}) is out of the buffer boundaries [0, {2}).", description.m_uiByteOffset, (description.m_uiByteOffset + description.m_uiByteWidth), bufferDescription.m_uiSize);

  if (bufferDescription.m_BindFlags.IsAnySet(xiiGALBindFlags::ShaderResource | xiiGALBindFlags::UnorderedAccess))
  {
    if (bufferDescription.m_Mode == xiiGALBufferMode::Structured || bufferDescription.m_Mode == xiiGALBufferMode::Formatted)
    {
      XII_VERIFY_BUFFER_VIEW(bufferDescription.m_uiElementByteStride != 0U, "The element byte stride is zero.");
      XII_VERIFY_BUFFER_VIEW((description.m_uiByteOffset % bufferDescription.m_uiElementByteStride) == 0U, "The buffer view byte offset ({0}) is not a multiple of the element byte stride ({1}).", description.m_uiByteOffset, bufferDescription.m_uiElementByteStride);
      XII_VERIFY_BUFFER_VIEW((description.m_uiByteWidth % bufferDescription.m_uiElementByteStride) == 0U, "The buffer view byte width ({0}) is not a multiple of the element byte stride ({1}).", description.m_uiByteWidth, bufferDescription.m_uiElementByteStride);
    }

    XII_VERIFY_BUFFER_VIEW(!(bufferDescription.m_Mode == xiiGALBufferMode::Formatted && description.m_Format.m_ValueType == xiiGALValueType::Undefined), "The format must be specified when creating a view of a formatted buffer.");

    if (bufferDescription.m_Mode == xiiGALBufferMode::Formatted || (bufferDescription.m_Mode == xiiGALBufferMode::Raw && description.m_Format.m_ValueType != xiiGALValueType::Undefined))
    {
      XII_VERIFY_BUFFER_VIEW(description.m_Format.m_uiComponents > 0U && description.m_Format.m_uiComponents <= 4U, "Incorrect number of format components ({0}). 1, 2, 3, or 4 are allowed values.");

      if (description.m_Format.m_ValueType == xiiGALValueType::Float16 || description.m_Format.m_ValueType == xiiGALValueType::Float32)
        description.m_Format.m_bIsNormalized = false;

      XII_VERIFY_BUFFER_VIEW(bufferDescription.m_Mode != xiiGALBufferMode::Raw && bufferDescription.m_uiElementByteStride != 0U, "To enable formatted views of a raw buffer, the element byte stride must be specified in the buffer creation description.");

      const xiiUInt32 uiViewElementStride = xiiGALValueType::GetSize(description.m_Format.m_ValueType) * description.m_Format.m_uiComponents;

      XII_VERIFY_BUFFER_VIEW(bufferDescription.m_uiElementByteStride == uiViewElementStride, "The buffer element byte stride ({0}) is not consistent with the size ({1}) defined by the format value type of the view ({2}).", bufferDescription.m_uiElementByteStride, uiViewElementStride, description.m_Format.m_ValueType);
    }

    if (bufferDescription.m_Mode == xiiGALBufferMode::Raw && description.m_Format.m_ValueType == xiiGALValueType::Undefined)
    {
      XII_VERIFY_BUFFER_VIEW((description.m_uiByteOffset % 16U) == 0U, "When creating a Raw buffer view, the offset of the first element from the start of the buffer ({0}) must be a multiple of 16 bytes.", description.m_uiByteOffset);
    }

    if (bufferDescription.m_Mode == xiiGALBufferMode::Structured)
    {
      const xiiUInt32 uiStructuredBufferOffsetAlignment = m_AdapterDescription.m_BufferProperties.m_uiStructuredBufferOffsetAlignment;

      XII_VERIFY_BUFFER_VIEW(uiStructuredBufferOffsetAlignment != 0, "Device structured buffer offset alignment may not have been initialized.");
      XII_VERIFY_BUFFER_VIEW((description.m_uiByteOffset % uiStructuredBufferOffsetAlignment) == 0U, "Structured buffer view byte offset ({0}) is not a multiple of the required structured buffer offset alignment ({1}).", description.m_uiByteOffset, uiStructuredBufferOffsetAlignment);
    }
  }

  // Hash description and return any existing one.
  xiiUInt32 uiHash = description.CalculateHash();

  {
    xiiGALBufferViewHandle hBufferView;
    if (pBuffer->m_BufferViews.TryGetValue(uiHash, hBufferView))
    {
      return hBufferView;
    }
  }

  xiiGALBufferView* pBufferView = CreateBufferViewPlatform(pBuffer, description);

  if (pBufferView != nullptr)
  {
    XII_ASSERT_DEBUG(pBufferView->GetDescription().CalculateHash() == uiHash, "BufferView hash does not match.");

    pBufferView->AddRef();

    xiiGALBufferViewHandle hBufferView(m_BufferViews.Insert(pBufferView));
    pBuffer->m_BufferViews.Insert(uiHash, hBufferView);

    return hBufferView;
  }

  return xiiGALBufferViewHandle();
}

void xiiGALDevice::DestroyBufferView(xiiGALBufferViewHandle hBufferView)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALBufferView* pBufferView = nullptr;

  if (m_BufferViews.TryGetValue(hBufferView, pBufferView))
  {
    AddDestroyedObject(GALObjectType::BufferView, hBufferView);
  }
  else
  {
    xiiLog::Warning("DestroyBufferView called on an invalid handle (double free?).");
  }
}

#undef XII_VERIFY_BUFFER_VIEW

#define XII_VERIFY_TEXTURE(expression, ...)              \
  do                                                     \
  {                                                      \
    XII_ASSERT_DEV((expression), __VA_ARGS__);           \
    if (!(expression)) { return xiiGALTextureHandle(); } \
  } while (false)

xiiGALTextureHandle xiiGALDevice::CreateTexture(const xiiGALTextureCreationDescription& description, const xiiGALTextureData* pInitialData /* = nullptr*/)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  const auto& formatProperties = xiiGALGraphicsUtilities::GetTextureFormatProperties(description.m_Format);

  // Validate texture description.

  XII_VERIFY_TEXTURE(description.m_Type != xiiGALResourceDimension::Undefined, "The texture resource dimension is undefined.");
  XII_VERIFY_TEXTURE(description.m_Type > xiiGALResourceDimension::Undefined && description.m_Type < xiiGALResourceDimension::ENUM_COUNT, "The texture resource dimension is invalid.");
  XII_VERIFY_TEXTURE(description.m_Size.width != 0U, "The texture width cannot be zero.");

  if (description.m_Type == xiiGALResourceDimension::Texture1D || description.m_Type == xiiGALResourceDimension::Texture1DArray)
  {
    if (description.m_Size.height != formatProperties.m_uiBlockHeight)
    {
      if (formatProperties.m_uiBlockHeight == 1U)
      {
        XII_VERIFY_TEXTURE(false, "The texture height ({0}) of a Texture1D or Texture1DArray must be equal to 1.");
      }
      else
      {
        XII_VERIFY_TEXTURE(false, "For block-compressed formats, the height ({0}) of a Texture1D or Texture1DArray must be equal to the compressed block height ({1}).", description.m_Size.height, formatProperties.m_uiBlockHeight);
      }
    }
  }
  else
  {
    XII_VERIFY_TEXTURE(description.m_Size.height != 0U, "The texture height cannot be zero.");
  }

  XII_VERIFY_TEXTURE(description.m_Type != xiiGALResourceDimension::Texture3D && description.m_uiArraySizeOrDepth != 0U, "A 3D texture depth cannot be zero.");

  if (description.m_Type == xiiGALResourceDimension::Texture1D || description.m_Type == xiiGALResourceDimension::Texture2D)
  {
    XII_VERIFY_TEXTURE(description.m_uiArraySizeOrDepth == 1U, "A Texture1D or Texture2D must have 1 array slice, ({0}) provided. Use Texture1DArray or Texture2DArray if more than one slice is needed.", description.m_uiArraySizeOrDepth);
  }

  if (description.m_Type == xiiGALResourceDimension::TextureCube || description.m_Type == xiiGALResourceDimension::TextureCubeArray)
  {
    XII_VERIFY_TEXTURE(description.m_Size.width == description.m_Size.height, "For TextureCube or TextureCubeArray textures, the width ({0} provided) must match the height ({1} provided).", description.m_Size.width, description.m_Size.height);
    XII_VERIFY_TEXTURE(description.m_uiArraySizeOrDepth >= 6U, "For TextureCube or TextureCubeArray textures, a minimum of 6 slices must be given ({0} provided).", description.m_uiArraySizeOrDepth);
  }

#if XII_ENABLED(XII_COMPILE_FOR_DEVELOPMENT)
  {
    xiiUInt32 uiMaxDimension = 0;
    if (description.Is1D())
      uiMaxDimension = description.m_Size.width;
    else if (description.Is2D())
      uiMaxDimension = xiiMath::Max(description.m_Size.width, description.m_Size.height);
    else if (description.Is3D())
      uiMaxDimension = xiiMath::Max(xiiMath::Max(description.m_Size.width, description.m_Size.height), description.m_uiArraySizeOrDepth);

    XII_VERIFY_TEXTURE((uiMaxDimension >= XII_BIT(description.m_uiMipLevels - 1)), "The texture has an incorrect number of Mip levels ({0}).", description.m_uiMipLevels);
  }
#endif

  if (description.m_uiSampleCount > 1U)
  {
    XII_VERIFY_TEXTURE(xiiMath::IsPowerOf2(description.m_uiSampleCount), "The texture sample count must be a power of two.");
    XII_VERIFY_TEXTURE(description.m_Type == xiiGALResourceDimension::Texture2D || description.m_Type == xiiGALResourceDimension::Texture2DArray, "Only Texture2D and Texture2DArray can be multi-sampled.");
    XII_VERIFY_TEXTURE(description.m_uiMipLevels == 1U, "Multi-sampled textures must have one mip level ({0} levels specified).", description.m_uiMipLevels);
    XII_VERIFY_TEXTURE(description.m_BindFlags.AreNoneSet(xiiGALBindFlags::UnorderedAccess), "xiiGALBindFlags::UnorderedAccess is not allowed for multi-sampled resources.");
  }

  if (description.m_MiscFlags.IsSet(xiiGALMiscTextureFlags::Memoryless))
  {
    XII_VERIFY_TEXTURE(m_AdapterDescription.m_MemoryProperties.m_MemorylessTextureBindFlags.IsAnyFlagSet(), "Memoryless textures are not supported by this device.");
    XII_VERIFY_TEXTURE(description.m_BindFlags.IsStrictlyAnySet(m_AdapterDescription.m_MemoryProperties.m_MemorylessTextureBindFlags), "Unsupported bind flags given for memoryless textures.");
    XII_VERIFY_TEXTURE(description.m_Usage == xiiGALResourceUsage::Default, "Memoryless attachment requires xiiGALResourceUsage::Default.");
    XII_VERIFY_TEXTURE(description.m_CPUAccessFlags.IsNoFlagSet(), "Memoryless attachment requires xiiGALCPUAccessFlags::None.");
    XII_VERIFY_TEXTURE(description.m_MiscFlags.AreNoneSet(xiiGALMiscTextureFlags::GenerateMips), "Memoryless attachment is not compatible with mip map generation.");
  }

  if (description.m_Usage == xiiGALResourceUsage::Staging)
  {
    XII_VERIFY_TEXTURE(description.m_BindFlags.IsNoFlagSet(), "Staging textures cannot be bound to any GPU pipeline stage.");
    XII_VERIFY_TEXTURE(!description.m_MiscFlags.IsSet(xiiGALMiscTextureFlags::GenerateMips), "Mipmaps cannot be automatically generated for staging textures.");
    XII_VERIFY_TEXTURE(description.m_CPUAccessFlags.IsAnyFlagSet(), "Staging textures must specify the CPU access flags.");
    XII_VERIFY_TEXTURE(description.m_CPUAccessFlags.IsStrictlyAnySet(xiiGALCPUAccessFlag::Read) || description.m_CPUAccessFlags.IsStrictlyAnySet(xiiGALCPUAccessFlag::Write), "Staging textures must use exactly one of xiiGALCPUAccessFlags::Read or xiiGALCPUAccessFlags::Write.");
  }
  else if (description.m_Usage == xiiGALResourceUsage::Unified)
  {
    XII_VERIFY_TEXTURE(false, "xiiGALResourceUsage::Unified textures are currently not supported.");
  }

  if (description.m_Usage == xiiGALResourceUsage::Dynamic && xiiMath::CountBits(description.m_uiImmediateContextMask) > 1U)
  {
    // Dynamic textures always use a backing resource that requires implicit state transitions in map/unmap operations, which is not safe in multiple device contexts.
    XII_VERIFY_TEXTURE(false, "xiiGALResourceUsage::Dynamic textures may only be used in one immediate device context.");
  }

  if (description.m_MiscFlags.IsSet(xiiGALMiscTextureFlags::Subsampled))
  {
    XII_VERIFY_TEXTURE(m_AdapterDescription.m_Features.m_VariableRateShading == xiiGALDeviceFeatureState::Enabled, "xiiGALMiscTextureFlags::Subsampled requires the Variable Shading Rate device feature.");
    XII_VERIFY_TEXTURE(m_Description.m_GraphicsDeviceType != xiiGALGraphicsDeviceType::Metal, "xiiGALMiscTextureFlags::Subsampled is unsupported in Metal. Use IRasterizationRateMapMtl to implement Variable Rate Shading in Metal.");
    XII_VERIFY_TEXTURE(m_AdapterDescription.m_ShadingRateProperties.m_CapabilityFlags.IsSet(xiiGALShadingRateCapabilityFlags::SubSampledRenderTarget), "xiiGALMiscTextureFlags::Subsampled requires the xiiGALShadingRateCapabilityFlags::SubSampledRenderTarget capability.");
    XII_VERIFY_TEXTURE(!description.m_BindFlags.AreAllSet(xiiGALBindFlags::RenderTarget | xiiGALBindFlags::DepthStencil), "Subsampled textures must use one of xiiGALBindFlags::RenderTarget or xiiGALBindFlags::DepthStencil bind flags.");
    XII_VERIFY_TEXTURE(!description.m_BindFlags.IsSet(xiiGALBindFlags::ShadingRate), "xiiGALMiscTextureFlags::Subsampled is not compatible with xiiGALBindFlags::ShadingRate.");
  }

  if (description.m_BindFlags.IsSet(xiiGALBindFlags::ShadingRate))
  {
    XII_VERIFY_TEXTURE(m_AdapterDescription.m_Features.m_VariableRateShading == xiiGALDeviceFeatureState::Enabled, "xiiGALBindFlags::ShadingRate requires the Variable Shading Rate device feature.");
    XII_VERIFY_TEXTURE(m_Description.m_GraphicsDeviceType != xiiGALGraphicsDeviceType::Metal, "xiiGALBindFlags::ShadingRate is unsupported in Metal. Use IRasterizationRateMapMtl to implement Variable Rate Shading in Metal.");
    XII_VERIFY_TEXTURE(m_AdapterDescription.m_ShadingRateProperties.m_CapabilityFlags.IsSet(xiiGALShadingRateCapabilityFlags::TextureBased), "xiiGALBindFlags::ShadingRate requires the xiiGALShadingRateCapabilityFlags::TextureBased capability.");
    XII_VERIFY_TEXTURE(description.m_uiSampleCount == 1U, "xiiGALBindFlags::ShadingRate is not allowed for multi-sampled textures.");

    if (description.m_Type == xiiGALResourceDimension::Texture2DArray && description.m_uiArraySizeOrDepth > 1U)
    {
      XII_VERIFY_TEXTURE(m_AdapterDescription.m_ShadingRateProperties.m_CapabilityFlags.IsSet(xiiGALShadingRateCapabilityFlags::TextureArray), "Shading rate texture arrays require the xiiGALShadingRateCapabilityFlags::TextureArray capability.");
    }

    XII_VERIFY_TEXTURE(description.m_Usage == xiiGALResourceUsage::Default || description.m_Usage == xiiGALResourceUsage::Immutable, "Shading rate textures only allow xiiGALResourceUsage::Default or xiiGALResourceUsage::Immutable.");

    XII_VERIFY_TEXTURE(description.m_uiMipLevels == 1U, "Shading rate textures must have a single mip level.");
    XII_VERIFY_TEXTURE(description.m_uiMipLevels != 1U, "Shading rate textures must have a single mip level."); // For Direct3D12 and Vulkan with VK_EXT_fragment_density_map
    XII_VERIFY_TEXTURE(description.m_BindFlags.IsStrictlyAnySet(m_AdapterDescription.m_ShadingRateProperties.m_BindFlags), "Unsupported bind flags are specified for the shading rate texture.");

    /// \todo GraphicsFoundation: Vulkan allows the creation of 2D texture arrays, and using a single slice for view even if xiiGALShadingRateCapabilityFlags::TextureArray is not supported by the device.
    if (description.m_Type != xiiGALResourceDimension::Texture2D && !(description.m_Type == xiiGALResourceDimension::Texture2DArray && m_AdapterDescription.m_ShadingRateProperties.m_CapabilityFlags.IsSet(xiiGALShadingRateCapabilityFlags::TextureArray)))
    {
      XII_VERIFY_TEXTURE(false, "Shading rate texture must be Texture2D or Texture2DArray with the xiiGALShadingRateCapabilityFlags::TextureArray capability.");
    }

    switch (m_AdapterDescription.m_ShadingRateProperties.m_Format)
    {
      case xiiGALShadingRateFormat::Palette:
      {
        XII_VERIFY_TEXTURE(description.m_Format == xiiGALTextureFormat::R8UInt, "The shading rate texture format must be xiiGALTextureFormat::R8UInt.");
      }
      break;
      case xiiGALShadingRateFormat::RG8UNormalized:
      {
        XII_VERIFY_TEXTURE(description.m_Format == xiiGALTextureFormat::R8UNormalized, "The shading rate texture format must be xiiGALTextureFormat::R8UNormalized.");
      }
      break;

      case xiiGALShadingRateFormat::ColumnRowFloat32:
      default:
        XII_VERIFY_TEXTURE(false, "The shading rate texture is not supported.");
    }
  }

  if (description.m_Usage == xiiGALResourceUsage::Sparse)
  {
    XII_VERIFY_TEXTURE(m_AdapterDescription.m_Features.m_SparseResources == xiiGALDeviceFeatureState::Enabled, "Sparse texture requires the Sparse Resources device feature.");

    if (description.m_MiscFlags.IsSet(xiiGALMiscTextureFlags::SparseAlias))
    {
      XII_VERIFY_TEXTURE(m_AdapterDescription.m_SparseResourceProperties.m_CapabilityFlags.AreNoneSet(xiiGALSparseResourceCapabilityFlags::Aliased), "xiiGALMiscTextureFlags::SparseAlias flag requires the xiiGALSparseResourceCapabilityFlags::Aliased capability.");
    }

    switch (description.m_Type)
    {
      case xiiGALResourceDimension::Texture2D:
      {
        XII_VERIFY_TEXTURE(m_AdapterDescription.m_SparseResourceProperties.m_CapabilityFlags.IsSet(xiiGALSparseResourceCapabilityFlags::Texture2D), "Texture2D requires the xiiGALSparseResourceCapabilityFlags::Texture2D capability.");
      }
      break;
      case xiiGALResourceDimension::Texture2DArray:
      case xiiGALResourceDimension::TextureCube:
      case xiiGALResourceDimension::TextureCubeArray:
      {
        XII_VERIFY_TEXTURE(m_AdapterDescription.m_SparseResourceProperties.m_CapabilityFlags.IsSet(xiiGALSparseResourceCapabilityFlags::Texture2D), "Texture2DArray, Texture2DCube and TextureCubeArray requires the xiiGALSparseResourceCapabilityFlags::Texture2D capability.");

        if (m_AdapterDescription.m_SparseResourceProperties.m_CapabilityFlags.AreNoneSet(xiiGALSparseResourceCapabilityFlags::Texture2DArrayMipTail))
        {
          /// \todo GraphicsFoundation: Validate texture mip size to the tile size property.
        }
      }
      break;
      case xiiGALResourceDimension::Texture3D:
      {
        XII_VERIFY_TEXTURE(m_AdapterDescription.m_SparseResourceProperties.m_CapabilityFlags.IsSet(xiiGALSparseResourceCapabilityFlags::Texture3D), "Texture3D requires the xiiGALSparseResourceCapabilityFlags::Texture3D capability.");
      }
      break;
      case xiiGALResourceDimension::Texture1D:
      case xiiGALResourceDimension::Texture1DArray:
      {
        XII_VERIFY_TEXTURE(false, "Texture1D and Texture1DArray sparse textures are not supported.");
      }
      break;

        XII_DEFAULT_CASE_NOT_IMPLEMENTED;
    }
  }
  else
  {
    XII_VERIFY_TEXTURE(description.m_MiscFlags.AreNoneSet(xiiGALMiscTextureFlags::SparseAlias), "The miscellaneous flags must not have xiiGALMiscTextureFlags::SparseAlias if the usage is not xiiGALResourceUsage::Sparse.");
  }

  xiiGALTexture* pTexture = CreateTexturePlatform(description, pInitialData);

  return FinalizeTextureInternal(description, pTexture);
}

xiiGALTextureHandle xiiGALDevice::FinalizeTextureInternal(const xiiGALTextureCreationDescription& description, xiiGALTexture* pTexture)
{
  if (pTexture != nullptr)
  {
    xiiGALTextureHandle hTexture(m_Textures.Insert(pTexture));

    pTexture->CreateDefaultResourceViews(hTexture);

    return hTexture;
  }

  return xiiGALTextureHandle();
}

void xiiGALDevice::DestroyTexture(xiiGALTextureHandle hTexture)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALTexture* pTexture = nullptr;
  if (m_Textures.TryGetValue(hTexture, pTexture))
  {
    AddDestroyedObject(GALObjectType::Texture, hTexture);
  }
  else
  {
    xiiLog::Warning("DestroyTexture called on invalid handle (double free?)");
  }
}

#undef XII_VERIFY_TEXTURE

#define XII_VERIFY_TEXTURE_VIEW(expression, ...)             \
  do                                                         \
  {                                                          \
    XII_ASSERT_DEV((expression), __VA_ARGS__);               \
    if (!(expression)) { return xiiGALTextureViewHandle(); } \
  } while (false)

xiiGALTextureViewHandle xiiGALDevice::CreateTextureView(xiiGALTextureViewCreationDescription& description)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALTexture* pTexture = Get<TextureTable, xiiGALTexture>(description.m_hTexture, m_Textures);

  XII_VERIFY_TEXTURE_VIEW(pTexture != nullptr, "The texture handle given for texture view creation is invalid.");

  const auto& textureDescription = pTexture->GetDescription();

  XII_VERIFY_TEXTURE_VIEW(description.m_ViewType > xiiGALTextureViewType::Undefined && description.m_ViewType < xiiGALTextureViewType::ENUM_COUNT, "The texture view type is invalid.");
  XII_VERIFY_TEXTURE_VIEW(description.m_uiMostDetailedMip < textureDescription.m_uiMipLevels, "The most detailed mip ({0}) is out of range. The texture has only {1} mip level (s).", description.m_uiMostDetailedMip, textureDescription.m_uiMipLevels);
  XII_VERIFY_TEXTURE_VIEW((description.m_uiMostDetailedMip + description.m_uiMipLevelCount) <= textureDescription.m_uiMipLevels, "The most detailed mip ({0}) and the number of mip levels in the view ({1}) is out of range. The texture has only {2} mip level (s).", description.m_uiMostDetailedMip, description.m_uiMipLevelCount, textureDescription.m_uiMipLevels);

  /// \todo GraphicsFoundation: Implement default texture view format deduction.
  if (description.m_Format == xiiGALTextureFormat::Unknown)
  {
    description.m_Format = xiiGALGraphicsUtilities::GetDefaultTextureViewFormat(textureDescription.m_Format, description.m_ViewType, textureDescription.m_BindFlags);
  }

  if (textureDescription.IsArray())
  {
    XII_VERIFY_TEXTURE_VIEW(description.m_uiFirstArrayOrDepthSlice < textureDescription.m_uiArraySizeOrDepth, "The first array slice ({0}) is out of range. The texture has only ({1}) slice (s)", description.m_uiFirstArrayOrDepthSlice, textureDescription.m_uiArraySizeOrDepth);
    XII_VERIFY_TEXTURE_VIEW((description.m_uiFirstArrayOrDepthSlice + description.m_uiArrayOrDepthSlicesCount) <= textureDescription.m_uiArraySizeOrDepth, "The first array slice ({0}) and the number of array slice (s) ({1}) are out of range. The texture has only ({2}) slice (s)", description.m_uiFirstArrayOrDepthSlice, description.m_uiArrayOrDepthSlicesCount, textureDescription.m_uiArraySizeOrDepth);
  }
  else if (!textureDescription.Is3D())
  {
    XII_VERIFY_TEXTURE_VIEW(description.m_uiFirstArrayOrDepthSlice == 0U, "For non-array texture, the First Array or Depth Slice must be zero.");
  }

  if (description.m_ResourceDimension == xiiGALResourceDimension::Undefined)
  {
    if (textureDescription.m_Type == xiiGALResourceDimension::TextureCube || textureDescription.m_Type == xiiGALResourceDimension::TextureCubeArray)
    {
      switch (description.m_ViewType)
      {
        case xiiGALTextureViewType::ShaderResource:
          description.m_ResourceDimension = textureDescription.m_Type;
          break;

        case xiiGALTextureViewType::RenderTarget:
        case xiiGALTextureViewType::DepthStencil:
        case xiiGALTextureViewType::UnorderedAccess:
          description.m_ResourceDimension = xiiGALResourceDimension::Texture2DArray;
          break;

        default:
          XII_VERIFY_TEXTURE_VIEW(false, "Unexpected view type.");
      }
    }
    else
    {
      description.m_ResourceDimension = textureDescription.m_Type;
    }
  }

  switch (textureDescription.m_Type)
  {
    case xiiGALResourceDimension::Texture1D:
    {
      const bool bIsValid = description.m_ResourceDimension == xiiGALResourceDimension::Texture1D;
      XII_VERIFY_TEXTURE_VIEW(bIsValid, "Incorrect texture view type for a Texture 1D view, only xiiGALResourceDimension Texture1D is allowed.");
    }
    break;
    case xiiGALResourceDimension::Texture1DArray:
    {
      const bool bIsValid = description.m_ResourceDimension == xiiGALResourceDimension::Texture1D || description.m_ResourceDimension == xiiGALResourceDimension::Texture1DArray;
      XII_VERIFY_TEXTURE_VIEW(bIsValid, "Incorrect texture view type for a Texture 1D Array view, only xiiGALResourceDimension Texture1D or Texture1DArray is allowed.");
    }
    break;
    case xiiGALResourceDimension::Texture2D:
    {
      const bool bIsValid = description.m_ResourceDimension == xiiGALResourceDimension::Texture2D || description.m_ResourceDimension == xiiGALResourceDimension::Texture2DArray;
      XII_VERIFY_TEXTURE_VIEW(bIsValid, "Incorrect texture view type for a Texture 2D view, only xiiGALResourceDimension Texture2D or Texture2DArray is allowed.");
    }
    break;
    case xiiGALResourceDimension::Texture2DArray:
    {
      const bool bIsValid = description.m_ResourceDimension == xiiGALResourceDimension::Texture2D || description.m_ResourceDimension == xiiGALResourceDimension::Texture2DArray;
      XII_VERIFY_TEXTURE_VIEW(bIsValid, "Incorrect texture view type for a Texture 2D Array view, only xiiGALResourceDimension Texture2D or Texture2DArray is allowed.");
    }
    break;
    case xiiGALResourceDimension::Texture3D:
    {
      const bool bIsValid = description.m_ResourceDimension == xiiGALResourceDimension::Texture3D;
      XII_VERIFY_TEXTURE_VIEW(bIsValid, "Incorrect texture view type for a Texture 3D view, only xiiGALResourceDimension Texture3D is allowed.");
    }
    break;
    case xiiGALResourceDimension::TextureCube:
    {
      if (description.m_ViewType == xiiGALTextureViewType::ShaderResource)
      {
        const bool bIsValid = description.m_ResourceDimension == xiiGALResourceDimension::Texture2D || description.m_ResourceDimension == xiiGALResourceDimension::Texture2DArray || description.m_ResourceDimension == xiiGALResourceDimension::TextureCube;
        XII_VERIFY_TEXTURE_VIEW(bIsValid, "Incorrect texture view type for a Texture Cube shader resource view, only xiiGALResourceDimension Texture2D or Texture2DArray or TextureCube is allowed.");
      }
      else
      {
        const bool bIsValid = description.m_ResourceDimension == xiiGALResourceDimension::Texture2D || description.m_ResourceDimension == xiiGALResourceDimension::Texture2DArray;
        XII_VERIFY_TEXTURE_VIEW(bIsValid, "Incorrect texture view type for a Texture Cube non-shader resource view, only xiiGALResourceDimension Texture2D or Texture2DArray is allowed.");
      }
    }
    break;
    case xiiGALResourceDimension::TextureCubeArray:
    {
      if (description.m_ViewType == xiiGALTextureViewType::ShaderResource)
      {
        const bool bIsValid = description.m_ResourceDimension == xiiGALResourceDimension::Texture2D || description.m_ResourceDimension == xiiGALResourceDimension::Texture2DArray || description.m_ResourceDimension == xiiGALResourceDimension::TextureCube || description.m_ResourceDimension == xiiGALResourceDimension::TextureCubeArray;
        XII_VERIFY_TEXTURE_VIEW(bIsValid, "Incorrect texture view type for a Texture Cube Array shader resource view, only xiiGALResourceDimension Texture2D or Texture2DArray or TextureCube TextureCubeArray is allowed.");
      }
      else
      {
        const bool bIsValid = description.m_ResourceDimension == xiiGALResourceDimension::Texture2D || description.m_ResourceDimension == xiiGALResourceDimension::Texture2DArray;
        XII_VERIFY_TEXTURE_VIEW(bIsValid, "Incorrect texture view type for a Texture Cube Array non-shader resource view, only xiiGALResourceDimension Texture2D or Texture2DArray is allowed.");
      }
    }
    break;

    default:
      XII_VERIFY_TEXTURE_VIEW(false, "Encountered an unexpected view type.");
  }

  switch (description.m_ResourceDimension)
  {
    case xiiGALResourceDimension::TextureCube:
    {
      XII_VERIFY_TEXTURE_VIEW(description.m_ViewType == xiiGALTextureViewType::ShaderResource, "Unexpected view type, a Shader Resource view is expected.");
      XII_VERIFY_TEXTURE_VIEW(description.m_uiArrayOrDepthSlicesCount == 6U || description.m_uiArrayOrDepthSlicesCount == 0U, "Texture Cube Shader Resource view is expected to have 6 array slices. {0} are provided.", description.m_uiArrayOrDepthSlicesCount);
    }
    break;
    case xiiGALResourceDimension::TextureCubeArray:
    {
      XII_VERIFY_TEXTURE_VIEW(description.m_ViewType == xiiGALTextureViewType::ShaderResource, "Unexpected view type, a Shader Resource view is expected.");
      XII_VERIFY_TEXTURE_VIEW((description.m_uiArrayOrDepthSlicesCount % 6U) == 0U, "The number of slices in Texture Cube Array Shader Resource view is expected to be a multiple of 6. {0} are provided.", description.m_uiArrayOrDepthSlicesCount);
    }
    break;
    case xiiGALResourceDimension::Texture1D:
    case xiiGALResourceDimension::Texture2D:
    {
      XII_VERIFY_TEXTURE_VIEW(description.m_uiArrayOrDepthSlicesCount <= 1U, "The number of slices in the view ({0}) must be 1 (or 0) for non-array Texture 1D/2D views.", description.m_uiArrayOrDepthSlicesCount);
    }
    break;
    case xiiGALResourceDimension::Texture1DArray:
    case xiiGALResourceDimension::Texture2DArray:
    {
    }
    break;
    case xiiGALResourceDimension::Texture3D:
    {
      const xiiUInt32 uiMipDepth = xiiMath::Max(textureDescription.m_uiArraySizeOrDepth >> description.m_uiMostDetailedMip, 1U);

      XII_VERIFY_TEXTURE_VIEW((description.m_uiFirstArrayOrDepthSlice + description.m_uiArrayOrDepthSlicesCount) <= uiMipDepth, "The first depth slice ({0}) and the number of slices in the view description ({1}), specify more slices than the target 3D texture mip level has ({2}).", description.m_uiFirstArrayOrDepthSlice, description.m_uiArrayOrDepthSlicesCount, uiMipDepth);
    }
    break;

    default:
      XII_VERIFY_TEXTURE_VIEW(false, "Unexpected texture dimension.");
  }

  XII_VERIFY_TEXTURE_VIEW(!xiiGALGraphicsUtilities::GetTextureFormatProperties(description.m_Format).m_bIsTypeless, "The texture view format ({0}) cannot be typeless.", description.m_Format.GetValue());

  if (description.m_Flags.IsSet(xiiGALTextureViewFlags::AllowMipGeneration))
  {
    XII_VERIFY_TEXTURE_VIEW(textureDescription.m_MiscFlags.IsSet(xiiGALMiscTextureFlags::GenerateMips), "The xiiGALTextureViewFlags::AllowMipGeneration flag can only be set if the texture was created with the xiiGALMiscTextureFlags::GenerateMips flag.");
    XII_VERIFY_TEXTURE_VIEW(description.m_ViewType == xiiGALTextureViewType::ShaderResource, "The xiiGALTextureViewFlags::AllowMipGeneration flag can only used with the xiiGALTextureViewType::ShaderResource view type.");
  }

  if (description.m_ViewType == xiiGALTextureViewType::ShadingRate)
  {
    XII_VERIFY_TEXTURE_VIEW(textureDescription.m_BindFlags.IsSet(xiiGALBindFlags::ShadingRate), "To create a xiiGALTextureViewType::ShadingRate, the texture must be created with the xiiGALBindFlags::ShadingRate flag.");
  }

  if (description.m_ViewType != xiiGALTextureViewType::ShaderResource && !xiiGALGraphicsUtilities::IsIdentityComponentMapping(description.m_ComponentSwizzle))
  {
    XII_VERIFY_TEXTURE_VIEW(false, "Non-identity texture component swizzle is only supported for Shader Resource views.");
  }

  if (description.m_uiMipLevelCount == 0U)
  {
    if (description.m_ViewType == xiiGALTextureViewType::ShaderResource)
      description.m_uiMipLevelCount = textureDescription.m_uiMipLevels - description.m_uiMostDetailedMip;
    else
      description.m_uiMipLevelCount = 1U;
  }

  if (description.m_uiArrayOrDepthSlicesCount == 0)
  {
    if (textureDescription.IsArray())
    {
      description.m_uiArrayOrDepthSlicesCount = textureDescription.m_uiArraySizeOrDepth - description.m_uiFirstArrayOrDepthSlice;
    }
    else if (textureDescription.Is3D())
    {
      const xiiUInt32 uiMipDepth              = xiiMath::Max(textureDescription.m_uiArraySizeOrDepth >> description.m_uiMostDetailedMip, 1U);
      description.m_uiArrayOrDepthSlicesCount = uiMipDepth - description.m_uiFirstArrayOrDepthSlice;
    }
    else
    {
      description.m_uiArrayOrDepthSlicesCount = 1U;
    }
  }

  // Hash description and return any existing one.
  xiiUInt32 uiHash = description.CalculateHash();

  {
    xiiGALTextureViewHandle hTextureView;
    if (pTexture->m_TextureViews.TryGetValue(uiHash, hTextureView))
    {
      return hTextureView;
    }
  }

  xiiGALTextureView* pTextureView = CreateTextureViewPlatform(pTexture, description);

  if (pTextureView != nullptr)
  {
    XII_ASSERT_DEBUG(pTextureView->GetDescription().CalculateHash() == uiHash, "TextureView hash does not match.");

    pTextureView->AddRef();

    xiiGALTextureViewHandle hTextureView(m_TextureViews.Insert(pTextureView));
    pTexture->m_TextureViews.Insert(uiHash, hTextureView);

    return hTextureView;
  }

  return xiiGALTextureViewHandle();
}

void xiiGALDevice::DestroyTextureView(xiiGALTextureViewHandle hTextureView)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALTextureView* pTextureView = nullptr;

  if (m_TextureViews.TryGetValue(hTextureView, pTextureView))
  {
    AddDestroyedObject(GALObjectType::TextureView, hTextureView);
  }
  else
  {
    xiiLog::Warning("DestroyTextureView called on an invalid handle (double free?).");
  }
}

#undef XII_VERIFY_TEXTURE_VIEW

#define XII_VERIFY_SAMPLER(expression, ...)              \
  do                                                     \
  {                                                      \
    XII_ASSERT_DEV((expression), __VA_ARGS__);           \
    if (!(expression)) { return xiiGALSamplerHandle(); } \
  } while (false)

xiiGALSamplerHandle xiiGALDevice::CreateSampler(const xiiGALSamplerCreationDescription& description)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  if (description.m_Flags.AreAllSet(xiiGALSamplerFlags::Subsampled | xiiGALSamplerFlags::SubsampledCoarseReconstruction))
  {
    XII_VERIFY_SAMPLER(m_AdapterDescription.m_ShadingRateProperties.m_CapabilityFlags.IsSet(xiiGALShadingRateCapabilityFlags::SubSampledRenderTarget), "Subsampled sampler requires the xiiGALShadingRateCapabilityFlags::SubSampledRenderTarget capability.");
  }
  if (description.m_bUnormalizedCoords)
  {
    XII_VERIFY_SAMPLER(m_Description.m_GraphicsDeviceType == xiiGALGraphicsDeviceType::Vulkan || m_Description.m_GraphicsDeviceType == xiiGALGraphicsDeviceType::Metal, "Unnormalized coordinates are only supported in Vulkan and Metal.");
    XII_VERIFY_SAMPLER(description.m_MinFilter == description.m_MagFilter, "When unnormalized coordinates are enabled, the MipFilter must equal the MagFilter.");
    XII_VERIFY_SAMPLER(description.m_MipFilter == xiiGALFilterType::Point, "When unnormalized coordinates are enabled, the MipFilter must be xiiGALFilterType::Point.");
    XII_VERIFY_SAMPLER(description.m_AddressU == xiiGALTextureAddressMode::Clamp || description.m_AddressU == xiiGALTextureAddressMode::Border, "When unnormalized coordinates are enabled, the AddressU must be xiiGALTextureAddressMode::Clamp or xiiGALTextureAddressMode::Border.");
    XII_VERIFY_SAMPLER(description.m_AddressV == xiiGALTextureAddressMode::Clamp || description.m_AddressV == xiiGALTextureAddressMode::Border, "When unnormalized coordinates are enabled, the AddressV must be xiiGALTextureAddressMode::Clamp or xiiGALTextureAddressMode::Border.");
    XII_VERIFY_SAMPLER(!xiiGALFilterType::IsComparisonFilter(description.m_MinFilter), "When unnormalized coordinates are enabled, the MinFilter and MagFilter must not be of the comparison type.");
    XII_VERIFY_SAMPLER(!xiiGALFilterType::IsAnisotropicFilter(description.m_MinFilter), "When unnormalized coordinates are enabled, the MinFilter and MagFilter must not be of the anisotropic type.");
  }

  // Hash description and return any existing one (including increasing the refcount).
  xiiUInt32 uiHash = description.CalculateHash();

  {
    xiiGALSamplerHandle hSampler;
    if (m_SamplerTable.TryGetValue(uiHash, hSampler))
    {
      xiiGALSampler* pSampler = m_Samplers[hSampler];
      if (pSampler->GetRefCount() == 0)
      {
        ReviveDestroyedObject(GALObjectType::Sampler, hSampler);
      }

      pSampler->AddRef();
      return hSampler;
    }
  }

  xiiGALSampler* pSampler = CreateSamplerPlatform(description);

  if (pSampler != nullptr)
  {
    XII_ASSERT_DEBUG(pSampler->GetDescription().CalculateHash() == uiHash, "Sampler hash does not match");

    pSampler->AddRef();

    xiiGALSamplerHandle hSampler(m_Samplers.Insert(pSampler));
    m_SamplerTable.Insert(uiHash, hSampler);

    return hSampler;
  }

  return xiiGALSamplerHandle();
}

void xiiGALDevice::DestroySampler(xiiGALSamplerHandle hSampler)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALSampler* pSampler = nullptr;

  if (m_Samplers.TryGetValue(hSampler, pSampler))
  {
    pSampler->ReleaseRef();

    if (pSampler->GetRefCount() == 0)
    {
      AddDestroyedObject(GALObjectType::Sampler, hSampler);
    }
  }
  else
  {
    xiiLog::Warning("DestroySampler called on an invalid handle (double free?). ");
  }
}

#undef XII_VERIFY_SAMPLER

xiiGALInputLayoutHandle xiiGALDevice::CreateInputLayout(const xiiGALInputLayoutCreationDescription& description)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  // Hash description and return any existing one (including increasing the refcount).
  xiiUInt32 uiHash = description.CalculateHash();

  {
    xiiGALInputLayoutHandle hInputLayout;
    if (m_InputLayoutTable.TryGetValue(uiHash, hInputLayout))
    {
      xiiGALInputLayout* pInputLayout = m_InputLayouts[hInputLayout];
      if (pInputLayout->GetRefCount() == 0)
      {
        ReviveDestroyedObject(GALObjectType::InputLayout, hInputLayout);
      }

      pInputLayout->AddRef();
      return hInputLayout;
    }
  }

  xiiGALInputLayout* pInputLayout = CreateInputLayoutPlatform(description);

  if (pInputLayout != nullptr)
  {
    pInputLayout->AddRef();

    xiiGALInputLayoutHandle hInputLayout(m_InputLayouts.Insert(pInputLayout));
    m_InputLayoutTable.Insert(uiHash, hInputLayout);

    return hInputLayout;
  }

  return xiiGALInputLayoutHandle();
}

void xiiGALDevice::DestroyInputLayout(xiiGALInputLayoutHandle hInputLayout)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALInputLayout* pInputLayout = nullptr;

  if (m_InputLayouts.TryGetValue(hInputLayout, pInputLayout))
  {
    pInputLayout->ReleaseRef();

    if (pInputLayout->GetRefCount() == 0)
    {
      AddDestroyedObject(GALObjectType::InputLayout, hInputLayout);
    }
  }
  else
  {
    xiiLog::Warning("DestroyInputLayout called on an invalid handle (double free?).");
  }
}

#define XII_VERIFY_QUERY(expression, ...)              \
  do                                                   \
  {                                                    \
    XII_ASSERT_DEV((expression), __VA_ARGS__);         \
    if (!(expression)) { return xiiGALQueryHandle(); } \
  } while (false)

xiiGALQueryHandle xiiGALDevice::CreateQuery(const xiiGALQueryCreationDescription& description)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  switch (description.m_Type)
  {
    case xiiGALQueryType::Occlusion:
    {
      XII_VERIFY_QUERY(m_AdapterDescription.m_Features.m_OcclusionQueries == xiiGALDeviceFeatureState::Enabled, "Occlusion queries are not supported by this device.");
    }
    break;
    case xiiGALQueryType::BinaryOcclusion:
    {
      XII_VERIFY_QUERY(m_AdapterDescription.m_Features.m_BinaryOcclusionQueries == xiiGALDeviceFeatureState::Enabled, "Binary occlusion queries are not supported by this device.");
    }
    break;
    case xiiGALQueryType::Timestamp:
    {
      XII_VERIFY_QUERY(m_AdapterDescription.m_Features.m_TimestampQueries == xiiGALDeviceFeatureState::Enabled, "Timestamp queries are not supported by this device.");
    }
    break;
    case xiiGALQueryType::PipelineStatistics:
    {
      XII_VERIFY_QUERY(m_AdapterDescription.m_Features.m_PipelineStatisticsQueries == xiiGALDeviceFeatureState::Enabled, "Pipeline statistics queries are not supported by this device.");
    }
    break;
    case xiiGALQueryType::Duration:
    {
      XII_VERIFY_QUERY(m_AdapterDescription.m_Features.m_DurationQueries == xiiGALDeviceFeatureState::Enabled, "Duration queries are not supported by this device.");
    }
    break;

    default:
      XII_VERIFY_QUERY(false, "Unexpected query type.");
  }

  xiiGALQuery* pQuery = CreateQueryPlatform(description);

  if (pQuery == nullptr)
  {
    return xiiGALQueryHandle();
  }
  else
  {
    return xiiGALQueryHandle(m_Queries.Insert(pQuery));
  }
}

void xiiGALDevice::DestroyQuery(xiiGALQueryHandle hQuery)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALQuery* pQuery = nullptr;

  if (m_Queries.TryGetValue(hQuery, pQuery))
  {
    AddDestroyedObject(GALObjectType::Query, hQuery);
  }
  else
  {
    xiiLog::Warning("DestroyQuery called on an invalid handle (double free?).");
  }
}

#undef XII_VERIFY_QUERY

#define XII_VERIFY_FENCE(expression, ...)              \
  do                                                   \
  {                                                    \
    XII_ASSERT_DEV((expression), __VA_ARGS__);         \
    if (!(expression)) { return xiiGALFenceHandle(); } \
  } while (false)

xiiGALFenceHandle xiiGALDevice::CreateFence(const xiiGALFenceCreationDescription& description)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  switch (description.m_Type)
  {
    case xiiGALFenceType::CpuWaitOnly:
      break;
    case xiiGALFenceType::General:
    {
      XII_VERIFY_FENCE(m_AdapterDescription.m_Features.m_NativeFence == xiiGALDeviceFeatureState::Enabled, "xiiGALFenceType::General requires the Native Fence device feature.");
    }
    break;

    default:
      XII_VERIFY_FENCE(false, "Unexpected query type.");
  }

  xiiGALFence* pFence = CreateFencePlatform(description);

  if (pFence == nullptr)
  {
    return xiiGALFenceHandle();
  }
  else
  {
    return xiiGALFenceHandle(m_Fences.Insert(pFence));
  }
}

void xiiGALDevice::DestroyFence(xiiGALFenceHandle hFence)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALFence* pFence = nullptr;

  if (m_Fences.TryGetValue(hFence, pFence))
  {
    AddDestroyedObject(GALObjectType::Fence, hFence);
  }
  else
  {
    xiiLog::Warning("DestroyFence called on an invalid handle (double free?).");
  }
}

#undef XII_VERIFY_FENCE

#define XII_VERIFY_RENDER_PASS(expression, ...)             \
  do                                                        \
  {                                                         \
    XII_ASSERT_DEV((expression), __VA_ARGS__);              \
    if (!(expression)) { return xiiGALRenderPassHandle(); } \
  } while (false)

xiiGALRenderPassHandle xiiGALDevice::CreateRenderPass(const xiiGALRenderPassCreationDescription& description)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  XII_VERIFY_RENDER_PASS(description.m_SubPasses.GetCount() > 0U, "The sub pass count must be greater than 0.");

  const bool bIsVulkanDevice = m_Description.m_GraphicsDeviceType == xiiGALGraphicsDeviceType::Vulkan;

  for (xiiUInt32 uiAttachmentIndex = 0U; uiAttachmentIndex < description.m_Attachments.GetCount(); ++uiAttachmentIndex)
  {
    const auto& attachment = description.m_Attachments[uiAttachmentIndex];

    XII_VERIFY_RENDER_PASS(attachment.m_Format != xiiGALTextureFormat::Unknown, "The format of attachment {0} is unknown.", uiAttachmentIndex);
    XII_VERIFY_RENDER_PASS(attachment.m_uiSampleCount != 0U, "The sample count of attachment {0} is zero.", uiAttachmentIndex);
    XII_VERIFY_RENDER_PASS(xiiMath::IsPowerOf2(attachment.m_uiSampleCount), "The sample count ({0}) of attachment {1} is not a power of 2.", attachment.m_uiSampleCount, uiAttachmentIndex);

    const auto& formatProperties = xiiGALGraphicsUtilities::GetTextureFormatProperties(attachment.m_Format);
    if (formatProperties.m_ComponentType == xiiGALTextureFormatComponentType::Depth || formatProperties.m_ComponentType == xiiGALTextureFormatComponentType::DepthStencil)
    {
      XII_VERIFY_RENDER_PASS(attachment.m_InitialStateFlags.IsStrictlyAnySet(xiiGALResourceStateFlags::DepthWrite | xiiGALResourceStateFlags::DepthRead | xiiGALResourceStateFlags::UnorderedAccess | xiiGALResourceStateFlags::ShaderResource | xiiGALResourceStateFlags::ResolveDestination | xiiGALResourceStateFlags::ResolveSource | xiiGALResourceStateFlags::CopyDestination | xiiGALResourceStateFlags::CopySource | xiiGALResourceStateFlags::InputAttachment | xiiGALResourceStateFlags::Undefined) || (bIsVulkanDevice && attachment.m_InitialStateFlags.IsSet(xiiGALResourceStateFlags::Common)),
                             "The initial state of the depth-stencil attachment {0} is invalid.", uiAttachmentIndex);

      XII_VERIFY_RENDER_PASS(attachment.m_FinalStateFlags.IsStrictlyAnySet(xiiGALResourceStateFlags::DepthWrite | xiiGALResourceStateFlags::DepthRead | xiiGALResourceStateFlags::UnorderedAccess | xiiGALResourceStateFlags::ShaderResource | xiiGALResourceStateFlags::ResolveDestination | xiiGALResourceStateFlags::ResolveSource | xiiGALResourceStateFlags::CopyDestination | xiiGALResourceStateFlags::CopySource | xiiGALResourceStateFlags::InputAttachment) || (bIsVulkanDevice && attachment.m_FinalStateFlags.IsSet(xiiGALResourceStateFlags::Common)),
                             "The final state of the depth-stencil attachment {0} is invalid.", uiAttachmentIndex);
    }
    else
    {
      XII_VERIFY_RENDER_PASS(attachment.m_InitialStateFlags.IsStrictlyAnySet(xiiGALResourceStateFlags::RenderTarget | xiiGALResourceStateFlags::UnorderedAccess | xiiGALResourceStateFlags::ShaderResource | xiiGALResourceStateFlags::ResolveDestination | xiiGALResourceStateFlags::ResolveSource | xiiGALResourceStateFlags::CopyDestination | xiiGALResourceStateFlags::CopySource | xiiGALResourceStateFlags::InputAttachment | xiiGALResourceStateFlags::Present | xiiGALResourceStateFlags::ShadingRate | xiiGALResourceStateFlags::Undefined) || (bIsVulkanDevice && attachment.m_InitialStateFlags.IsSet(xiiGALResourceStateFlags::Common)),
                             "The initial state of the color attachment {0} is invalid.", uiAttachmentIndex);

      XII_VERIFY_RENDER_PASS(attachment.m_FinalStateFlags.IsStrictlyAnySet(xiiGALResourceStateFlags::RenderTarget | xiiGALResourceStateFlags::UnorderedAccess | xiiGALResourceStateFlags::ShaderResource | xiiGALResourceStateFlags::ResolveDestination | xiiGALResourceStateFlags::ResolveSource | xiiGALResourceStateFlags::CopyDestination | xiiGALResourceStateFlags::CopySource | xiiGALResourceStateFlags::InputAttachment | xiiGALResourceStateFlags::Present | xiiGALResourceStateFlags::ShadingRate) || (bIsVulkanDevice && attachment.m_FinalStateFlags.IsSet(xiiGALResourceStateFlags::Common)),
                             "The final state of the color attachment {0} is invalid.", uiAttachmentIndex);
    }
  }

  const xiiGALShadingRateAttachmentDescription* pShadingRateAttachment = nullptr;
  for (xiiUInt32 uiSubPassIndex = 0U; uiSubPassIndex < description.m_SubPasses.GetCount(); ++uiSubPassIndex)
  {
    const auto& subpass = description.m_SubPasses[uiSubPassIndex];

    for (xiiUInt32 uiInputAttachmentIndex = 0U; uiInputAttachmentIndex < subpass.m_InputAttachments.GetCount(); ++uiInputAttachmentIndex)
    {
      const auto& attachmentReference = subpass.m_InputAttachments[uiInputAttachmentIndex];

      if (attachmentReference.m_uiAttachmentIndex == XII_GAL_ATTACHMENT_UNUSED)
        continue;

      // If the attachment member of any element of Input Attachment, Color Attachment, Resolve Attachment or Depth Stencil attachment, or any element of Preserve Attachments in any element of
      // the sub pass is not XII_GAL_ATTACHMENT_UNUSED, it must be less than the attachment count.
      // Link: https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkRenderPassCreateInfo-attachment-00834
      XII_VERIFY_RENDER_PASS(attachmentReference.m_uiAttachmentIndex < description.m_Attachments.GetCount(), "The attachment index ({0}) of the input attachment reference {1} of sub pass {2} must be less than the number of attachments ({3}).", attachmentReference.m_uiAttachmentIndex, uiInputAttachmentIndex, uiSubPassIndex, description.m_Attachments.GetCount());
      XII_VERIFY_RENDER_PASS(attachmentReference.m_ResourceStateFlags == xiiGALResourceStateFlags::InputAttachment || (bIsVulkanDevice && attachmentReference.m_ResourceStateFlags == xiiGALResourceStateFlags::Common), "The attachment with index {0} referenced as an input attachment in sub pass {1} must be in {2} state.", attachmentReference.m_uiAttachmentIndex, uiSubPassIndex, (bIsVulkanDevice ? "xiiGALResourceStateFlags::InputAttachment or xiiGALResourceStateFlags::Common" : "xiiGALResourceStateFlags::InputAttachment"));
    }

    for (xiiUInt32 uiColorAttachmentIndex = 0U; uiColorAttachmentIndex < subpass.m_RenderTargetAttachments.GetCount(); ++uiColorAttachmentIndex)
    {
      const auto& attachmentReference = subpass.m_RenderTargetAttachments[uiColorAttachmentIndex];

      if (attachmentReference.m_uiAttachmentIndex == XII_GAL_ATTACHMENT_UNUSED)
        continue;

      // If the attachment member of any element of Input Attachment, Color Attachment, Resolve Attachment or Depth Stencil attachment, or any element of Preserve Attachments in any element of
      // the sub pass is not XII_GAL_ATTACHMENT_UNUSED, it must be less than the attachment count.
      // Link: https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkRenderPassCreateInfo-attachment-00834
      XII_VERIFY_RENDER_PASS(attachmentReference.m_uiAttachmentIndex < description.m_Attachments.GetCount(), "The attachment index ({0}) of the render target attachment reference {1} of sub pass {2} must be less than the number of attachments ({3}).", attachmentReference.m_uiAttachmentIndex, uiColorAttachmentIndex, uiSubPassIndex, description.m_Attachments.GetCount());
      XII_VERIFY_RENDER_PASS(attachmentReference.m_ResourceStateFlags == xiiGALResourceStateFlags::RenderTarget || (bIsVulkanDevice && attachmentReference.m_ResourceStateFlags == xiiGALResourceStateFlags::Common), "The attachment with index {0} referenced as an input attachment in sub pass {1} must be in {2} state.", attachmentReference.m_uiAttachmentIndex, uiSubPassIndex, (bIsVulkanDevice ? "xiiGALResourceStateFlags::RenderTarget or xiiGALResourceStateFlags::Common" : "xiiGALResourceStateFlags::RenderTarget"));

      const auto& format             = description.m_Attachments[attachmentReference.m_uiAttachmentIndex].m_Format;
      const auto& rtFormatProperties = xiiGALGraphicsUtilities::GetTextureFormatProperties(format);
      XII_VERIFY_RENDER_PASS(rtFormatProperties.m_ComponentType != xiiGALTextureFormatComponentType::Depth && rtFormatProperties.m_ComponentType != xiiGALTextureFormatComponentType::DepthStencil && rtFormatProperties.m_ComponentType != xiiGALTextureFormatComponentType::Compressed, "Attachment with index {0} referenced as a render target attachment in sub pass {1} uses format {2}, which is not a valid render target format.", attachmentReference.m_uiAttachmentIndex, uiSubPassIndex, format.GetValue());
    }

    if (!subpass.m_ResolveAttachments.IsEmpty())
    {
      for (xiiUInt32 uiResolveAttachmentIndex = 0U; uiResolveAttachmentIndex < subpass.m_ResolveAttachments.GetCount(); ++uiResolveAttachmentIndex)
      {
        const auto& attachmentReference = subpass.m_RenderTargetAttachments[uiResolveAttachmentIndex];

        if (attachmentReference.m_uiAttachmentIndex == XII_GAL_ATTACHMENT_UNUSED)
          continue;

        // If the attachment member of any element of Input Attachment, Color Attachment, Resolve Attachment or Depth Stencil attachment, or any element of Preserve Attachments in any element of
        // the sub pass is not XII_GAL_ATTACHMENT_UNUSED, it must be less than the attachment count.
        // Link: https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkRenderPassCreateInfo-attachment-00834
        XII_VERIFY_RENDER_PASS(attachmentReference.m_uiAttachmentIndex < description.m_Attachments.GetCount(), "The attachment index ({0}) of the resolve attachment reference {1} of sub pass {2} must be less than the number of attachments ({3}).", attachmentReference.m_uiAttachmentIndex, uiResolveAttachmentIndex, uiSubPassIndex, description.m_Attachments.GetCount());
      }
    }

    if (!subpass.m_DepthStencilAttachment.IsEmpty())
    {
      const auto& attachmentReference = subpass.m_DepthStencilAttachment.PeekBack();

      if (attachmentReference.m_uiAttachmentIndex != XII_GAL_ATTACHMENT_UNUSED)
      {
        // If the attachment member of any element of Input Attachment, Color Attachment, Resolve Attachment or Depth Stencil attachment, or any element of Preserve Attachments in any element of
        // the sub pass is not XII_GAL_ATTACHMENT_UNUSED, it must be less than the attachment count.
        // Link: https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkRenderPassCreateInfo-attachment-00834
        XII_VERIFY_RENDER_PASS(attachmentReference.m_uiAttachmentIndex < description.m_Attachments.GetCount(), "The attachment index ({0}) of the depth-stencil attachment reference of sub pass {1} must be less than the number of attachments ({3}).", attachmentReference.m_uiAttachmentIndex, uiSubPassIndex, description.m_Attachments.GetCount());
        XII_VERIFY_RENDER_PASS(attachmentReference.m_ResourceStateFlags == xiiGALResourceStateFlags::DepthRead || attachmentReference.m_ResourceStateFlags == xiiGALResourceStateFlags::DepthWrite || (bIsVulkanDevice && attachmentReference.m_ResourceStateFlags == xiiGALResourceStateFlags::Common), "The attachment with index ({0}) of the depth-stencil attachment reference of sub pass {1} must be must be in {2} state.", attachmentReference.m_uiAttachmentIndex, uiSubPassIndex, (bIsVulkanDevice ? "xiiGALResourceStateFlags::DepthRead or xiiGALResourceStateFlags::DepthWrite or xiiGALResourceStateFlags::Common" : "xiiGALResourceStateFlags::DepthRead or xiiGALResourceStateFlags::DepthWrite"));

        const auto& format                = description.m_Attachments[attachmentReference.m_uiAttachmentIndex].m_Format;
        const auto& depthFormatProperties = xiiGALGraphicsUtilities::GetTextureFormatProperties(format);
        XII_VERIFY_RENDER_PASS(depthFormatProperties.m_ComponentType == xiiGALTextureFormatComponentType::Depth || depthFormatProperties.m_ComponentType == xiiGALTextureFormatComponentType::DepthStencil, "Attachment with index {0} referenced as a depth-stencil attachment in sub pass {1} uses format {2}, which is not a valid depth buffer format.", attachmentReference.m_uiAttachmentIndex, uiSubPassIndex, format.GetValue());
      }
    }

    for (xiiUInt32 uiPreserveAttachmentIndex = 0U; uiPreserveAttachmentIndex < subpass.m_PreserveAttachments.GetCount(); ++uiPreserveAttachmentIndex)
    {
      const auto& attachmentReference = subpass.m_PreserveAttachments[uiPreserveAttachmentIndex];

      XII_VERIFY_RENDER_PASS(attachmentReference != XII_GAL_ATTACHMENT_UNUSED, "The attachment index of preserve attachment reference {0} of sub pass {1} is XII_GAL_ATTACHMENT_UNUSED.");
      XII_VERIFY_RENDER_PASS(attachmentReference < description.m_Attachments.GetCount(), "The attachment index ({0}) of the preserve attachment reference {1} of sub pass {2} must be less than the number of attachments ({3}).", attachmentReference, uiPreserveAttachmentIndex, uiSubPassIndex, description.m_Attachments.GetCount());
    }

    if (!subpass.m_ResolveAttachments.IsEmpty())
    {
      for (xiiUInt32 uiColorAttachmentIndex = 0U; uiColorAttachmentIndex < subpass.m_RenderTargetAttachments.GetCount(); ++uiColorAttachmentIndex)
      {
        const auto& attachmentReference       = subpass.m_RenderTargetAttachments[uiColorAttachmentIndex];
        const auto& resolveAttacmentReference = subpass.m_ResolveAttachments[uiColorAttachmentIndex];

        if (resolveAttacmentReference.m_uiAttachmentIndex != XII_GAL_ATTACHMENT_UNUSED && attachmentReference.m_uiAttachmentIndex == XII_GAL_ATTACHMENT_UNUSED)
        {
          // If pResolveAttachments is not NULL, for each resolve attachment that is not VK_ATTACHMENT_UNUSED, the corresponding color attachment must not be VK_ATTACHMENT_UNUSED.
          // Link: https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkSubpassDescription-pResolveAttachments-00847
          XII_VERIFY_RENDER_PASS(false, "The resolve attachment of sub pass {0} is not unused but the render target attachment reference {1} is unused.", uiSubPassIndex, uiColorAttachmentIndex);
        }

        if (resolveAttacmentReference.m_uiAttachmentIndex != XII_GAL_ATTACHMENT_UNUSED && description.m_Attachments[attachmentReference.m_uiAttachmentIndex].m_uiSampleCount == 1U)
        {
          // If pResolveAttachments is not NULL, for each resolve attachment that is not VK_ATTACHMENT_UNUSED, the corresponding color attachment must not have a sample count of VK_SAMPLE_COUNT_1_BIT.
          // Link: https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkSubpassDescription-pResolveAttachments-00848
          XII_VERIFY_RENDER_PASS(false, "The render target attachment at index {0} referenced by attachment reference {1} of sub pass {2} is used as the source of a resolve operation, but its sample count is 1.", attachmentReference.m_uiAttachmentIndex, uiColorAttachmentIndex, uiSubPassIndex);
        }

        if (resolveAttacmentReference.m_uiAttachmentIndex != XII_GAL_ATTACHMENT_UNUSED && description.m_Attachments[resolveAttacmentReference.m_uiAttachmentIndex].m_uiSampleCount != 1U)
        {
          // If pResolveAttachments is not NULL, each resolve attachment that is not VK_ATTACHMENT_UNUSED must have a sample count of VK_SAMPLE_COUNT_1_BIT.
          // Link: https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkSubpassDescription-pResolveAttachments-00849
          XII_VERIFY_RENDER_PASS(false, "The resolve attachment at index {0} referenced by attachment reference {1} of sub pass {2} must have a sample count of 1.", resolveAttacmentReference.m_uiAttachmentIndex, uiColorAttachmentIndex, uiSubPassIndex);
        }

        if (resolveAttacmentReference.m_uiAttachmentIndex != XII_GAL_ATTACHMENT_UNUSED && attachmentReference.m_uiAttachmentIndex != XII_GAL_ATTACHMENT_UNUSED && description.m_Attachments[attachmentReference.m_uiAttachmentIndex].m_Format != description.m_Attachments[resolveAttacmentReference.m_uiAttachmentIndex].m_Format)
        {
          // If pResolveAttachments is not NULL, each resolve attachment that is not VK_ATTACHMENT_UNUSED must have the same VkFormat as its corresponding color attachment.
          // Link: https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkSubpassDescription-pResolveAttachments-00850
          XII_VERIFY_RENDER_PASS(false, "The format ({0}) of render target attachment at index {1} referenced by attachment reference {2} of sub pass {3} does not match the format ({4}) of the corresponding resolve attachment at index {5}.", description.m_Attachments[attachmentReference.m_uiAttachmentIndex].m_Format.GetValue(), attachmentReference.m_uiAttachmentIndex, uiColorAttachmentIndex, uiSubPassIndex, description.m_Attachments[resolveAttacmentReference.m_uiAttachmentIndex].m_Format.GetValue(), resolveAttacmentReference.m_uiAttachmentIndex);
        }
      }
    }

    if (!subpass.m_ShadingRateAttachment.IsEmpty())
    {
      pShadingRateAttachment          = pShadingRateAttachment == nullptr ? subpass.m_ShadingRateAttachment.GetData() : pShadingRateAttachment;
      const auto& attachmentReference = subpass.m_ShadingRateAttachment[0].m_AttachmentReference;

      if (attachmentReference.m_uiAttachmentIndex != XII_GAL_ATTACHMENT_UNUSED)
      {
        XII_VERIFY_RENDER_PASS(attachmentReference.m_uiAttachmentIndex < description.m_Attachments.GetCount(), "The attachment index ({0}) of the shading rate attachment reference of sub pass {1} must be less than the number of attachments ({3}).", attachmentReference.m_uiAttachmentIndex, uiSubPassIndex, description.m_Attachments.GetCount());
        XII_VERIFY_RENDER_PASS(m_AdapterDescription.m_Features.m_VariableRateShading == xiiGALDeviceFeatureState::Enabled, "The sub pass at index {0} uses a shading rate attachment, but the Variable Shading Rate device feature is not enabled.", uiSubPassIndex);
        XII_VERIFY_RENDER_PASS(attachmentReference.m_ResourceStateFlags == xiiGALResourceStateFlags::ShadingRate, "The attachment with index {0} referenced as a shading rate attachment in sub pass {1} must be in the xiiGALResourceStateFlags::ShadingRate state.");

        const auto& tileSize    = subpass.m_ShadingRateAttachment[0].m_TileSize;
        const auto& minTileSize = m_AdapterDescription.m_ShadingRateProperties.m_MinTileSize;
        const auto& maxTileSize = m_AdapterDescription.m_ShadingRateProperties.m_MaxTileSize;
        if (tileSize.HasNonZeroArea())
        {
          XII_VERIFY_RENDER_PASS(tileSize.width >= minTileSize.width && tileSize.width <= maxTileSize.width, "The sub pass at index {0} uses a shading rate attachment with tile width {1} that is not in te allowed range [{2},{3}]. Check MinTileSize/MaxTileSize members of the Shading Rate Properties.", uiSubPassIndex, tileSize.width, minTileSize.width, maxTileSize.height);
          XII_VERIFY_RENDER_PASS(tileSize.height >= minTileSize.height && tileSize.height <= maxTileSize.height, "The sub pass at index {0} uses a shading rate attachment with tile height {1} that is not in te allowed range [{2},{3}]. Check MinTileSize/MaxTileSize members of the Shading Rate Properties.", uiSubPassIndex, tileSize.height, minTileSize.height, maxTileSize.height);

          // The tile size is only used for Vulkan shading rate and current hardware only supports aspect ratio of 1.
          /// \todo GraphicsFoundation: Use VkPhysicalDeviceFragmentShadingRatePropertiesKHR::maxFragmentShadingRateAttachmentTexelSizeAspectRatio.
          XII_VERIFY_RENDER_PASS(tileSize.width == tileSize.height, "The sub pass at index {0} uses shading rate attachment with tile width {1} that is not equal to the tile height {2}.", uiSubPassIndex, tileSize.width, tileSize.height);
          XII_VERIFY_RENDER_PASS(xiiMath::IsPowerOf2(tileSize.width) && xiiMath::IsPowerOf2(tileSize.height), "The sub pass at index {0} uses a shading rate attachment with tile sizes {1}x{2} that are not a power of two.", uiSubPassIndex, tileSize.width, tileSize.height);
        }
      }
    }
  }

  if (pShadingRateAttachment != nullptr && m_AdapterDescription.m_ShadingRateProperties.m_CapabilityFlags.IsSet(xiiGALShadingRateCapabilityFlags::SameTextureForWholeRenderPass))
  {
    for (xiiUInt32 uiSubPassIndex = 0U; uiSubPassIndex < description.m_SubPasses.GetCount(); ++uiSubPassIndex)
    {
      const auto& subpass = description.m_SubPasses[uiSubPassIndex];

      XII_VERIFY_RENDER_PASS(!subpass.m_ShadingRateAttachment.IsEmpty(), "Render pass uses a shading rate attachment, but sub pass {0} uses no shading rate attachment. A device with the xiiGALShadingRateCapabilityFlags::SameTextureForWholeRenderPass capability requires that all sub passes of a render pass use the same shading rate attachment.", uiSubPassIndex);
      XII_VERIFY_RENDER_PASS(subpass.m_ShadingRateAttachment == xiiMakeArrayPtr(pShadingRateAttachment, 1U), "The shading rate attachment in sub pass {0} does not match the shading rate attachment used by the previous sub passes. A device with the xiiGALShadingRateCapabilityFlags::SameTextureForWholeRenderPass capability requires that all sub passes of a render pass use the same shading rate attachment.", uiSubPassIndex);
    }
  }

  for (xiiUInt32 uiDependencyIndex = 0U; uiDependencyIndex < description.m_Dependencies.GetCount(); ++uiDependencyIndex)
  {
    const auto& dependency = description.m_Dependencies[uiDependencyIndex];

    /// \todo GraphicsFoundation: Check render pass dependency source and destination stage mask is set to the undefined pipeline stage.
  }

  xiiGALRenderPass* pRenderPass = CreateRenderPassPlatform(description);

  if (pRenderPass == nullptr)
  {
    return xiiGALRenderPassHandle();
  }
  else
  {
    return xiiGALRenderPassHandle(m_RenderPasses.Insert(pRenderPass));
  }
}

void xiiGALDevice::DestroyRenderPass(xiiGALRenderPassHandle hRenderPass)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALRenderPass* pRenderPass = nullptr;

  if (m_RenderPasses.TryGetValue(hRenderPass, pRenderPass))
  {
    AddDestroyedObject(GALObjectType::RenderPass, hRenderPass);
  }
  else
  {
    xiiLog::Warning("DestroyRenderPass called on an invalid handle (double free?).");
  }
}

#undef XII_VERIFY_RENDER_PASS

#define XII_VERIFY_FRAME_BUFFER(expression, ...)             \
  do                                                         \
  {                                                          \
    XII_ASSERT_DEV((expression), __VA_ARGS__);               \
    if (!(expression)) { return xiiGALFramebufferHandle(); } \
  } while (false)

xiiGALFramebufferHandle xiiGALDevice::CreateFramebuffer(const xiiGALFramebufferCreationDescription& description)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  XII_VERIFY_FRAME_BUFFER(!description.m_hRenderPass.IsInvalidated(), "The render pass handle is invalid.");

  for (xiiUInt32 i = 0U; i < description.m_Attachments.GetCount(); ++i)
  {
    const auto& attachment = description.m_Attachments[i];

    // If flags does not include VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT, and attachmentCount is not 0, pAttachments must be a valid pointer to an array of attachmentCount valid VkImageView handles.
    // Link: https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkFramebufferCreateInfo-flags-02778
    XII_VERIFY_FRAME_BUFFER(!attachment.IsInvalidated(), "The framebuffer attachment at index {0} is invalid.", i);
  }

  const auto& renderPassDescription = GetRenderPass(description.m_hRenderPass)->GetDescription();

  XII_VERIFY_FRAME_BUFFER(description.m_Attachments.GetCount() == renderPassDescription.m_Attachments.GetCount(), "The number of framebuffer attachments ({0}) must be equal to the number of attachments ({1}) in the render pass.", description.m_Attachments.GetCount(), renderPassDescription.m_Attachments.GetCount());

  for (xiiUInt32 uiAttachmentIndex = 0U; uiAttachmentIndex < renderPassDescription.m_Attachments.GetCount(); ++uiAttachmentIndex)
  {
    const auto& attachmentDescription = renderPassDescription.m_Attachments[uiAttachmentIndex];
    const auto& viewDescription       = GetTextureView(description.m_Attachments[uiAttachmentIndex])->GetDescription();
    const auto& textureDescription    = GetTextureView(description.m_Attachments[uiAttachmentIndex])->GetTexture()->GetDescription();

    // If flags does not include VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT, each element of pAttachments must have been created with a VkFormat value that matches the VkFormat specified by the corresponding VkAttachmentDescription in renderPass.
    // Link: https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkFramebufferCreateInfo-pAttachments-00880
    XII_VERIFY_FRAME_BUFFER(viewDescription.m_Format == textureDescription.m_Format, "The format ({0}) of attachment {1} does not match the format ({2}) defined by the render pass for the same attachment.", viewDescription.m_Format.GetValue(), uiAttachmentIndex, textureDescription.m_Format.GetValue());

    // If flags does not include VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT, each element of pAttachments must have been created with a samples value that matches the samples value specified by the corresponding VkAttachmentDescription in renderPass
    // Link: https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkFramebufferCreateInfo-pAttachments-00881
    XII_VERIFY_FRAME_BUFFER(textureDescription.m_uiSampleCount == attachmentDescription.m_uiSampleCount, "The sample count ({0}) of attachment {1} does not match the sample count ({2}) defined by the render pass for the same attachment.", textureDescription.m_uiSampleCount, uiAttachmentIndex, attachmentDescription.m_uiSampleCount);

    if (textureDescription.m_MiscFlags.IsSet(xiiGALMiscTextureFlags::Memoryless))
    {
      const bool bHasStencilComponent = xiiGALGraphicsUtilities::GetTextureFormatProperties(attachmentDescription.m_Format).m_ComponentType == xiiGALTextureFormatComponentType::DepthStencil;

      XII_VERIFY_FRAME_BUFFER(attachmentDescription.m_LoadOperation != xiiGALAttachmentLoadOperation::Load && !(bHasStencilComponent && attachmentDescription.m_StencilLoadOperation == xiiGALAttachmentLoadOperation::Load), "Memoryless attachment {i} is not compatible with xiiGALAttachmentLoadOperation::Load.", uiAttachmentIndex);
      XII_VERIFY_FRAME_BUFFER(attachmentDescription.m_StencilStoreOperation != xiiGALAttachmentStoreOperation::Store && !(bHasStencilComponent && attachmentDescription.m_StencilLoadOperation == xiiGALAttachmentStoreOperation::Store), "Memoryless attachment {i} is not compatible with xiiGALAttachmentStoreOperation::Store.", uiAttachmentIndex);

#if XII_ENABLED(XII_PLATFORM_OSX)
      {
        xiiUInt32 uiSubpassCount = 0U;

        for (xiiUInt32 j = 0; j < renderPassDescription.m_SubPasses.GetCount(); ++j)
        {
          const auto& subpass = renderPassDescription.m_SubPasses[j];

          bool bUsedInSubPass = false;
          for (xiiUInt32 k = 0; k < subpass.m_RenderTargetAttachments.GetCount(); ++k)
          {
            const auto& attachmentReference = subpass.m_RenderTargetAttachments[k];

            if (attachmentReference.m_uiAttachmentIndex == uiAttachmentIndex)
              bUsedInSubPass = true;
          }
          for (xiiUInt32 k = 0; k < subpass.m_InputAttachments.GetCount(); ++k)
          {
            const auto& attachmentReference = subpass.m_InputAttachments[k];

            if (attachmentReference.m_uiAttachmentIndex == uiAttachmentIndex)
              bUsedInSubPass = true;
          }
          if (!subpass.m_DepthStencilAttachment.IsEmpty() && subpass.m_DepthStencilAttachment[0].m_uiAttachmentIndex == uiAttachmentIndex)
            bUsedInSubPass = true;

          if (bUsedInSubPass)
            ++uiSubpassCount;
        }

        XII_VERIFY_FRAME_BUFFER(uiSubpassCount <= 1U, "Memoryless attachment {0} is used in more than one sub pass, which is not supported on MacOS/iOS as the contents of the attachment cannot be preserved between sub passes without storing it in global memory.", uiAttachmentIndex);
      }
#endif
    }
  }

  for (xiiUInt32 uiSubPassIndex = 0U; uiSubPassIndex < renderPassDescription.m_SubPasses.GetCount(); ++uiSubPassIndex)
  {
    const auto& subpass = renderPassDescription.m_SubPasses[uiSubPassIndex];

    for (xiiUInt32 uiInputAttachmentIndex = 0U; uiInputAttachmentIndex < subpass.m_InputAttachments.GetCount(); ++uiInputAttachmentIndex)
    {
      const auto& attachmentReference = subpass.m_InputAttachments[uiInputAttachmentIndex];

      if (attachmentReference.m_uiAttachmentIndex == XII_GAL_ATTACHMENT_UNUSED)
        continue;

      XII_VERIFY_FRAME_BUFFER(attachmentReference.m_uiAttachmentIndex < description.m_Attachments.GetCount(), "The input attachment index ({0}) at {1} must be less than the attachment count ({2}).", attachmentReference.m_uiAttachmentIndex, uiInputAttachmentIndex, description.m_Attachments.GetCount());

      const auto& hAttachment = description.m_Attachments[attachmentReference.m_uiAttachmentIndex];
      XII_VERIFY_FRAME_BUFFER(!hAttachment.IsInvalidated(), "The attachment at index {0} is used as an input attachment by sub pass {1} of render pass and must be valid.", attachmentReference.m_uiAttachmentIndex, uiSubPassIndex);

      const auto& textureDescription = GetTextureView(hAttachment)->GetTexture()->GetDescription();

      // If flags does not include VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT, each element of pAttachments that is used as an input attachment by renderPass must have been created with a usage value including VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT.
      // Link: https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkFramebufferCreateInfo-pAttachments-00879
      XII_VERIFY_FRAME_BUFFER(textureDescription.m_BindFlags.IsSet(xiiGALBindFlags::InputAttachment), "The attachment at index {1} is used as an input attachment in sub pass {2} of render pass, but was not created with the xiiGALBindFlags::InputAttachment bind flag.", attachmentReference.m_uiAttachmentIndex, uiSubPassIndex);
    }

    for (xiiUInt32 uiColorAttachmentIndex = 0U; uiColorAttachmentIndex < subpass.m_RenderTargetAttachments.GetCount(); ++uiColorAttachmentIndex)
    {
      const auto& attachmentReference = subpass.m_RenderTargetAttachments[uiColorAttachmentIndex];

      if (attachmentReference.m_uiAttachmentIndex == XII_GAL_ATTACHMENT_UNUSED)
        continue;

      XII_VERIFY_FRAME_BUFFER(attachmentReference.m_uiAttachmentIndex < description.m_Attachments.GetCount(), "The render target attachment index ({0}) at {1} must be less than the attachment count ({2}).", attachmentReference.m_uiAttachmentIndex, uiColorAttachmentIndex, description.m_Attachments.GetCount());

      const auto& hAttachment = description.m_Attachments[attachmentReference.m_uiAttachmentIndex];
      XII_VERIFY_FRAME_BUFFER(!hAttachment.IsInvalidated(), "The attachment at index {0} is used as a render target attachment by sub pass {1} of render pass and must be valid.", attachmentReference.m_uiAttachmentIndex, uiSubPassIndex);

      const auto& viewDescription = GetTextureView(hAttachment)->GetDescription();
      XII_VERIFY_FRAME_BUFFER(viewDescription.m_ViewType == xiiGALTextureViewType::RenderTarget, "The attachment at index {0} is used as a render target attachment by sub pass {1} of render pass, but is not of the xiiGALTextureViewType::RenderTarget type.", attachmentReference.m_uiAttachmentIndex, uiSubPassIndex);

      const auto& textureDescription = GetTextureView(hAttachment)->GetTexture()->GetDescription();

      // If flags does not include VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT, each element of pAttachments that is used as a color attachment or resolve attachment by renderPass must have been created with a usage value including VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT.
      // https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkFramebufferCreateInfo-pAttachments-00877
      XII_VERIFY_FRAME_BUFFER(textureDescription.m_BindFlags.IsSet(xiiGALBindFlags::RenderTarget), "The attachment at index {0} is used as a render target attachment by sub pass {1} of render pass, but was not created with the xiiGALBindFlags::RenderTarget bind flag.", attachmentReference.m_uiAttachmentIndex, uiSubPassIndex);
    }

    for (xiiUInt32 uiResolveAttachmentIndex = 0U; uiResolveAttachmentIndex < subpass.m_ResolveAttachments.GetCount(); ++uiResolveAttachmentIndex)
    {
      const auto& attachmentReference = subpass.m_ResolveAttachments[uiResolveAttachmentIndex];

      if (attachmentReference.m_uiAttachmentIndex == XII_GAL_ATTACHMENT_UNUSED)
        continue;

      XII_VERIFY_FRAME_BUFFER(attachmentReference.m_uiAttachmentIndex < description.m_Attachments.GetCount(), "The resolve attachment index ({0}) at {1} must be less than the attachment count ({2}).", attachmentReference.m_uiAttachmentIndex, uiResolveAttachmentIndex, description.m_Attachments.GetCount());

      const auto& hAttachment = description.m_Attachments[attachmentReference.m_uiAttachmentIndex];
      XII_VERIFY_FRAME_BUFFER(!hAttachment.IsInvalidated(), "The attachment at index {0} is used as a resolve attachment by sub pass {1} of render pass and must be valid.", attachmentReference.m_uiAttachmentIndex, uiSubPassIndex);

      const auto& viewDescription = GetTextureView(hAttachment)->GetDescription();
      XII_VERIFY_FRAME_BUFFER(viewDescription.m_ViewType == xiiGALTextureViewType::RenderTarget, "The attachment at index {0} is used as a resolve attachment by sub pass {1} of render pass, but is not of the xiiGALTextureViewType::RenderTarget type.", attachmentReference.m_uiAttachmentIndex, uiSubPassIndex);

      const auto& textureDescription = GetTextureView(hAttachment)->GetTexture()->GetDescription();

      // If flags does not include VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT, each element of pAttachments that is used as a color attachment or resolve attachment by renderPass must have been created with a usage value including VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT.
      // https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkFramebufferCreateInfo-pAttachments-00877
      XII_VERIFY_FRAME_BUFFER(textureDescription.m_BindFlags.IsSet(xiiGALBindFlags::RenderTarget), "The attachment at index {0} is used as a resolve attachment by sub pass {1} of render pass, but was not created with the xiiGALBindFlags::RenderTarget bind flag.", attachmentReference.m_uiAttachmentIndex, uiSubPassIndex);
    }

    if (!subpass.m_DepthStencilAttachment.IsEmpty())
    {
      const auto& attachmentReference = subpass.m_DepthStencilAttachment[0];

      if (attachmentReference.m_uiAttachmentIndex != XII_GAL_ATTACHMENT_UNUSED)
      {
        XII_VERIFY_FRAME_BUFFER(attachmentReference.m_uiAttachmentIndex < description.m_Attachments.GetCount(), "The depth-stencil attachment index ({0}) must be less than the attachment count ({1}).", attachmentReference.m_uiAttachmentIndex, description.m_Attachments.GetCount());

        const auto& hAttachment = description.m_Attachments[attachmentReference.m_uiAttachmentIndex];
        XII_VERIFY_FRAME_BUFFER(!hAttachment.IsInvalidated(), "The attachment at index {0} is used as a depth-stencil attachment by sub pass {1} of render pass and must be valid.", attachmentReference.m_uiAttachmentIndex, uiSubPassIndex);

        const auto& viewDescription = GetTextureView(hAttachment)->GetDescription();
        XII_VERIFY_FRAME_BUFFER(viewDescription.m_ViewType == xiiGALTextureViewType::DepthStencil, "The attachment at index {0} is used as a depth-stencil attachment by sub pass {1} of render pass, but is not of the xiiGALTextureViewType::DepthStencil type.", attachmentReference.m_uiAttachmentIndex, uiSubPassIndex);

        const auto& textureDescription = GetTextureView(hAttachment)->GetTexture()->GetDescription();

        // If flags does not include VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT, each element of pAttachments that is used as a depth/stencil attachment by renderPass must have been created with a usage value including VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT.
        // Link: https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkFramebufferCreateInfo-pAttachments-02633
        XII_VERIFY_FRAME_BUFFER(textureDescription.m_BindFlags.IsSet(xiiGALBindFlags::DepthStencil), "The attachment at index {0} is used as a depth-stencil attachment by sub pass {1} of render pass, but was not created with the xiiGALBindFlags::DepthStencil bind flag.", attachmentReference.m_uiAttachmentIndex, uiSubPassIndex);
      }
    }
  }

  bool bIsVRSEnabled = false;
  for (xiiUInt32 uiSubPassIndex = 0U; uiSubPassIndex < renderPassDescription.m_SubPasses.GetCount(); ++uiSubPassIndex)
  {
    const auto& subpass = renderPassDescription.m_SubPasses[uiSubPassIndex];

    if (!subpass.m_ShadingRateAttachment.IsEmpty())
    {
      const auto& attachmentReference = subpass.m_ShadingRateAttachment[0];
      if (attachmentReference.m_AttachmentReference.m_uiAttachmentIndex != XII_GAL_ATTACHMENT_UNUSED)
      {
        XII_VERIFY_FRAME_BUFFER(attachmentReference.m_AttachmentReference.m_uiAttachmentIndex < description.m_Attachments.GetCount(), "The shading rate attachment index ({0}) must be less than the attachment count ({1}).", attachmentReference.m_AttachmentReference.m_uiAttachmentIndex, description.m_Attachments.GetCount());

        const auto& hAttachment = description.m_Attachments[attachmentReference.m_AttachmentReference.m_uiAttachmentIndex];
        XII_VERIFY_FRAME_BUFFER(!hAttachment.IsInvalidated(), "The attachment at index {0} is used as a shading rate attachment by sub pass {1} of render pass and must be valid.", attachmentReference.m_AttachmentReference.m_uiAttachmentIndex, uiSubPassIndex);

        const auto& viewDescription = GetTextureView(hAttachment)->GetDescription();
        XII_VERIFY_FRAME_BUFFER(viewDescription.m_ViewType == xiiGALTextureViewType::ShadingRate, "The attachment at index {0} is used as a shading rate attachment by sub pass {1} of render pass, but is not of the xiiGALTextureViewType::ShadingRate type.", attachmentReference.m_AttachmentReference.m_uiAttachmentIndex, uiSubPassIndex);

        const auto& textureDescription = GetTextureView(hAttachment)->GetTexture()->GetDescription();

        // If flags does not include VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT, each element of pAttachments that is used as a depth/stencil attachment by renderPass must have been created with a usage value including VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT.
        // Link: https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkFramebufferCreateInfo-pAttachments-02633
        XII_VERIFY_FRAME_BUFFER(textureDescription.m_BindFlags.IsSet(xiiGALBindFlags::ShadingRate), "The attachment at index {0} is used as a shading rate attachment by sub pass {1} of render pass, but was not created with the xiiGALBindFlags::ShadingRate bind flag.", attachmentReference.m_AttachmentReference.m_uiAttachmentIndex, uiSubPassIndex);

        bIsVRSEnabled = true;
      }
    }
  }

  if (bIsVRSEnabled && !m_AdapterDescription.m_ShadingRateProperties.m_CapabilityFlags.IsSet(xiiGALShadingRateCapabilityFlags::NonSubSampledRenderTarget))
  {
    XII_VERIFY_FRAME_BUFFER(!m_AdapterDescription.m_ShadingRateProperties.m_CapabilityFlags.IsSet(xiiGALShadingRateCapabilityFlags::SubSampledRenderTarget), "One of xiiGALShadingRateCapabilityFlags::NonSubSampledRenderTarget or xiiGALShadingRateCapabilityFlags::SubSampledRenderTarget capabilities must be presented if texture based variable rate shading is supported.");

    for (xiiUInt32 uiAttachmentIndex = 0; uiAttachmentIndex < renderPassDescription.m_Attachments.GetCount(); ++uiAttachmentIndex)
    {
      const auto& attachmentReference = description.m_Attachments[uiAttachmentIndex];

      if (GetTextureView(attachmentReference)->GetDescription().m_ViewType == xiiGALTextureViewType::ShadingRate)
        continue;

      const auto& textureDescription = GetTextureView(attachmentReference)->GetTexture()->GetDescription();

      XII_VERIFY_FRAME_BUFFER(textureDescription.m_MiscFlags.IsSet(xiiGALMiscTextureFlags::Subsampled), "The attachment at index {0} must be created with the xiiGALMiscTextureFlags::Subsampled flag. If the xiiGALShadingRateCapabilityFlags::NonSubSampledRenderTarget capability is not supported, all attachments except the shading rate texture must have been created with a xiiGALMiscTextureFlags::Subsampled flag.");
    }
  }

  xiiGALFramebuffer* pFramebuffer = CreateFramebufferPlatform(description);

  if (pFramebuffer == nullptr)
  {
    return xiiGALFramebufferHandle();
  }
  else
  {
    return xiiGALFramebufferHandle(m_Framebuffers.Insert(pFramebuffer));
  }
}

void xiiGALDevice::DestroyFramebuffer(xiiGALFramebufferHandle hFramebuffer)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALFramebuffer* pFramebuffer = nullptr;

  if (m_Framebuffers.TryGetValue(hFramebuffer, pFramebuffer))
  {
    AddDestroyedObject(GALObjectType::Framebuffer, hFramebuffer);
  }
  else
  {
    xiiLog::Warning("DestroyFramebuffer called on an invalid handle (double free?).");
  }
}

#undef XII_VERIFY_FRAME_BUFFER

#define XII_VERIFY_BOTTOM_LEVEL_AS(expression, ...)            \
  do                                                           \
  {                                                            \
    XII_ASSERT_DEV((expression), __VA_ARGS__);                 \
    if (!(expression)) { return xiiGALBottomLevelASHandle(); } \
  } while (false)

xiiGALBottomLevelASHandle xiiGALDevice::CreateBottomLevelAS(const xiiGALBottomLevelASCreationDescription& description)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  if (description.m_uiCompactedSize > 0U)
  {
    XII_VERIFY_BOTTOM_LEVEL_AS(!description.m_Triangles.IsEmpty() && !description.m_BoundingBoxes.IsEmpty(), "If a non-zero compacted size is given, the Triangles and Bounding Boxes must not be empty.");
    XII_VERIFY_BOTTOM_LEVEL_AS(description.m_BuildASFlags == xiiGALRayTracingBuildASFlags::None, "If a non-zero compacted size is given, the Flags must be xiiGALRayTracingBuildASFlags::None.");
  }
  else
  {
    XII_VERIFY_BOTTOM_LEVEL_AS(((description.m_BoundingBoxes.GetCount() != 0U) ^ (description.m_Triangles.GetCount() != 0U)), "Exactly one of the Bounding Box count {0}, and the Triangles count {1} must be non-zero.", description.m_BoundingBoxes.GetCount(), description.m_Triangles.GetCount());
    XII_VERIFY_BOTTOM_LEVEL_AS(!description.m_BuildASFlags.AreAllSet(xiiGALRayTracingBuildASFlags::PreferFastTrace | xiiGALRayTracingBuildASFlags::PreferFastBuild), "xiiGALRayTracingBuildASFlags::PreferFastTrace and xiiGALRayTracingBuildASFlags::PreferFastBuild are mutually exclusive.");

    for (xiiUInt32 i = 0U; i < description.m_Triangles.GetCount(); ++i)
    {
      const auto& triangle = description.m_Triangles[i];

      XII_VERIFY_BOTTOM_LEVEL_AS(!triangle.m_sGeometryName.IsEmpty(), "The geometry name at triangle {0} must not be empty.", i);
      XII_VERIFY_BOTTOM_LEVEL_AS(triangle.m_VertexValueType == xiiGALValueType::Float32 || triangle.m_VertexValueType == xiiGALValueType::Float16 || triangle.m_VertexValueType == xiiGALValueType::Int32, "The Vertex Value Type specified in triangle {0} is invalid. Allowed types are xiiGALValueType::Float32, xiiGALValueType::Float16 or xiiGALValueType::Int32.", i);
      XII_VERIFY_BOTTOM_LEVEL_AS(triangle.m_uiVertexComponentCount == 2U || triangle.m_uiVertexComponentCount == 3U, "The Vertex Component Count specified in triangle {0} is invalid. Allowed values are 2 or 3.", i);
      XII_VERIFY_BOTTOM_LEVEL_AS(triangle.m_uiMaxVertexCount > 0U, "The Max Vertex Count specified in triangle {0} must be greater than zero.", i);
      XII_VERIFY_BOTTOM_LEVEL_AS(triangle.m_uiMaxPrimitiveCount > 0U, "The Max Primitive Count specified in triangle {0} must be greater than zero.", i);

      if (triangle.m_IndexType == xiiGALValueType::Undefined)
      {
        XII_VERIFY_BOTTOM_LEVEL_AS(triangle.m_uiMaxVertexCount == (triangle.m_uiMaxPrimitiveCount * 3U), "The Max Vertex Count ({0}) specified in triangle {1} must be equal to the Max Vertex Count multiplied by 3 ({2}).", triangle.m_uiMaxVertexCount, i, (triangle.m_uiMaxPrimitiveCount * 3U));
      }
      else
      {
        XII_VERIFY_BOTTOM_LEVEL_AS(triangle.m_IndexType == xiiGALValueType::UInt32 || triangle.m_IndexType == xiiGALValueType::UInt16, "The Index Type specified in triangle {0} must be xiiGALValueType::UInt16 or xiiGALValueType::UInt32.");
      }
    }

    for (xiiUInt32 i = 0U; i < description.m_BoundingBoxes.GetCount(); ++i)
    {
      const auto& boundingBox = description.m_BoundingBoxes[i];

      XII_VERIFY_BOTTOM_LEVEL_AS(!boundingBox.m_sGeometryName.IsEmpty(), "The Geometry Name in bounding box {0} must not be empty.");
      XII_VERIFY_BOTTOM_LEVEL_AS(boundingBox.m_uiMaxBoxCount > 0U, "The Max Box Count in bounding box {0} must be greater than zero.");
    }
  }

  xiiGALBottomLevelAS* pBottomLevelAS = CreateBottomLevelASPlatform(description);

  if (pBottomLevelAS == nullptr)
  {
    return xiiGALBottomLevelASHandle();
  }
  else
  {
    return xiiGALBottomLevelASHandle(m_BottomLevelAccelerationStructures.Insert(pBottomLevelAS));
  }
}

void xiiGALDevice::DestroyBottomLevelAS(xiiGALBottomLevelASHandle hBottomLevelAS)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALBottomLevelAS* pBottomLevelAS = nullptr;

  if (m_BottomLevelAccelerationStructures.TryGetValue(hBottomLevelAS, pBottomLevelAS))
  {
    AddDestroyedObject(GALObjectType::BottomLevelAS, hBottomLevelAS);
  }
  else
  {
    xiiLog::Warning("DestroyBottomLevelAS called on an invalid handle (double free?).");
  }
}

#undef XII_VERIFY_BOTTOM_LEVEL_AS

#define XII_VERIFY_TOP_LEVEL_AS(expression, ...)            \
  do                                                        \
  {                                                         \
    XII_ASSERT_DEV((expression), __VA_ARGS__);              \
    if (!(expression)) { return xiiGALTopLevelASHandle(); } \
  } while (false)

xiiGALTopLevelASHandle xiiGALDevice::CreateTopLevelAS(const xiiGALTopLevelASCreationDescription& description)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  if (description.m_uiCompactedSize > 0U)
  {
    XII_VERIFY_TOP_LEVEL_AS(description.m_uiMaxInstanceCount == 0U, "If a non-zero compacted size is given, the Max Instance Count must be zero.");
    XII_VERIFY_TOP_LEVEL_AS(description.m_Flags == xiiGALRayTracingBuildASFlags::None, "If a non-zero compacted size is given, the specified Flags must be xiiGALRayTracingBuildASFlags::None.");
  }
  else
  {
    XII_VERIFY_TOP_LEVEL_AS(description.m_uiMaxInstanceCount > 0U, "The max instance count must be greater than zero.");
    XII_VERIFY_TOP_LEVEL_AS(!description.m_Flags.AreAllSet(xiiGALRayTracingBuildASFlags::PreferFastTrace | xiiGALRayTracingBuildASFlags::PreferFastBuild), "xiiGALRayTracingBuildASFlags::PreferFastTrace and xiiGALRayTracingBuildASFlags::PreferFastBuild are mutually exclusive.");
  }

  xiiGALTopLevelAS* pTopLevelAS = CreateTopLevelASPlatform(description);

  if (pTopLevelAS == nullptr)
  {
    return xiiGALTopLevelASHandle();
  }
  else
  {
    return xiiGALTopLevelASHandle(m_TopLevelAccelerationStructures.Insert(pTopLevelAS));
  }
}

void xiiGALDevice::DestroyTopLevelAS(xiiGALTopLevelASHandle hTopLevelAS)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALTopLevelAS* pTopLevelAS = nullptr;

  if (m_TopLevelAccelerationStructures.TryGetValue(hTopLevelAS, pTopLevelAS))
  {
    AddDestroyedObject(GALObjectType::TopLevelAS, hTopLevelAS);
  }
  else
  {
    xiiLog::Warning("DestroyTopLevelAS called on an invalid handle (double free?).");
  }
}

#undef XII_VERIFY_TOP_LEVEL_AS

#define XII_VERIFY_PIPELINE_RESOURCE_SIGNATURE(expression, ...)            \
  do                                                                       \
  {                                                                        \
    XII_ASSERT_DEV((expression), __VA_ARGS__);                             \
    if (!(expression)) { return xiiGALPipelineResourceSignatureHandle(); } \
  } while (false)

XII_NODISCARD xiiGALPipelineResourceSignatureHandle xiiGALDevice::CreatePipelineResourceSignature(xiiGALPipelineResourceSignatureCreationDescription& description)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  XII_VERIFY_PIPELINE_RESOURCE_SIGNATURE(description.m_uiBindingIndex < XII_GAL_MAX_RESOURCE_SIGNATURES_COUNT, "The pipeline resource signature binding index ({0}) exceeds the maximum allowed value ({1}).", description.m_uiBindingIndex, XII_GAL_MAX_RESOURCE_SIGNATURES_COUNT - 1);
  XII_VERIFY_PIPELINE_RESOURCE_SIGNATURE(description.m_uiBindingIndex <= s_uiMaxResourcesInSignature, "The pipeline resource signature resource count ({0}) exceeds the maximum allowed value ({1}).", description.m_Resources.GetCount(), s_uiMaxResourcesInSignature);
  XII_VERIFY_PIPELINE_RESOURCE_SIGNATURE(description.m_bUseCombinedTextureSamplers && !description.m_sCombinedSamplerSuffix.IsEmpty(), "The pipeline resource signature is set to use combined texture sampplers, but the combined texture sampler is empty.");

  // Ensure that shader stages do not conflict for resources with the same name.

  xiiMap<xiiHashedString, xiiSet<xiiGALShaderStage::StorageType>> usedResourceShaderStages;
  for (xiiUInt32 i = 0; i < description.m_Resources.GetCount(); ++i)
  {
    const auto& resource = description.m_Resources[i];

    XII_VERIFY_PIPELINE_RESOURCE_SIGNATURE(!resource.m_sName.IsEmpty(), "The pipeline resource at index '{0}' requires a non-empty name.", i);
    XII_VERIFY_PIPELINE_RESOURCE_SIGNATURE(!resource.m_ShaderStages.IsNoFlagSet(), "The pipeline resource at index '{0}' requires a valid shader stage, and must not be xiiGALShaderStage::Unknown.", i);
    XII_VERIFY_PIPELINE_RESOURCE_SIGNATURE(resource.m_uiArraySize > 0U, "The pipeline resource at index '{0}' requires a non-zero array size.", i);

    xiiSet<xiiGALShaderStage::StorageType> shaderStageSet;
    if (usedResourceShaderStages.TryGetValue(resource.m_sName, shaderStageSet))
    {
      XII_VERIFY_PIPELINE_RESOURCE_SIGNATURE(!shaderStageSet.Contains(resource.m_ShaderStages.GetValue()), "Multiple resources with name '{}' found with overlapping shader stages. There may be resources with the same name in different shader stages, but the stages must not overlap.");
    }
    else
    {
      xiiSet<xiiGALShaderStage::StorageType> set;
      set.Insert(resource.m_ShaderStages.GetValue());

      usedResourceShaderStages.Insert(resource.m_sName, set);
    }

    XII_VERIFY_PIPELINE_RESOURCE_SIGNATURE(!(resource.m_PipelineResourceFlags.IsSet(xiiGALPipelineResourceFlags::RuntimeArray) && m_AdapterDescription.m_Features.m_ShaderResourceRuntimeArray == xiiGALDeviceFeatureState::Disabled), "The pipeline resource at index '{0}' specifies the xiiGALPipelineResourceFlags::RuntimeArray flag, which requires the shader resource runtime array device feature.", i);
    XII_VERIFY_PIPELINE_RESOURCE_SIGNATURE(!(resource.m_ResourceVariableType == xiiGALShaderResourceType::AccelerationStructure && m_AdapterDescription.m_Features.m_RayTracing == xiiGALDeviceFeatureState::Disabled), "The pipeline resource at index '{0}' specifies the xiiGALShaderResourceType::AccelerationStructure type, which requires ray tracing device feature.", i);
    XII_VERIFY_PIPELINE_RESOURCE_SIGNATURE(!(resource.m_ResourceVariableType == xiiGALShaderResourceType::InputAttachment && resource.m_ShaderStages != xiiGALShaderStage::Pixel), "The pipeline resource at index '{0}' specifies the xiiGALShaderResourceType::InputAttachment type, but its only supported in the pixel shader stage.", i);

    xiiBitflags<xiiGALPipelineResourceFlags> allowedResourceFlags = xiiGALGraphicsUtilities::GetValidPipelineResourceFlags(resource.m_ResourceType);
    XII_VERIFY_PIPELINE_RESOURCE_SIGNATURE(resource.m_PipelineResourceFlags.IsStrictlyAnySet(allowedResourceFlags), "The pipeline resource at index '{0}' contains flags that are not allowed for the shader resource type.", i);

    if (m_Description.m_GraphicsDeviceType == xiiGALGraphicsDeviceType::Direct3D12 || m_Description.m_GraphicsDeviceType == xiiGALGraphicsDeviceType::Direct3D11 || m_Description.m_GraphicsDeviceType == xiiGALGraphicsDeviceType::Metal)
    {
      XII_VERIFY_PIPELINE_RESOURCE_SIGNATURE(!(resource.m_PipelineResourceFlags.IsSet(xiiGALPipelineResourceFlags::CombinedSampler) && !description.m_bUseCombinedTextureSamplers), "The pipeline resource at index '{0}' specifies the xiiGALPipelineResourceFlags::CombinedSampler flag, but combined sampler usage is disabled. In Direct3D and Metal graphics implementations, the xiiGALPipelineResourceFlags::CombinedSampler flag may only be used when combined sampler usage is enabled.", i);
    }

    XII_VERIFY_PIPELINE_RESOURCE_SIGNATURE(!(resource.m_PipelineResourceFlags.IsSet(xiiGALPipelineResourceFlags::GeneralInputAttachment) && m_Description.m_GraphicsDeviceType != xiiGALGraphicsDeviceType::Vulkan), "The pipeline resource at index '{0}' specifies the xiiGALPipelineResourceFlags::GeneralInputAttachment flag, which is only valid on a Vulkan graphics implementation.", i);
  }

  // Ensure that immutable samplers do not have conflicting shader stages.

  xiiMap<xiiHashedString, xiiSet<xiiGALShaderStage::StorageType>> usedImmutableSamplerShaderStages;
  for (xiiUInt32 i = 0; i < description.m_ImmutableSamplers.GetCount(); ++i)
  {
    auto& samplerDescription = description.m_ImmutableSamplers[i];

    XII_VERIFY_PIPELINE_RESOURCE_SIGNATURE(!samplerDescription.m_SamplerOrTextureName.IsEmpty(), "The immutable sampler at index '{0}' requires a non-empty name.", i);
    XII_VERIFY_PIPELINE_RESOURCE_SIGNATURE(!samplerDescription.m_ShaderStages.IsNoFlagSet(), "The immutable sampler at index '{0}' requires a valid shader stage, and must not be xiiGALShaderStage::Unknown.", i);

    // Use anisotropic filtering if any of Anisotropic is set.
    if (samplerDescription.m_SamplerDescription.m_MagFilter == xiiGALFilterType::Anisotropic || samplerDescription.m_SamplerDescription.m_MinFilter == xiiGALFilterType::Anisotropic || samplerDescription.m_SamplerDescription.m_MipFilter == xiiGALFilterType::Anisotropic)
    {
      if (samplerDescription.m_SamplerDescription.m_ComparisonFunction == xiiGALComparisonFunction::Never)
      {
        samplerDescription.m_SamplerDescription.m_MinFilter = xiiGALFilterType::Anisotropic;
        samplerDescription.m_SamplerDescription.m_MagFilter = xiiGALFilterType::Anisotropic;
        samplerDescription.m_SamplerDescription.m_MipFilter = xiiGALFilterType::Anisotropic;
      }
      else
      {
        samplerDescription.m_SamplerDescription.m_MinFilter = xiiGALFilterType::ComparisonAnisotropic;
        samplerDescription.m_SamplerDescription.m_MagFilter = xiiGALFilterType::ComparisonAnisotropic;
        samplerDescription.m_SamplerDescription.m_MipFilter = xiiGALFilterType::ComparisonAnisotropic;
      }
    }

    xiiSet<xiiGALShaderStage::StorageType> shaderStageSet;
    if (usedImmutableSamplerShaderStages.TryGetValue(samplerDescription.m_SamplerOrTextureName, shaderStageSet))
    {
      XII_VERIFY_PIPELINE_RESOURCE_SIGNATURE(!shaderStageSet.Contains(samplerDescription.m_ShaderStages.GetValue()), "Multiple immutable samplers with name '{}' found with overlapping shader stages. There may be immutable samplers with the same name in different shader stages, but the stages must not overlap.");
    }
    else
    {
      xiiSet<xiiGALShaderStage::StorageType> set;
      set.Insert(samplerDescription.m_ShaderStages.GetValue());

      usedImmutableSamplerShaderStages.Insert(samplerDescription.m_SamplerOrTextureName, set);
    }
  }

  /// \todo GraphicsFoundation: Verify combined texture samplers, all samplers should be assigned to textures when combined texture samplers are used, all immutable samplers should be assigned to textures or samplers when combined texture samplers are used.

  xiiUInt32 uiHash = xiiGALDescriptorHash::Hash(description);
  {
    xiiGALPipelineResourceSignatureHandle hPipelineResourceSignature;
    if (m_PipelineResourceSignatureTable.TryGetValue(uiHash, hPipelineResourceSignature))
    {
      xiiGALPipelineResourceSignature* pPipelineResourceSignature = m_PipelineResourceSignatures[hPipelineResourceSignature];
      if (pPipelineResourceSignature->GetRefCount() == 0)
      {
        ReviveDestroyedObject(GALObjectType::PipelineResourceSignature, hPipelineResourceSignature);
      }

      pPipelineResourceSignature->AddRef();
      return hPipelineResourceSignature;
    }
  }

  xiiGALPipelineResourceSignature* pPipelineResourceSignature = CreatePipelineResourceSignaturePlatform(description);

  if (pPipelineResourceSignature != nullptr)
  {
    XII_ASSERT_DEBUG(xiiGALDescriptorHash::Hash(pPipelineResourceSignature->GetDescription()) == uiHash, "PipelineResourceSignature hash does not match.");

    pPipelineResourceSignature->AddRef();

    xiiGALPipelineResourceSignatureHandle hPipelineResourceSignaure(m_PipelineResourceSignatures.Insert(pPipelineResourceSignature));
    m_PipelineResourceSignatureTable.Insert(uiHash, hPipelineResourceSignaure);

    return hPipelineResourceSignaure;
  }

  return xiiGALPipelineResourceSignatureHandle();
}

void xiiGALDevice::DestroyPipelineResourceSignature(xiiGALPipelineResourceSignatureHandle hPipelineResourceSignature)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALPipelineResourceSignature* pPipelineResourceSignature = nullptr;

  if (m_PipelineResourceSignatures.TryGetValue(hPipelineResourceSignature, pPipelineResourceSignature))
  {
    pPipelineResourceSignature->ReleaseRef();

    if (pPipelineResourceSignature->GetRefCount() == 0)
    {
      AddDestroyedObject(GALObjectType::PipelineResourceSignature, hPipelineResourceSignature);
    }
  }
  else
  {
    xiiLog::Warning("DestroyPipelineResourceSignature called on an invalid handle (double free?).");
  }
}

#undef XII_VERIFY_PIPELINE_RESOURCE_SIGNATURE

#define XII_VERIFY_PIPELINE_STATE(expression, ...)             \
  do                                                           \
  {                                                            \
    XII_ASSERT_DEV((expression), __VA_ARGS__);                 \
    if (!(expression)) { return xiiGALPipelineStateHandle(); } \
  } while (false)

XII_NODISCARD xiiGALPipelineStateHandle xiiGALDevice::CreatePipelineState(const xiiGALPipelineStateCreationDescription& description)
{
  /// \todo GraphicsFoundation: Verify pipeline state description.

  xiiUInt32 uiHash = xiiGALDescriptorHash::Hash(description);
  {
    xiiGALPipelineStateHandle hPipelineState;
    if (m_PipelineStateTable.TryGetValue(uiHash, hPipelineState))
    {
      xiiGALPipelineState* pPipelineState = m_PipelineStates[hPipelineState];
      if (pPipelineState->GetRefCount() == 0)
      {
        ReviveDestroyedObject(GALObjectType::PipelineState, hPipelineState);
      }

      pPipelineState->AddRef();
      return hPipelineState;
    }
  }

  xiiGALPipelineState* pPipelineState = CreatePipelineStatePlatform(description);

  if (pPipelineState != nullptr)
  {
    XII_ASSERT_DEBUG(xiiGALDescriptorHash::Hash(pPipelineState->GetDescription()) == uiHash, "PipelineState hash does not match.");

    pPipelineState->AddRef();

    xiiGALPipelineStateHandle hPipelineResourceSignaure(m_PipelineStates.Insert(pPipelineState));
    m_PipelineStateTable.Insert(uiHash, hPipelineResourceSignaure);

    return hPipelineResourceSignaure;
  }

  return xiiGALPipelineStateHandle();
}

void xiiGALDevice::DestroyPipelineState(xiiGALPipelineStateHandle hPipelineState)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  xiiGALPipelineState* pPipelineState = nullptr;

  if (m_PipelineStates.TryGetValue(hPipelineState, pPipelineState))
  {
    pPipelineState->ReleaseRef();

    if (pPipelineState->GetRefCount() == 0)
    {
      AddDestroyedObject(GALObjectType::PipelineState, hPipelineState);
    }
  }
  else
  {
    xiiLog::Warning("DestroyPipelineState called on an invalid handle (double free?).");
  }
}

#undef XII_VERIFY_PIPELINE_STATE

void xiiGALDevice::WaitIdle()
{
  WaitIdlePlatform();
}

xiiUInt64 xiiGALDevice::GetMemoryConsumptionForTexture(const xiiGALTextureCreationDescription& desc) const
{
  auto& formatProperties = xiiGALGraphicsUtilities::GetTextureFormatProperties(desc.m_Format);

  // This generic implementation is only an approximation, but it can be overridden by specific devices to give an accurate memory consumption figure.
  xiiUInt64 uiMemory = xiiUInt64(desc.m_Size.width) * xiiUInt64(desc.m_Size.height) * xiiUInt64(desc.m_uiArraySizeOrDepth);
  uiMemory *= formatProperties.GetElementSize();
  uiMemory *= desc.m_uiSampleCount;

  // Also account for mip maps
  if (desc.m_uiMipLevels > 1)
  {
    uiMemory += static_cast<xiiUInt64>((1.0 / 3.0) * uiMemory);
  }

  return uiMemory;
}

xiiUInt64 xiiGALDevice::GetMemoryConsumptionForBuffer(const xiiGALBufferCreationDescription& desc) const
{
  return desc.m_uiSize;
}

void xiiGALDevice::FlushDestroyedObjects()
{
  // Can't use range based for here since new objects might be added during iteration
  for (xiiUInt32 i = 0; i < m_DestroyedObjects.GetCount(); ++i)
  {
    const auto& destroyedObject = m_DestroyedObjects[i];

    switch (destroyedObject.m_uiType)
    {
      case GALObjectType::SwapChain:
      {
        xiiGALSwapChainHandle hSwapChain(xiiGALSwapChainHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALSwapChain*      pSwapChain = nullptr;

        XII_VERIFY(m_SwapChains.Remove(hSwapChain, &pSwapChain), "SwapChain not found in idTable.");

        DestroySwapChainPlatform(pSwapChain);
      }
      break;
      case GALObjectType::CommandQueue:
      {
        xiiGALCommandQueueHandle hCommandQueue(xiiGALCommandQueueHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALCommandQueue*      pCommandQueue = nullptr;

        XII_VERIFY(m_CommandQueues.Remove(hCommandQueue, &pCommandQueue), "CommandQueue not found in idTable.");

        DestroyCommandQueuePlatform(pCommandQueue);
      }
      break;
      case GALObjectType::BottomLevelAS:
      {
        xiiGALBottomLevelASHandle hBottomLevelAS(xiiGALBottomLevelASHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALBottomLevelAS*      pBottomLevelAS = nullptr;

        XII_VERIFY(m_BottomLevelAccelerationStructures.Remove(hBottomLevelAS, &pBottomLevelAS), "BottomLevelAS not found in idTable.");

        DestroyBottomLevelASPlatform(pBottomLevelAS);
      }
      break;
      case GALObjectType::Buffer:
      {
        xiiGALBufferHandle hBuffer(xiiGALBufferHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALBuffer*      pBuffer = nullptr;

        XII_VERIFY(m_Buffers.Remove(hBuffer, &pBuffer), "Buffer not found in idTable.");

        DestroyViews(pBuffer);
        DestroyBufferPlatform(pBuffer);
      }
      break;
      case GALObjectType::BufferView:
      {
        xiiGALBufferViewHandle hBufferView(xiiGALBufferViewHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALBufferView*      pBufferView = nullptr;

        m_BufferViews.Remove(hBufferView, &pBufferView);

        xiiGALBuffer* pResource = pBufferView->m_pBuffer;
        XII_ASSERT_DEBUG(pResource != nullptr, "");

        XII_VERIFY(pResource->m_BufferViews.Remove(pBufferView->GetDescription().CalculateHash()), "");
        pBufferView->m_pBuffer = nullptr;

        DestroyBufferViewPlatform(pBufferView);
      }
      break;
      case GALObjectType::Fence:
      {
        xiiGALFenceHandle hFence(xiiGALFenceHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALFence*      pFence = nullptr;

        XII_VERIFY(m_Fences.Remove(hFence, &pFence), "Fence not found in idTable.");

        DestroyFencePlatform(pFence);
      }
      break;
      case GALObjectType::Framebuffer:
      {
        xiiGALFramebufferHandle hFramebuffer(xiiGALFramebufferHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALFramebuffer*      pFramebuffer = nullptr;

        XII_VERIFY(m_Framebuffers.Remove(hFramebuffer, &pFramebuffer), "Framebuffer not found in idTable.");

        DestroyFramebufferPlatform(pFramebuffer);
      }
      break;
      case GALObjectType::Query:
      {
        xiiGALQueryHandle hQuery(xiiGALQueryHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALQuery*      pQuery = nullptr;

        XII_VERIFY(m_Queries.Remove(hQuery, &pQuery), "Query not found in idTable.");

        DestroyQueryPlatform(pQuery);
      }
      break;
      case GALObjectType::RenderPass:
      {
        xiiGALRenderPassHandle hRenderPass(xiiGALRenderPassHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALRenderPass*      pRenderPass = nullptr;

        XII_VERIFY(m_RenderPasses.Remove(hRenderPass, &pRenderPass), "RenderPass not found in idTable.");

        DestroyRenderPassPlatform(pRenderPass);
      }
      break;
      case GALObjectType::Sampler:
      {
        xiiGALSamplerHandle hSampler(xiiGALSamplerHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALSampler*      pSampler = nullptr;

        XII_VERIFY(m_Samplers.Remove(hSampler, &pSampler), "Sampler not found in idTable.");
        XII_VERIFY(m_SamplerTable.Remove(pSampler->GetDescription().CalculateHash()), "Sampler not found in de-duplication table.");

        DestroySamplerPlatform(pSampler);
      }
      break;
      case GALObjectType::Texture:
      {
        xiiGALTextureHandle hTexture(xiiGALTextureHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALTexture*      pTexture = nullptr;

        XII_VERIFY(m_Textures.Remove(hTexture, &pTexture), "Texture not found in idTable.");

        DestroyViews(pTexture);
        DestroyTexturePlatform(pTexture);
      }
      break;
      case GALObjectType::TextureView:
      {
        xiiGALTextureViewHandle hTextureView(xiiGALTextureViewHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALTextureView*      pTextureView = nullptr;

        m_TextureViews.Remove(hTextureView, &pTextureView);

        xiiGALTexture* pResource = pTextureView->m_pTexture;
        XII_ASSERT_DEBUG(pResource != nullptr, "");

        XII_VERIFY(pResource->m_TextureViews.Remove(pTextureView->GetDescription().CalculateHash()), "");
        pTextureView->m_pTexture = nullptr;

        DestroyTextureViewPlatform(pTextureView);
      }
      break;
      case GALObjectType::TopLevelAS:
      {
        xiiGALTopLevelASHandle hTopLevelAS(xiiGALTopLevelASHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALTopLevelAS*      pTopLevelAS = nullptr;

        XII_VERIFY(m_TopLevelAccelerationStructures.Remove(hTopLevelAS, &pTopLevelAS), "TopLevelAS not found in idTable.");

        DestroyTopLevelASPlatform(pTopLevelAS);
      }
      break;
      case GALObjectType::InputLayout:
      {
        xiiGALInputLayoutHandle hInputLayout(xiiGALInputLayoutHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALInputLayout*      pInputLayout = nullptr;

        XII_VERIFY(m_InputLayouts.Remove(hInputLayout, &pInputLayout), "InputLayout not found in idTable.");
        XII_VERIFY(m_InputLayoutTable.Remove(pInputLayout->GetDescription().CalculateHash()), "InputLayout not found in de-duplication table.");

        DestroyInputLayoutPlatform(pInputLayout);
      }
      break;
      case GALObjectType::Shader:
      {
        xiiGALShaderHandle hShader(xiiGALShaderHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALShader*      pShader = nullptr;

        XII_VERIFY(m_Shaders.Remove(hShader, &pShader), "Shader not found in idTable.");

        DestroyShaderPlatform(pShader);
      }
      break;
      case GALObjectType::BlendState:
      {
        xiiGALBlendStateHandle hBlendState(xiiGALBlendStateHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALBlendState*      pBlendState = nullptr;

        XII_VERIFY(m_BlendStates.Remove(hBlendState, &pBlendState), "BlendState not found in idTable.");
        XII_VERIFY(m_BlendStateTable.Remove(pBlendState->GetDescription().CalculateHash()), "BlendState not found in de-duplication table.");

        DestroyBlendStatePlatform(pBlendState);
      }
      break;
      case GALObjectType::DepthStencilState:
      {
        xiiGALDepthStencilStateHandle hDepthStencilState(xiiGALDepthStencilStateHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALDepthStencilState*      pDepthStencilState = nullptr;

        XII_VERIFY(m_DepthStencilStates.Remove(hDepthStencilState, &pDepthStencilState), "DepthStencilState not found in idTable.");
        XII_VERIFY(m_DepthStencilStateTable.Remove(pDepthStencilState->GetDescription().CalculateHash()), "DepthStencilState not found in de-duplication table.");

        DestroyDepthStencilStatePlatform(pDepthStencilState);
      }
      break;
      case GALObjectType::RasterizerState:
      {
        xiiGALRasterizerStateHandle hRasterizerState(xiiGALRasterizerStateHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALRasterizerState*      pRasterizerState = nullptr;

        XII_VERIFY(m_RasterizerStates.Remove(hRasterizerState, &pRasterizerState), "RasterizerState not found in idTable.");
        XII_VERIFY(m_RasterizerStateTable.Remove(pRasterizerState->GetDescription().CalculateHash()), "RasterizerState not found in de-duplication table.");

        DestroyRasterizerStatePlatform(pRasterizerState);
      }
      break;
      case GALObjectType::PipelineResourceSignature:
      {
        xiiGALPipelineResourceSignatureHandle hPipelineResourceSignature(xiiGALPipelineResourceSignatureHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALPipelineResourceSignature*      pPipelineResourceSignature = nullptr;

        XII_VERIFY(m_PipelineResourceSignatures.Remove(hPipelineResourceSignature, &pPipelineResourceSignature), "PipelineResourceSignature not found in idTable.");
        XII_VERIFY(m_PipelineResourceSignatureTable.Remove(xiiGALDescriptorHash::Hash(pPipelineResourceSignature->GetDescription())), "PipelineResourceSignature not found in de-duplication table.");

        DestroyPipelineResourceSignaturePlatform(pPipelineResourceSignature);
      }
      break;
      case GALObjectType::PipelineState:
      {
        xiiGALPipelineStateHandle hPipelineState(xiiGALPipelineStateHandle::IdType(destroyedObject.m_uiHandle));
        xiiGALPipelineState*      pPipelineState = nullptr;

        XII_VERIFY(m_PipelineStates.Remove(hPipelineState, &pPipelineState), "PipelineState not found in idTable.");
        XII_VERIFY(m_PipelineStateTable.Remove(xiiGALDescriptorHash::Hash(pPipelineState->GetDescription())), "PipelineState not found in de-duplication table.");

        DestroyPipelineStatePlatform(pPipelineState);
      }
      break;

        XII_DEFAULT_CASE_NOT_IMPLEMENTED;
    }
  }

  m_DestroyedObjects.Clear();
}

void xiiGALDevice::DestroyViews(xiiGALResource* pResource)
{
  XII_GAL_DEVICE_LOCK_AND_CHECK();

  for (auto it = pResource->m_BufferViews.GetIterator(); it.IsValid(); ++it)
  {
    xiiGALBufferViewHandle hBufferView = it.Value();
    xiiGALBufferView*      pBufferView = m_BufferViews[hBufferView];

    XII_VERIFY(m_BufferViews.Remove(hBufferView), "");

    DestroyBufferViewPlatform(pBufferView);
  }
  pResource->m_BufferViews.Clear();

  for (xiiUInt32 i = 0; i < xiiGALBufferViewType::ENUM_COUNT; ++i)
  {
    pResource->m_DefaultBufferViews[i].Invalidate();
  }

  for (auto it = pResource->m_TextureViews.GetIterator(); it.IsValid(); ++it)
  {
    xiiGALTextureViewHandle hTextureView = it.Value();
    xiiGALTextureView*      pTextureView = m_TextureViews[hTextureView];

    XII_VERIFY(m_TextureViews.Remove(hTextureView), "");

    DestroyTextureViewPlatform(pTextureView);
  }
  pResource->m_TextureViews.Clear();

  for (xiiUInt32 i = 0; i < xiiGALBufferViewType::ENUM_COUNT; ++i)
  {
    pResource->m_DefaultTextureViews[i].Invalidate();
  }
}

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_Device_Implementation_Device);
