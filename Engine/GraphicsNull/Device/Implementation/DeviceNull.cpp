#include <GraphicsNull/GraphicsNullPCH.h>

#include <Foundation/Configuration/Startup.h>
#include <GraphicsFoundation/Device/DeviceFactory.h>
#include <GraphicsFoundation/Profiling/Profiling.h>

#include <GraphicsNull/CommandEncoder/CommandListNull.h>
#include <GraphicsNull/CommandEncoder/CommandQueueNull.h>
#include <GraphicsNull/Device/DeviceNull.h>
#include <GraphicsNull/Device/SwapChainNull.h>
#include <GraphicsNull/Resources/BottomLevelASNull.h>
#include <GraphicsNull/Resources/BufferNull.h>
#include <GraphicsNull/Resources/BufferViewNull.h>
#include <GraphicsNull/Resources/FenceNull.h>
#include <GraphicsNull/Resources/FramebufferNull.h>
#include <GraphicsNull/Resources/QueryNull.h>
#include <GraphicsNull/Resources/RenderPassNull.h>
#include <GraphicsNull/Resources/SamplerNull.h>
#include <GraphicsNull/Resources/TextureNull.h>
#include <GraphicsNull/Resources/TextureViewNull.h>
#include <GraphicsNull/Resources/TopLevelASNull.h>
#include <GraphicsNull/Shader/InputLayoutNull.h>
#include <GraphicsNull/Shader/ShaderNull.h>
#include <GraphicsNull/States/BlendStateNull.h>
#include <GraphicsNull/States/DepthStencilStateNull.h>
#include <GraphicsNull/States/PipelineResourceSignatureNull.h>
#include <GraphicsNull/States/PipelineStateNull.h>
#include <GraphicsNull/States/RasterizerStateNull.h>

xiiInternal::NewInstance<xiiGALDevice> CreateNullDevice(xiiAllocatorBase* pAllocator, const xiiGALDeviceCreationDescription& description)
{
  return XII_NEW(pAllocator, xiiGALDeviceNull, description);
}

// clang-format off
XII_BEGIN_SUBSYSTEM_DECLARATION(GraphicsNull, DeviceFactory)

ON_CORESYSTEMS_STARTUP
{
  const xiiGALDeviceImplementationDescription implementation = {.m_APIType = xiiGALGraphicsDeviceType::Null, .m_sShaderModel = "NULL_SM", .m_sShaderCompiler = "xiiShaderCompiler" };

  xiiGALDeviceFactory::RegisterImplementation("Null", &CreateNullDevice, implementation);
}

ON_CORESYSTEMS_SHUTDOWN
{
  xiiGALDeviceFactory::UnregisterImplementation("Null");
}

XII_END_SUBSYSTEM_DECLARATION;
// clang-format on

xiiGALDeviceNull::xiiGALDeviceNull(const xiiGALDeviceCreationDescription& description) :
  xiiGALDevice(description)
{
}

xiiGALDeviceNull::~xiiGALDeviceNull() = default;

xiiResult xiiGALDeviceNull::InitializePlatform()
{
  XII_LOG_BLOCK("xiiGALDeviceNull::InitializePlatform");

  xiiClipSpaceDepthRange::Default           = xiiClipSpaceDepthRange::ZeroToOne;
  xiiClipSpaceYMode::RenderToTextureDefault = xiiClipSpaceYMode::Regular;

  return XII_SUCCESS;
}

void xiiGALDeviceNull::ReportLiveGPUObjects()
{
}

void xiiGALDeviceNull::FlushPendingObjects()
{
  FlushDestroyedObjects();
}

xiiResult xiiGALDeviceNull::ShutdownPlatform()
{
  m_pDefaultQueue.Clear();

  return XII_SUCCESS;
}

void xiiGALDeviceNull::BeginPipelinePlatform(xiiStringView sName, xiiGALSwapChain* pSwapChain)
{
  if (pSwapChain)
  {
    pSwapChain->AcquireNextRenderTarget();
  }
}

void xiiGALDeviceNull::EndPipelinePlatform(xiiGALSwapChain* pSwapChain)
{
  if (pSwapChain)
  {
    pSwapChain->Present();
  }
}

void xiiGALDeviceNull::BeginFramePlatform(const xiiUInt64 uiRenderFrame)
{
}

void xiiGALDeviceNull::EndFramePlatform()
{
  ++m_uiFrameNumber;
}

xiiGALSwapChain* xiiGALDeviceNull::CreateSwapChainPlatform(const xiiGALSwapChainCreationDescription& description)
{
  xiiGALSwapChainNull* pSwapChainNull = XII_NEW(&m_Allocator, xiiGALSwapChainNull, this, description);

  if (pSwapChainNull->InitPlatform().Succeeded())
    return pSwapChainNull;

  XII_DELETE(&m_Allocator, pSwapChainNull);

  return pSwapChainNull;
}

void xiiGALDeviceNull::DestroySwapChainPlatform(xiiGALSwapChain* pSwapChain)
{
  xiiGALSwapChainNull* pSwapChainNull = static_cast<xiiGALSwapChainNull*>(pSwapChain);

  pSwapChainNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pSwapChainNull);
}

xiiGALCommandQueue* xiiGALDeviceNull::CreateCommandQueuePlatform(const xiiGALCommandQueueCreationDescription& description)
{
  xiiGALCommandQueueNull* pCommandQueueNull = XII_NEW(&m_Allocator, xiiGALCommandQueueNull, this, description);

  if (pCommandQueueNull->InitPlatform().Succeeded())
    return pCommandQueueNull;

  XII_DELETE(&m_Allocator, pCommandQueueNull);

  return pCommandQueueNull;
}

void xiiGALDeviceNull::DestroyCommandQueuePlatform(xiiGALCommandQueue* pCommandQueue)
{
  xiiGALCommandQueueNull* pCommandQueueNull = static_cast<xiiGALCommandQueueNull*>(pCommandQueue);

  pCommandQueueNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pCommandQueueNull);
}

xiiGALBlendState* xiiGALDeviceNull::CreateBlendStatePlatform(const xiiGALBlendStateCreationDescription& description)
{
  xiiGALBlendStateNull* pBlendStateNull = XII_NEW(&m_Allocator, xiiGALBlendStateNull, this, description);

  if (pBlendStateNull->InitPlatform().Succeeded())
    return pBlendStateNull;

  XII_DELETE(&m_Allocator, pBlendStateNull);

  return pBlendStateNull;
}

void xiiGALDeviceNull::DestroyBlendStatePlatform(xiiGALBlendState* pBlendState)
{
  xiiGALBlendStateNull* pBlendStateNull = static_cast<xiiGALBlendStateNull*>(pBlendState);

  pBlendStateNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pBlendStateNull);
}

xiiGALDepthStencilState* xiiGALDeviceNull::CreateDepthStencilStatePlatform(const xiiGALDepthStencilStateCreationDescription& description)
{
  xiiGALDepthStencilStateNull* pDepthStencilStateNull = XII_NEW(&m_Allocator, xiiGALDepthStencilStateNull, this, description);

  if (pDepthStencilStateNull->InitPlatform().Succeeded())
    return pDepthStencilStateNull;

  XII_DELETE(&m_Allocator, pDepthStencilStateNull);

  return pDepthStencilStateNull;
}

void xiiGALDeviceNull::DestroyDepthStencilStatePlatform(xiiGALDepthStencilState* pDepthStencilState)
{
  xiiGALDepthStencilStateNull* pDepthStencilStateNull = static_cast<xiiGALDepthStencilStateNull*>(pDepthStencilState);

  pDepthStencilStateNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pDepthStencilStateNull);
}

xiiGALRasterizerState* xiiGALDeviceNull::CreateRasterizerStatePlatform(const xiiGALRasterizerStateCreationDescription& description)
{
  xiiGALRasterizerStateNull* pRasterizerStateNull = XII_NEW(&m_Allocator, xiiGALRasterizerStateNull, this, description);

  if (pRasterizerStateNull->InitPlatform().Succeeded())
    return pRasterizerStateNull;

  XII_DELETE(&m_Allocator, pRasterizerStateNull);

  return pRasterizerStateNull;
}

void xiiGALDeviceNull::DestroyRasterizerStatePlatform(xiiGALRasterizerState* pRasterizerState)
{
  xiiGALRasterizerStateNull* pRasterizerStateNull = static_cast<xiiGALRasterizerStateNull*>(pRasterizerState);

  pRasterizerStateNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pRasterizerStateNull);
}

xiiGALShader* xiiGALDeviceNull::CreateShaderPlatform(const xiiGALShaderCreationDescription& description)
{
  xiiGALShaderNull* pShaderNull = XII_NEW(&m_Allocator, xiiGALShaderNull, this, description);

  if (pShaderNull->InitPlatform().Succeeded())
    return pShaderNull;

  XII_DELETE(&m_Allocator, pShaderNull);

  return pShaderNull;
}

void xiiGALDeviceNull::DestroyShaderPlatform(xiiGALShader* pShader)
{
  xiiGALShaderNull* pShaderNull = static_cast<xiiGALShaderNull*>(pShader);

  pShaderNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pShaderNull);
}

xiiGALBuffer* xiiGALDeviceNull::CreateBufferPlatform(const xiiGALBufferCreationDescription& description, const xiiGALBufferData* pInitialData)
{
  xiiGALBufferNull* pBufferNull = XII_NEW(&m_Allocator, xiiGALBufferNull, this, description);

  if (pBufferNull->InitPlatform(pInitialData).Succeeded())
    return pBufferNull;

  XII_DELETE(&m_Allocator, pBufferNull);

  return pBufferNull;
}

void xiiGALDeviceNull::DestroyBufferPlatform(xiiGALBuffer* pBuffer)
{
  xiiGALBufferNull* pBufferNull = static_cast<xiiGALBufferNull*>(pBuffer);

  pBufferNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pBufferNull);
}

xiiGALBufferView* xiiGALDeviceNull::CreateBufferViewPlatform(xiiGALBuffer* pBuffer, const xiiGALBufferViewCreationDescription& description)
{
  xiiGALBufferViewNull* pBufferViewNull = XII_NEW(&m_Allocator, xiiGALBufferViewNull, this, pBuffer, description);

  if (pBufferViewNull->InitPlatform().Succeeded())
    return pBufferViewNull;

  XII_DELETE(&m_Allocator, pBufferViewNull);

  return pBufferViewNull;
}

void xiiGALDeviceNull::DestroyBufferViewPlatform(xiiGALBufferView* pBufferView)
{
  xiiGALBufferViewNull* pBufferViewNull = static_cast<xiiGALBufferViewNull*>(pBufferView);

  pBufferViewNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pBufferViewNull);
}

xiiGALTexture* xiiGALDeviceNull::CreateTexturePlatform(const xiiGALTextureCreationDescription& description, const xiiGALTextureData* pInitialData)
{
  xiiGALTextureNull* pTextureNull = XII_NEW(&m_Allocator, xiiGALTextureNull, this, description);

  if (pTextureNull->InitPlatform(pInitialData).Succeeded())
    return pTextureNull;

  XII_DELETE(&m_Allocator, pTextureNull);

  return pTextureNull;
}

void xiiGALDeviceNull::DestroyTexturePlatform(xiiGALTexture* pTexture)
{
  xiiGALTextureNull* pTextureNull = static_cast<xiiGALTextureNull*>(pTexture);

  pTextureNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pTextureNull);
}

xiiGALTextureView* xiiGALDeviceNull::CreateTextureViewPlatform(xiiGALTexture* pTexture, const xiiGALTextureViewCreationDescription& description)
{
  xiiGALTextureViewNull* pTextureViewNull = XII_NEW(&m_Allocator, xiiGALTextureViewNull, this, pTexture, description);

  if (pTextureViewNull->InitPlatform().Succeeded())
    return pTextureViewNull;

  XII_DELETE(&m_Allocator, pTextureViewNull);

  return pTextureViewNull;
}

void xiiGALDeviceNull::DestroyTextureViewPlatform(xiiGALTextureView* pTextureView)
{
  xiiGALTextureViewNull* pTextureViewNull = static_cast<xiiGALTextureViewNull*>(pTextureView);

  pTextureViewNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pTextureViewNull);
}

xiiGALSampler* xiiGALDeviceNull::CreateSamplerPlatform(const xiiGALSamplerCreationDescription& description)
{
  xiiGALSamplerNull* pSamplerNull = XII_NEW(&m_Allocator, xiiGALSamplerNull, this, description);

  if (pSamplerNull->InitPlatform().Succeeded())
    return pSamplerNull;

  XII_DELETE(&m_Allocator, pSamplerNull);

  return pSamplerNull;
}

void xiiGALDeviceNull::DestroySamplerPlatform(xiiGALSampler* pSampler)
{
  xiiGALSamplerNull* pSamplerNull = static_cast<xiiGALSamplerNull*>(pSampler);

  pSamplerNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pSamplerNull);
}

xiiGALInputLayout* xiiGALDeviceNull::CreateInputLayoutPlatform(const xiiGALInputLayoutCreationDescription& description)
{
  xiiGALInputLayoutNull* pInputLayoutNull = XII_NEW(&m_Allocator, xiiGALInputLayoutNull, this, description);

  if (pInputLayoutNull->InitPlatform().Succeeded())
    return pInputLayoutNull;

  XII_DELETE(&m_Allocator, pInputLayoutNull);

  return pInputLayoutNull;
}

void xiiGALDeviceNull::DestroyInputLayoutPlatform(xiiGALInputLayout* pInputLayout)
{
  xiiGALInputLayoutNull* pInputLayoutNull = static_cast<xiiGALInputLayoutNull*>(pInputLayout);

  pInputLayoutNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pInputLayoutNull);
}

xiiGALQuery* xiiGALDeviceNull::CreateQueryPlatform(const xiiGALQueryCreationDescription& description)
{
  xiiGALQueryNull* pQueryNull = XII_NEW(&m_Allocator, xiiGALQueryNull, this, description);

  if (pQueryNull->InitPlatform().Succeeded())
    return pQueryNull;

  XII_DELETE(&m_Allocator, pQueryNull);

  return pQueryNull;
}

void xiiGALDeviceNull::DestroyQueryPlatform(xiiGALQuery* pQuery)
{
  xiiGALQueryNull* pQueryNull = static_cast<xiiGALQueryNull*>(pQuery);

  pQueryNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pQueryNull);
}

xiiGALFence* xiiGALDeviceNull::CreateFencePlatform(const xiiGALFenceCreationDescription& description)
{
  xiiGALFenceNull* pFenceNull = XII_NEW(&m_Allocator, xiiGALFenceNull, this, description);

  if (pFenceNull->InitPlatform().Succeeded())
    return pFenceNull;

  XII_DELETE(&m_Allocator, pFenceNull);

  return pFenceNull;
}

void xiiGALDeviceNull::DestroyFencePlatform(xiiGALFence* pFence)
{
  xiiGALFenceNull* pFenceNull = static_cast<xiiGALFenceNull*>(pFence);

  pFenceNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pFenceNull);
}

xiiGALRenderPass* xiiGALDeviceNull::CreateRenderPassPlatform(const xiiGALRenderPassCreationDescription& description)
{
  xiiGALRenderPassNull* pRenderPassNull = XII_NEW(&m_Allocator, xiiGALRenderPassNull, this, description);

  if (pRenderPassNull->InitPlatform().Succeeded())
    return pRenderPassNull;

  XII_DELETE(&m_Allocator, pRenderPassNull);

  return pRenderPassNull;
}

void xiiGALDeviceNull::DestroyRenderPassPlatform(xiiGALRenderPass* pRenderPass)
{
  xiiGALRenderPassNull* pRenderPassNull = static_cast<xiiGALRenderPassNull*>(pRenderPass);

  pRenderPassNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pRenderPassNull);
}

xiiGALFramebuffer* xiiGALDeviceNull::CreateFramebufferPlatform(const xiiGALFramebufferCreationDescription& description)
{
  xiiGALFramebufferNull* pFramebufferNull = XII_NEW(&m_Allocator, xiiGALFramebufferNull, this, description);

  if (pFramebufferNull->InitPlatform().Succeeded())
    return pFramebufferNull;

  XII_DELETE(&m_Allocator, pFramebufferNull);

  return pFramebufferNull;
}

void xiiGALDeviceNull::DestroyFramebufferPlatform(xiiGALFramebuffer* pFramebuffer)
{
  xiiGALFramebufferNull* pFramebufferNull = static_cast<xiiGALFramebufferNull*>(pFramebuffer);

  pFramebufferNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pFramebufferNull);
}

xiiGALBottomLevelAS* xiiGALDeviceNull::CreateBottomLevelASPlatform(const xiiGALBottomLevelASCreationDescription& description)
{
  xiiGALBottomLevelASNull* pBottomLevelASNull = XII_NEW(&m_Allocator, xiiGALBottomLevelASNull, this, description);

  if (pBottomLevelASNull->InitPlatform().Succeeded())
    return pBottomLevelASNull;

  XII_DELETE(&m_Allocator, pBottomLevelASNull);

  return pBottomLevelASNull;
}

void xiiGALDeviceNull::DestroyBottomLevelASPlatform(xiiGALBottomLevelAS* pBottomLevelAS)
{
  xiiGALBottomLevelASNull* pBottomLevelASNull = static_cast<xiiGALBottomLevelASNull*>(pBottomLevelAS);

  pBottomLevelASNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pBottomLevelASNull);
}

xiiGALTopLevelAS* xiiGALDeviceNull::CreateTopLevelASPlatform(const xiiGALTopLevelASCreationDescription& description)
{
  xiiGALTopLevelASNull* pTopLevelASNull = XII_NEW(&m_Allocator, xiiGALTopLevelASNull, this, description);

  if (pTopLevelASNull->InitPlatform().Succeeded())
    return pTopLevelASNull;

  XII_DELETE(&m_Allocator, pTopLevelASNull);

  return pTopLevelASNull;
}

void xiiGALDeviceNull::DestroyTopLevelASPlatform(xiiGALTopLevelAS* pTopLevelAS)
{
  xiiGALTopLevelASNull* pTopLevelASNull = static_cast<xiiGALTopLevelASNull*>(pTopLevelAS);

  pTopLevelASNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pTopLevelASNull);
}

xiiGALPipelineResourceSignature* xiiGALDeviceNull::CreatePipelineResourceSignaturePlatform(const xiiGALPipelineResourceSignatureCreationDescription& description)
{
  xiiGALPipelineResourceSignatureNull* pPipelineResourceSignatureNull = XII_NEW(&m_Allocator, xiiGALPipelineResourceSignatureNull, this, description);

  if (pPipelineResourceSignatureNull->InitPlatform().Succeeded())
    return pPipelineResourceSignatureNull;

  XII_DELETE(&m_Allocator, pPipelineResourceSignatureNull);

  return pPipelineResourceSignatureNull;
}

void xiiGALDeviceNull::DestroyPipelineResourceSignaturePlatform(xiiGALPipelineResourceSignature* pPipelineResourceSignature)
{
  xiiGALPipelineResourceSignatureNull* pPipelineResourceSignatureNull = static_cast<xiiGALPipelineResourceSignatureNull*>(pPipelineResourceSignature);

  pPipelineResourceSignatureNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pPipelineResourceSignatureNull);
}

xiiGALPipelineState* xiiGALDeviceNull::CreatePipelineStatePlatform(const xiiGALPipelineStateCreationDescription& description)
{
  xiiGALPipelineStateNull* pPipelineStateNull = XII_NEW(&m_Allocator, xiiGALPipelineStateNull, this, description);

  if (pPipelineStateNull->InitPlatform().Succeeded())
    return pPipelineStateNull;

  XII_DELETE(&m_Allocator, pPipelineStateNull);

  return pPipelineStateNull;
}

void xiiGALDeviceNull::DestroyPipelineStatePlatform(xiiGALPipelineState* pPipelineState)
{
  xiiGALPipelineStateNull* pPipelineStateNull = static_cast<xiiGALPipelineStateNull*>(pPipelineState);

  pPipelineStateNull->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pPipelineStateNull);
}

void xiiGALDeviceNull::WaitIdlePlatform()
{
  FlushPendingObjects();
}

xiiResult xiiGALDeviceNull::CreateCommandQueuesPlatform()
{
  xiiGALCommandQueueCreationDescription queueDescription = {.m_QueueType = xiiGALCommandQueueType::Graphics};

  m_pDefaultQueue = XII_NEW(&m_Allocator, xiiGALCommandQueueNull, this, queueDescription);

  return XII_SUCCESS;
}

void xiiGALDeviceNull::FillCapabilitiesPlatform()
{
  m_Description.m_GraphicsDeviceType = xiiGALGraphicsDeviceType::Null;

  m_AdapterDescription.m_sAdapterName = "XII Null Graphics Adapter";
  m_AdapterDescription.m_Type         = xiiGALDeviceAdapterType::Software;
  m_AdapterDescription.m_Vendor       = xiiGALGraphicsAdapterVendor::Unknown;

  m_AdapterDescription.m_uiVendorID         = 12;
  m_AdapterDescription.m_uiDeviceID         = 22;
  m_AdapterDescription.m_uiVideoOutputCount = 0;

  // Memory properties

  m_AdapterDescription.m_MemoryProperties.m_uiLocalMemory               = 8423211008;
  m_AdapterDescription.m_MemoryProperties.m_uiHostVisibleMemory         = 17001539584;
  m_AdapterDescription.m_MemoryProperties.m_uiUnifiedMemory             = 0;
  m_AdapterDescription.m_MemoryProperties.m_uiMaxMemoryAllocation       = 0;
  m_AdapterDescription.m_MemoryProperties.m_UnifiedMemoryCPUAccessFlags = xiiGALCPUAccessFlag::None;
  m_AdapterDescription.m_MemoryProperties.m_MemorylessTextureBindFlags  = xiiGALBindFlags::None;

  // Raytracing properties

  m_AdapterDescription.m_RayTracingProperties.m_uiMaxRecursionDepth        = 31;
  m_AdapterDescription.m_RayTracingProperties.m_uiMaxRayGenThreads         = 1073741824;
  m_AdapterDescription.m_RayTracingProperties.m_uiMaxInstancesPerTLAS      = 16777216;
  m_AdapterDescription.m_RayTracingProperties.m_uiMaxPrimitivesPerBLAS     = 536870912;
  m_AdapterDescription.m_RayTracingProperties.m_uiMaxGeometriesPerBLAS     = 16777216;
  m_AdapterDescription.m_RayTracingProperties.m_uiVertexBufferAlignment    = 1;
  m_AdapterDescription.m_RayTracingProperties.m_uiIndexBufferAlignment     = 1;
  m_AdapterDescription.m_RayTracingProperties.m_uiTransformBufferAlignment = 16;
  m_AdapterDescription.m_RayTracingProperties.m_uiBoxBufferAlignment       = 8;
  m_AdapterDescription.m_RayTracingProperties.m_uiScratchBufferAlignment   = 256;
  m_AdapterDescription.m_RayTracingProperties.m_uiInstanceBufferAlignment  = 16;
  m_AdapterDescription.m_RayTracingProperties.m_uiShaderGroupHandleSize    = 32;
  m_AdapterDescription.m_RayTracingProperties.m_uiMaxShaderRecordStride    = 4096;
  m_AdapterDescription.m_RayTracingProperties.m_uiShaderGroupBaseAlignment = 64;
  m_AdapterDescription.m_RayTracingProperties.m_CapabilityFlags            = xiiGALRayTracingCapabilityFlags::StandaloneShaders | xiiGALRayTracingCapabilityFlags::InlineRayTracing | xiiGALRayTracingCapabilityFlags::IndirectRayTracing;

  // Wave operation properties

  m_AdapterDescription.m_WaveOperationProperties.m_uiMinSize             = 32;
  m_AdapterDescription.m_WaveOperationProperties.m_uiMaxSize             = 32;
  m_AdapterDescription.m_WaveOperationProperties.m_WaveFeatures          = xiiGALWaveFeature::Basic | xiiGALWaveFeature::Vote | xiiGALWaveFeature::Arithmetic | xiiGALWaveFeature::BallOut | xiiGALWaveFeature::Quad;
  m_AdapterDescription.m_WaveOperationProperties.m_SupportedShaderStages = xiiGALShaderStage::Pixel | xiiGALShaderStage::Compute | xiiGALShaderStage::Amplification | xiiGALShaderStage::Mesh;

  // Buffer properties

  m_AdapterDescription.m_BufferProperties.m_uiConstantBufferAlignment         = 256;
  m_AdapterDescription.m_BufferProperties.m_uiStructuredBufferOffsetAlignment = 16;

  // Texture properties

  m_AdapterDescription.m_TextureProperties.m_uiMaxTexture1DDimension     = 16384;
  m_AdapterDescription.m_TextureProperties.m_uiMaxTexture1DArraySlices   = 2048;
  m_AdapterDescription.m_TextureProperties.m_uiMaxTexture2DDimension     = 16384;
  m_AdapterDescription.m_TextureProperties.m_uiMaxTexture2DArraySlices   = 2048;
  m_AdapterDescription.m_TextureProperties.m_uiMaxTexture3DDimension     = 2048;
  m_AdapterDescription.m_TextureProperties.m_uiMaxTextureCubeDimension   = 16384;
  m_AdapterDescription.m_TextureProperties.m_bTexture2DMSSupported       = true;
  m_AdapterDescription.m_TextureProperties.m_bTexture2DMSArraySupported  = true;
  m_AdapterDescription.m_TextureProperties.m_bTextureViewSupported       = true;
  m_AdapterDescription.m_TextureProperties.m_bCubeMapArraysSupported     = true;
  m_AdapterDescription.m_TextureProperties.m_bTextureView2DOn3DSupported = true;

  // Sampler properties

  m_AdapterDescription.m_SamplerProperties.m_bBorderSamplingModeSupported   = true;
  m_AdapterDescription.m_SamplerProperties.m_bAnisotropicFilteringSupported = true;
  m_AdapterDescription.m_SamplerProperties.m_bLODBiasSupported              = true;

  // Mesh shader properties

  m_AdapterDescription.m_MeshShaderProperties.m_uiMaxThreadGroupCountX     = 65536;
  m_AdapterDescription.m_MeshShaderProperties.m_uiMaxThreadGroupCountY     = 65536;
  m_AdapterDescription.m_MeshShaderProperties.m_uiMaxThreadGroupCountZ     = 65536;
  m_AdapterDescription.m_MeshShaderProperties.m_uiMaxThreadGroupTotalCount = 4194304;

  // Shading rate properties

  // Null graphics device does not handle shading rates yet.

  // Compute shader properties

  m_AdapterDescription.m_ComputeShaderProperties.m_uiSharedMemorySize          = 32768;
  m_AdapterDescription.m_ComputeShaderProperties.m_uiMaxThreadGroupInvocations = 1024;
  m_AdapterDescription.m_ComputeShaderProperties.m_uiMaxThreadGroupSizeX       = 1024;
  m_AdapterDescription.m_ComputeShaderProperties.m_uiMaxThreadGroupSizeY       = 1024;
  m_AdapterDescription.m_ComputeShaderProperties.m_uiMaxThreadGroupSizeZ       = 64;
  m_AdapterDescription.m_ComputeShaderProperties.m_uiMaxThreadGroupCountX      = 65535;
  m_AdapterDescription.m_ComputeShaderProperties.m_uiMaxThreadGroupCountY      = 65535;
  m_AdapterDescription.m_ComputeShaderProperties.m_uiMaxThreadGroupCountZ      = 65535;

  // Draw command properties

  m_AdapterDescription.m_DrawCommandProperties.m_CapabilityFlags        = xiiGALDrawCommandCapabilityFlags::DrawIndirect | xiiGALDrawCommandCapabilityFlags::DrawIndirectFirstInstance | xiiGALDrawCommandCapabilityFlags::NativeMultiDrawIndirect | xiiGALDrawCommandCapabilityFlags::DrawIndirectCounterBuffer;
  m_AdapterDescription.m_DrawCommandProperties.m_uiMaxIndexValue        = 4294967295;
  m_AdapterDescription.m_DrawCommandProperties.m_uiMaxDrawIndirectCount = 4294967295;

  // Sparse resource properties

  // Null graphics device does not handle sparse resources yet.

  // Device features support.

  m_AdapterDescription.m_Features.m_SeparablePrograms             = xiiGALDeviceFeatureState::Enabled;
  m_AdapterDescription.m_Features.m_WireframeFill                 = xiiGALDeviceFeatureState::Enabled;
  m_AdapterDescription.m_Features.m_MultithreadedResourceCreation = xiiGALDeviceFeatureState::Enabled;
  m_AdapterDescription.m_Features.m_ComputeShaders                = xiiGALDeviceFeatureState::Enabled;
  m_AdapterDescription.m_Features.m_GeometryShaders               = xiiGALDeviceFeatureState::Enabled;
  m_AdapterDescription.m_Features.m_Tessellation                  = xiiGALDeviceFeatureState::Enabled;
  m_AdapterDescription.m_Features.m_MeshShaders                   = xiiGALDeviceFeatureState::Enabled;
  m_AdapterDescription.m_Features.m_RayTracing                    = xiiGALDeviceFeatureState::Enabled;
  m_AdapterDescription.m_Features.m_OcclusionQueries              = xiiGALDeviceFeatureState::Enabled;
  m_AdapterDescription.m_Features.m_BinaryOcclusionQueries        = xiiGALDeviceFeatureState::Enabled;
  m_AdapterDescription.m_Features.m_TimestampQueries              = xiiGALDeviceFeatureState::Enabled;
  m_AdapterDescription.m_Features.m_PipelineStatisticsQueries     = xiiGALDeviceFeatureState::Enabled;
  m_AdapterDescription.m_Features.m_DurationQueries               = xiiGALDeviceFeatureState::Enabled;
  m_AdapterDescription.m_Features.m_DepthBiasClamp                = xiiGALDeviceFeatureState::Enabled;
  m_AdapterDescription.m_Features.m_DepthClamp                    = xiiGALDeviceFeatureState::Enabled;
  m_AdapterDescription.m_Features.m_IndependentBlend              = xiiGALDeviceFeatureState::Enabled;
  m_AdapterDescription.m_Features.m_NativeFence                   = xiiGALDeviceFeatureState::Enabled;

  // Command queue properties

  // Null graphics device does not handle command queues yet.
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_Device_Implementation_DeviceNull);
