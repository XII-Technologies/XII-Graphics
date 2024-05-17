#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <Foundation/Configuration/Startup.h>
#include <GraphicsFoundation/Device/DeviceFactory.h>
#include <GraphicsFoundation/Profiling/Profiling.h>
#include <GraphicsFoundation/Utilities/GraphicsUtilities.h>

#include <GraphicsD3D11/CommandEncoder/CommandListD3D11.h>
#include <GraphicsD3D11/CommandEncoder/CommandQueueD3D11.h>
#include <GraphicsD3D11/Device/DeviceD3D11.h>
#include <GraphicsD3D11/Device/SwapChainD3D11.h>
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

#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <sdkddkver.h>

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALDeviceD3D11, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiInternal::NewInstance<xiiGALDevice> CreateD3D11Device(xiiAllocatorBase* pAllocator, const xiiGALDeviceCreationDescription& description)
{
  return XII_NEW(pAllocator, xiiGALDeviceD3D11, description);
}

// clang-format off
XII_BEGIN_SUBSYSTEM_DECLARATION(GraphicsD3D11, DeviceFactory)

ON_CORESYSTEMS_STARTUP
{
  const xiiGALDeviceImplementationDescription implementation = {.m_APIType = xiiGALGraphicsDeviceType::Direct3D12, .m_sShaderModel = "D3D_SM50", .m_sShaderCompiler = "xiiShaderCompiler" };

  xiiGALDeviceFactory::RegisterImplementation("D3D11", &CreateD3D11Device, implementation);
}

ON_CORESYSTEMS_SHUTDOWN
{
  xiiGALDeviceFactory::UnregisterImplementation("D3D11");
}

XII_END_SUBSYSTEM_DECLARATION;
// clang-format on

#define XII_VERIFY_D3D11(expression, ...)      \
  do                                           \
  {                                            \
    XII_ASSERT_DEV((expression), __VA_ARGS__); \
    if (!(expression)) { return XII_FAILURE; } \
  } while (false)

xiiGALDeviceD3D11::xiiGALDeviceD3D11(const xiiGALDeviceCreationDescription& description) :
  xiiGALDevice(description)
{
}

xiiGALDeviceD3D11::~xiiGALDeviceD3D11() = default;

xiiResult xiiGALDeviceD3D11::InitializePlatform()
{
  XII_LOG_BLOCK("xiiGALDeviceD3D11::InitializePlatform");

  // Enable the D3D11 debug layer.
  xiiUInt64 uiCreationFlags = 0U;

  if (m_Description.m_ValidationLevel != xiiGALDeviceValidationLevel::Disabled && HasSDKLayers())
  {
    uiCreationFlags |= D3D11_CREATE_DEVICE_DEBUG;
  }

  XII_VERIFY_D3D11(SUCCEEDED(CreateDXGIFactory1(__uuidof(m_pDXGIFactory), reinterpret_cast<void**>(static_cast<IDXGIFactory5**>(&m_pDXGIFactory)))), "Failed to create IDXGIFactory5 DXGI factory. Error code '{}'.", xiiArgErrorCode(GetLastError()));

  const D3D_FEATURE_LEVEL minFeatureLevel = D3D_FEATURE_LEVEL_11_0;

  IDXGIAdapter4* pHardwareAdapter = nullptr;
  if (m_Description.m_uiAdapterID == XII_GAL_DEFAULT_ADAPTER_ID)
  {
    /// \todo GraphicsD3D11: Select best adapter ID by default, based on memory size, number of command queues, and prefer Discrete over Integrated over Software adapters.
    GetHardwareAdapter(m_pDXGIFactory, &pHardwareAdapter, minFeatureLevel);
    XII_VERIFY_D3D11(pHardwareAdapter != nullptr, "No suitable hardware adapter found.");
  }
  else
  {
    xiiDynamicArray<IDXGIAdapter4*> compatibleAdapters = GetCompatibleAdapters(minFeatureLevel);

    XII_VERIFY_D3D11(m_Description.m_uiAdapterID < compatibleAdapters.GetCount(), "{0} is not a valid adapter ID. The total number of compatible adapters on this system is {1}.", m_Description.m_uiAdapterID, compatibleAdapters.GetCount());

    pHardwareAdapter = compatibleAdapters[m_Description.m_uiAdapterID];
    compatibleAdapters.RemoveAtAndSwap(m_Description.m_uiAdapterID);

    XII_GAL_D3D11_RELEASE_ARRAY(compatibleAdapters);
  }
  m_pDXGIAdapter = pHardwareAdapter;

  const D3D_FEATURE_LEVEL targetFeatureLevels[]     = {D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0};
  const char*             targetFeatureLevelNames[] = {"11.1", "11.0"};
  xiiUInt32               uiFeatureLevelIndex       = 0U;
  HRESULT                 hResult                   = E_FAIL;

  ID3D11Device*        pDevice        = nullptr;
  ID3D11DeviceContext* pDeviceContext = nullptr;
  XII_SCOPE_EXIT(XII_GAL_D3D11_RELEASE(pDeviceContext); XII_GAL_D3D11_RELEASE(pDevice));

  for (const auto& featureLevel : targetFeatureLevels)
  {
    hResult = D3D11CreateDevice(m_pDXGIAdapter, D3D_DRIVER_TYPE_UNKNOWN, 0, static_cast<xiiUInt32>(uiCreationFlags), &featureLevel, 1, D3D11_SDK_VERSION, &pDevice, nullptr, &pDeviceContext);

    if (SUCCEEDED(hResult))
      break;

    ++uiFeatureLevelIndex;
  }

  if (FAILED(hResult))
  {
    xiiLog::Error("Failed to create D3D11 hardware device. Attempting to create a WARP device.");

    // Try to create a WARP device (a high-performance software device that has the capabilities of a hardware device).
    XII_GAL_D3D11_RELEASE(m_pDXGIAdapter);

    IDXGIAdapter4* pWarpAdapter = nullptr;
    XII_VERIFY_D3D11(SUCCEEDED(m_pDXGIFactory->EnumWarpAdapter(__uuidof(pWarpAdapter), reinterpret_cast<void**>(static_cast<IDXGIAdapter4**>(&pWarpAdapter)))), "Failed to enumerate WARP adapter. Error code '{}'.", xiiArgErrorCode(GetLastError()));
    m_pDXGIAdapter = pWarpAdapter;

    uiFeatureLevelIndex = 0U;

    for (const auto& featureLevel : targetFeatureLevels)
    {
      hResult = D3D11CreateDevice(m_pDXGIAdapter, D3D_DRIVER_TYPE_UNKNOWN, 0, static_cast<xiiUInt32>(uiCreationFlags), &featureLevel, 1, D3D11_SDK_VERSION, &pDevice, nullptr, &pDeviceContext);

      if (SUCCEEDED(hResult))
      {
        xiiLog::Info("Initialized D3D11 WARP device with feature level {0}.", targetFeatureLevelNames[uiFeatureLevelIndex]);
        break;
      }

      ++uiFeatureLevelIndex;
    }

    XII_VERIFY_D3D11(SUCCEEDED(hResult), "Failed to create D3D11 WARP device.");
  }
  else
  {
    xiiLog::Info("Initialized D3D11 device with feature level {0}.", targetFeatureLevelNames[uiFeatureLevelIndex]);
  }

  if (FAILED(pDevice->QueryInterface(__uuidof(m_pDeviceD3D11), reinterpret_cast<void**>(static_cast<ID3D11Device5**>(&m_pDeviceD3D11)))))
  {
    xiiLog::Error("Failed to retrieve ID3D11Device4 from device interface.");
    return XII_FAILURE;
  }

  if (FAILED(pDeviceContext->QueryInterface(__uuidof(m_pDeviceContext), reinterpret_cast<void**>(static_cast<ID3D11DeviceContext4**>(&m_pDeviceContext)))))
  {
    xiiLog::Error("Failed to retrieve ID3D11DeviceContext4 from device context interface.");
    return XII_FAILURE;
  }

  EnumerateDisplayModes(targetFeatureLevels[uiFeatureLevelIndex], m_pDXGIAdapter, 0, xiiGALTextureFormat::RGBA8UNormalizedSRGB, m_DisplayModes);

  if (m_Description.m_ValidationLevel != xiiGALDeviceValidationLevel::Disabled)
  {
    if (SUCCEEDED(m_pDeviceD3D11->QueryInterface(__uuidof(m_pDebugD3D11), reinterpret_cast<void**>(static_cast<ID3D11Debug**>(&m_pDebugD3D11)))))
    {
      ID3D11InfoQueue* pInfoQueue = nullptr;
      XII_SCOPE_EXIT(XII_GAL_D3D11_RELEASE(pInfoQueue));

      if (SUCCEEDED(m_pDebugD3D11->QueryInterface(&pInfoQueue)))
      {
        // Suppress whole categories of messages
        // D3D11_MESSAGE_CATEGORY categories[] = {};

        // Suppress messages based on their severity level
        D3D11_MESSAGE_SEVERITY severities[] = {D3D11_MESSAGE_SEVERITY_INFO};

        // Suppress individual messages by their ID
        D3D11_MESSAGE_ID denyIDs[] =
          {
            // D3D11 WARNING: ID3D11Device::CreateInputLayout: Element in the layout mask was not found in the shader signature.
            // This mismatch is invalid if the shader actually uses the missing element.
            // [ EXECUTION WARNING #391: CREATEINPUTLAYOUT_MISSINGELEMENT]
            D3D11_MESSAGE_ID_CREATEINPUTLAYOUT_MISSINGELEMENT,
          };

        D3D11_INFO_QUEUE_FILTER queueFilter = {};
        // queueFilter.DenyList.NumCategories = XII_ARRAY_SIZE(categories);
        // queueFilter.DenyList.pCategoryList = categories;
        queueFilter.DenyList.NumSeverities = XII_ARRAY_SIZE(severities);
        queueFilter.DenyList.pSeverityList = severities;
        queueFilter.DenyList.NumIDs        = XII_ARRAY_SIZE(denyIDs);
        queueFilter.DenyList.pIDList       = denyIDs;

        XII_VERIFY(SUCCEEDED(pInfoQueue->PushStorageFilter(&queueFilter)), "Failed to push storage filter.");

#if XII_ENABLED(XII_COMPILE_FOR_DEVELOPMENT)
        if (IsDebuggerPresent())
        {
          XII_VERIFY(SUCCEEDED(pInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, TRUE)), "Failed to set break on corruption.");
          XII_VERIFY(SUCCEEDED(pInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, TRUE)), "Failed to set break on error.");
          XII_VERIFY(SUCCEEDED(pInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, TRUE)), "Failed to set break on warning.");
        }
#endif
      }
    }

#if XII_ENABLED(XII_COMPILE_FOR_DEVELOPMENT)
// We can prevent the GPU from overclocking or underclocking to get consistent timings.
// m_pDeviceD3D11->SetStablePowerState(TRUE);
#endif
  }

  xiiClipSpaceDepthRange::Default           = xiiClipSpaceDepthRange::ZeroToOne;
  xiiClipSpaceYMode::RenderToTextureDefault = xiiClipSpaceYMode::Regular;

  return XII_SUCCESS;
}

void xiiGALDeviceD3D11::ReportLiveGPUObjects()
{
#if XII_ENABLED(XII_COMPILE_FOR_DEVELOPMENT)
  IDXGIDebug1* dxgiDebug = nullptr;
  HRESULT      hResult   = DXGIGetDebugInterface1(0U, IID_PPV_ARGS(&dxgiDebug));
  if (SUCCEEDED(hResult))
  {
    OutputDebugStringW(L" +++++ Live D3D11 Objects: +++++\n");

    // Prints to OutputDebugString
    dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);

    OutputDebugStringW(L" ----- Live D3D11 Objects: -----\n");

    dxgiDebug->Release();
  }
#endif
}

void xiiGALDeviceD3D11::FlushPendingObjects()
{
  FlushDestroyedObjects();
}

void xiiGALDeviceD3D11::ResetCommandQueuesSwapChainReferences()
{
  for (auto& queue : m_CommandQueues)
  {
    if (queue != nullptr)
    {
      queue->ReleaseSwapChainCommanListReferences();
    }
  }
}

xiiResult xiiGALDeviceD3D11::ShutdownPlatform()
{
  for (xiiUInt8 i = 0; i < XII_ARRAY_SIZE(m_CommandQueues); ++i)
  {
    if (m_CommandQueues[i] == nullptr)
      continue;

    DestroyCommandQueuePlatform(m_CommandQueues[i].Release());
  }

  for (xiiUInt32 type = 0; type < TemporaryResourceType::ENUM_COUNT; ++type)
  {
    for (auto it = m_FreeTempResources[type].GetIterator(); it.IsValid(); ++it)
    {
      xiiDynamicArray<ID3D11Resource*>& resources = it.Value();
      for (auto pResource : resources)
      {
        XII_GAL_D3D11_RELEASE(pResource);
      }
    }
    m_FreeTempResources[type].Clear();

    for (auto& tempResource : m_UsedTempResources[type])
    {
      XII_GAL_D3D11_RELEASE(tempResource.m_pResource);
    }
    m_UsedTempResources[type].Clear();
  }

  XII_GAL_D3D11_RELEASE(m_pDeviceContext);
  XII_GAL_D3D11_RELEASE(m_pDebugD3D11);
  XII_GAL_D3D11_RELEASE(m_pDeviceD3D11);
  XII_GAL_D3D11_RELEASE(m_pDXGIAdapter);
  XII_GAL_D3D11_RELEASE(m_pDXGIFactory);

  m_DisplayModes.Clear();

  ReportLiveGPUObjects();

  return XII_SUCCESS;
}


xiiResult xiiGALDeviceD3D11::CreateCommandQueuesPlatform()
{
  xiiUInt32 queueCountPerContext[XII_GAL_MAX_ADAPTER_QUEUE_COUNT] = {};

  auto CreateCommandQueue = [&](xiiBitflags<xiiGALCommandQueueType> queueType, xiiStringView sName, xiiUInt32 uiAdapterId) {
    const auto& queues = m_AdapterDescription.m_CommandQueueProperties;

    for (xiiUInt32 i = 0, uiCount = queues.GetCount(); i < uiCount; ++i)
    {
      auto& currentQueue = queues[i];

      if (queueCountPerContext[i] >= currentQueue.m_uiMaxDeviceContexts)
        continue;

      if ((currentQueue.m_Type & queueType) == queueType)
      {
        queueCountPerContext[i] += 1;

        xiiUInt32 uiCommandQueueIndex = GetCommandQueueIndex(queueType);

        xiiGALCommandQueueCreationDescription queueDescription   = {.m_QueueType = queueType};
        xiiGALCommandQueueD3D11*              pCommandQueueD3D11 = static_cast<xiiGALCommandQueueD3D11*>(CreateCommandQueuePlatform(queueDescription));
        m_CommandQueues[uiCommandQueueIndex]                     = xiiUniquePtr<xiiGALCommandQueueD3D11>(pCommandQueueD3D11, &m_Allocator);

        xiiStringBuilder sb;
        sb.SetFormat("Command Queue ({}) - {}", uiCommandQueueIndex, sName);
        m_CommandQueues[uiCommandQueueIndex]->SetDebugName(sb);

        return true;
      }
    }
    return false;
  };

  if (!CreateCommandQueue(xiiGALCommandQueueType::Graphics, "Default Graphics", m_Description.m_uiAdapterID))
    return XII_FAILURE;

  CreateCommandQueue(xiiGALCommandQueueType::Transfer, "Default Transfer", m_Description.m_uiAdapterID);
  CreateCommandQueue(xiiGALCommandQueueType::Compute, "Default Compute", m_Description.m_uiAdapterID);
  CreateCommandQueue(xiiGALCommandQueueType::SparseBinding, "Default Sparse Binding", m_Description.m_uiAdapterID);

  return XII_SUCCESS;
}

void xiiGALDeviceD3D11::SetDebugNamePlatform(xiiStringView sName)
{
  if (m_pDeviceD3D11 != nullptr)
  {
    xiiStringBuilder sb;
    if (FAILED(m_pDeviceD3D11->SetPrivateData(WKPDID_D3DDebugObjectName, sName.GetElementCount(), sName.GetData(sb))))
    {
      xiiLog::Error("Failed to set the Direct3D11 device debug name.");
    }
  }
}

void xiiGALDeviceD3D11::BeginPipelinePlatform(xiiStringView sName, xiiGALSwapChain* pSwapChain)
{
  if (pSwapChain)
  {
    pSwapChain->AcquireNextRenderTarget();
  }
}

void xiiGALDeviceD3D11::EndPipelinePlatform(xiiGALSwapChain* pSwapChain)
{
  if (pSwapChain)
  {
    pSwapChain->Present();
  }
}

void xiiGALDeviceD3D11::BeginFramePlatform(const xiiUInt64 uiRenderFrame)
{
}

void xiiGALDeviceD3D11::EndFramePlatform()
{
  FreeTemporaryResources(m_uiFrameCounter);

  ++m_uiFrameCounter;
}

xiiGALSwapChain* xiiGALDeviceD3D11::CreateSwapChainPlatform(const xiiGALSwapChainCreationDescription& description)
{
  xiiGALSwapChainD3D11* pSwapChainD3D11 = XII_NEW(&m_Allocator, xiiGALSwapChainD3D11, this, description);

  if (pSwapChainD3D11->InitPlatform().Succeeded())
    return pSwapChainD3D11;

  XII_DELETE(&m_Allocator, pSwapChainD3D11);

  return pSwapChainD3D11;
}

void xiiGALDeviceD3D11::DestroySwapChainPlatform(xiiGALSwapChain* pSwapChain)
{
  xiiGALSwapChainD3D11* pSwapChainD3D11 = static_cast<xiiGALSwapChainD3D11*>(pSwapChain);

  pSwapChainD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pSwapChainD3D11);
}

xiiGALCommandQueue* xiiGALDeviceD3D11::CreateCommandQueuePlatform(const xiiGALCommandQueueCreationDescription& description)
{
  xiiGALCommandQueueD3D11* pCommandQueueD3D11 = XII_NEW(&m_Allocator, xiiGALCommandQueueD3D11, this, description);

  if (pCommandQueueD3D11->InitPlatform().Succeeded())
    return pCommandQueueD3D11;

  XII_DELETE(&m_Allocator, pCommandQueueD3D11);

  return pCommandQueueD3D11;
}

void xiiGALDeviceD3D11::DestroyCommandQueuePlatform(xiiGALCommandQueue* pCommandQueue)
{
  xiiGALCommandQueueD3D11* pCommandQueueD3D11 = static_cast<xiiGALCommandQueueD3D11*>(pCommandQueue);

  pCommandQueueD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pCommandQueueD3D11);
}

xiiGALBlendState* xiiGALDeviceD3D11::CreateBlendStatePlatform(const xiiGALBlendStateCreationDescription& description)
{
  xiiGALBlendStateD3D11* pBlendStateD3D11 = XII_NEW(&m_Allocator, xiiGALBlendStateD3D11, this, description);

  if (pBlendStateD3D11->InitPlatform().Succeeded())
    return pBlendStateD3D11;

  XII_DELETE(&m_Allocator, pBlendStateD3D11);

  return pBlendStateD3D11;
}

void xiiGALDeviceD3D11::DestroyBlendStatePlatform(xiiGALBlendState* pBlendState)
{
  xiiGALBlendStateD3D11* pBlendStateD3D11 = static_cast<xiiGALBlendStateD3D11*>(pBlendState);

  pBlendStateD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pBlendStateD3D11);
}

xiiGALDepthStencilState* xiiGALDeviceD3D11::CreateDepthStencilStatePlatform(const xiiGALDepthStencilStateCreationDescription& description)
{
  xiiGALDepthStencilStateD3D11* pDepthStencilStateD3D11 = XII_NEW(&m_Allocator, xiiGALDepthStencilStateD3D11, this, description);

  if (pDepthStencilStateD3D11->InitPlatform().Succeeded())
    return pDepthStencilStateD3D11;

  XII_DELETE(&m_Allocator, pDepthStencilStateD3D11);

  return pDepthStencilStateD3D11;
}

void xiiGALDeviceD3D11::DestroyDepthStencilStatePlatform(xiiGALDepthStencilState* pDepthStencilState)
{
  xiiGALDepthStencilStateD3D11* pDepthStencilStateD3D11 = static_cast<xiiGALDepthStencilStateD3D11*>(pDepthStencilState);

  pDepthStencilStateD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pDepthStencilStateD3D11);
}

xiiGALRasterizerState* xiiGALDeviceD3D11::CreateRasterizerStatePlatform(const xiiGALRasterizerStateCreationDescription& description)
{
  xiiGALRasterizerStateD3D11* pRasterizerStateD3D11 = XII_NEW(&m_Allocator, xiiGALRasterizerStateD3D11, this, description);

  if (pRasterizerStateD3D11->InitPlatform().Succeeded())
    return pRasterizerStateD3D11;

  XII_DELETE(&m_Allocator, pRasterizerStateD3D11);

  return pRasterizerStateD3D11;
}

void xiiGALDeviceD3D11::DestroyRasterizerStatePlatform(xiiGALRasterizerState* pRasterizerState)
{
  xiiGALRasterizerStateD3D11* pRasterizerStateD3D11 = static_cast<xiiGALRasterizerStateD3D11*>(pRasterizerState);

  pRasterizerStateD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pRasterizerStateD3D11);
}

xiiGALShader* xiiGALDeviceD3D11::CreateShaderPlatform(const xiiGALShaderCreationDescription& description)
{
  xiiGALShaderD3D11* pShaderD3D11 = XII_NEW(&m_Allocator, xiiGALShaderD3D11, this, description);

  if (pShaderD3D11->InitPlatform().Succeeded())
    return pShaderD3D11;

  XII_DELETE(&m_Allocator, pShaderD3D11);

  return pShaderD3D11;
}

void xiiGALDeviceD3D11::DestroyShaderPlatform(xiiGALShader* pShader)
{
  xiiGALShaderD3D11* pShaderD3D11 = static_cast<xiiGALShaderD3D11*>(pShader);

  pShaderD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pShaderD3D11);
}

xiiGALBuffer* xiiGALDeviceD3D11::CreateBufferPlatform(const xiiGALBufferCreationDescription& description, const xiiGALBufferData* pInitialData)
{
  xiiGALBufferD3D11* pBufferD3D11 = XII_NEW(&m_Allocator, xiiGALBufferD3D11, this, description);

  if (pBufferD3D11->InitPlatform(pInitialData).Succeeded())
    return pBufferD3D11;

  XII_DELETE(&m_Allocator, pBufferD3D11);

  return pBufferD3D11;
}

void xiiGALDeviceD3D11::DestroyBufferPlatform(xiiGALBuffer* pBuffer)
{
  xiiGALBufferD3D11* pBufferD3D11 = static_cast<xiiGALBufferD3D11*>(pBuffer);

  pBufferD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pBufferD3D11);
}

xiiGALBufferView* xiiGALDeviceD3D11::CreateBufferViewPlatform(xiiGALBuffer* pBuffer, const xiiGALBufferViewCreationDescription& description)
{
  xiiGALBufferViewD3D11* pBufferViewD3D11 = XII_NEW(&m_Allocator, xiiGALBufferViewD3D11, this, pBuffer, description);

  if (pBufferViewD3D11->InitPlatform().Succeeded())
    return pBufferViewD3D11;

  XII_DELETE(&m_Allocator, pBufferViewD3D11);

  return pBufferViewD3D11;
}

void xiiGALDeviceD3D11::DestroyBufferViewPlatform(xiiGALBufferView* pBufferView)
{
  xiiGALBufferViewD3D11* pBufferViewD3D11 = static_cast<xiiGALBufferViewD3D11*>(pBufferView);

  pBufferViewD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pBufferViewD3D11);
}

xiiGALTexture* xiiGALDeviceD3D11::CreateTexturePlatform(const xiiGALTextureCreationDescription& description, const xiiGALTextureData* pInitialData)
{
  xiiGALTextureD3D11* pTextureD3D11 = XII_NEW(&m_Allocator, xiiGALTextureD3D11, this, description);

  if (pTextureD3D11->InitPlatform(pInitialData).Succeeded())
    return pTextureD3D11;

  XII_DELETE(&m_Allocator, pTextureD3D11);

  return pTextureD3D11;
}

void xiiGALDeviceD3D11::DestroyTexturePlatform(xiiGALTexture* pTexture)
{
  xiiGALTextureD3D11* pTextureD3D11 = static_cast<xiiGALTextureD3D11*>(pTexture);

  pTextureD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pTextureD3D11);
}

xiiGALTextureView* xiiGALDeviceD3D11::CreateTextureViewPlatform(xiiGALTexture* pTexture, const xiiGALTextureViewCreationDescription& description)
{
  xiiGALTextureViewD3D11* pTextureViewD3D11 = XII_NEW(&m_Allocator, xiiGALTextureViewD3D11, this, pTexture, description);

  if (pTextureViewD3D11->InitPlatform().Succeeded())
    return pTextureViewD3D11;

  XII_DELETE(&m_Allocator, pTextureViewD3D11);

  return pTextureViewD3D11;
}

void xiiGALDeviceD3D11::DestroyTextureViewPlatform(xiiGALTextureView* pTextureView)
{
  xiiGALTextureViewD3D11* pTextureViewD3D11 = static_cast<xiiGALTextureViewD3D11*>(pTextureView);

  pTextureViewD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pTextureViewD3D11);
}

xiiGALSampler* xiiGALDeviceD3D11::CreateSamplerPlatform(const xiiGALSamplerCreationDescription& description)
{
  xiiGALSamplerD3D11* pSamplerD3D11 = XII_NEW(&m_Allocator, xiiGALSamplerD3D11, this, description);

  if (pSamplerD3D11->InitPlatform().Succeeded())
    return pSamplerD3D11;

  XII_DELETE(&m_Allocator, pSamplerD3D11);

  return pSamplerD3D11;
}

void xiiGALDeviceD3D11::DestroySamplerPlatform(xiiGALSampler* pSampler)
{
  xiiGALSamplerD3D11* pSamplerD3D11 = static_cast<xiiGALSamplerD3D11*>(pSampler);

  pSamplerD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pSamplerD3D11);
}

xiiGALInputLayout* xiiGALDeviceD3D11::CreateInputLayoutPlatform(const xiiGALInputLayoutCreationDescription& description)
{
  xiiGALInputLayoutD3D11* pInputLayoutD3D11 = XII_NEW(&m_Allocator, xiiGALInputLayoutD3D11, this, description);

  if (pInputLayoutD3D11->InitPlatform().Succeeded())
    return pInputLayoutD3D11;

  XII_DELETE(&m_Allocator, pInputLayoutD3D11);

  return pInputLayoutD3D11;
}

void xiiGALDeviceD3D11::DestroyInputLayoutPlatform(xiiGALInputLayout* pInputLayout)
{
  xiiGALInputLayoutD3D11* pInputLayoutD3D11 = static_cast<xiiGALInputLayoutD3D11*>(pInputLayout);

  pInputLayoutD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pInputLayoutD3D11);
}

xiiGALQuery* xiiGALDeviceD3D11::CreateQueryPlatform(const xiiGALQueryCreationDescription& description)
{
  xiiGALQueryD3D11* pQueryD3D11 = XII_NEW(&m_Allocator, xiiGALQueryD3D11, this, description);

  if (pQueryD3D11->InitPlatform().Succeeded())
    return pQueryD3D11;

  XII_DELETE(&m_Allocator, pQueryD3D11);

  return pQueryD3D11;
}

void xiiGALDeviceD3D11::DestroyQueryPlatform(xiiGALQuery* pQuery)
{
  xiiGALQueryD3D11* pQueryD3D11 = static_cast<xiiGALQueryD3D11*>(pQuery);

  pQueryD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pQueryD3D11);
}

xiiGALFence* xiiGALDeviceD3D11::CreateFencePlatform(const xiiGALFenceCreationDescription& description)
{
  xiiGALFenceD3D11* pFenceD3D11 = XII_NEW(&m_Allocator, xiiGALFenceD3D11, this, description);

  if (pFenceD3D11->InitPlatform().Succeeded())
    return pFenceD3D11;

  XII_DELETE(&m_Allocator, pFenceD3D11);

  return pFenceD3D11;
}

void xiiGALDeviceD3D11::DestroyFencePlatform(xiiGALFence* pFence)
{
  xiiGALFenceD3D11* pFenceD3D11 = static_cast<xiiGALFenceD3D11*>(pFence);

  pFenceD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pFenceD3D11);
}

xiiGALRenderPass* xiiGALDeviceD3D11::CreateRenderPassPlatform(const xiiGALRenderPassCreationDescription& description)
{
  xiiGALRenderPassD3D11* pRenderPassD3D11 = XII_NEW(&m_Allocator, xiiGALRenderPassD3D11, this, description);

  if (pRenderPassD3D11->InitPlatform().Succeeded())
    return pRenderPassD3D11;

  XII_DELETE(&m_Allocator, pRenderPassD3D11);

  return pRenderPassD3D11;
}

void xiiGALDeviceD3D11::DestroyRenderPassPlatform(xiiGALRenderPass* pRenderPass)
{
  xiiGALRenderPassD3D11* pRenderPassD3D11 = static_cast<xiiGALRenderPassD3D11*>(pRenderPass);

  pRenderPassD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pRenderPassD3D11);
}

xiiGALFramebuffer* xiiGALDeviceD3D11::CreateFramebufferPlatform(const xiiGALFramebufferCreationDescription& description)
{
  xiiGALFramebufferD3D11* pFramebufferD3D11 = XII_NEW(&m_Allocator, xiiGALFramebufferD3D11, this, description);

  if (pFramebufferD3D11->InitPlatform().Succeeded())
    return pFramebufferD3D11;

  XII_DELETE(&m_Allocator, pFramebufferD3D11);

  return pFramebufferD3D11;
}

void xiiGALDeviceD3D11::DestroyFramebufferPlatform(xiiGALFramebuffer* pFramebuffer)
{
  xiiGALFramebufferD3D11* pFramebufferD3D11 = static_cast<xiiGALFramebufferD3D11*>(pFramebuffer);

  pFramebufferD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pFramebufferD3D11);
}

xiiGALBottomLevelAS* xiiGALDeviceD3D11::CreateBottomLevelASPlatform(const xiiGALBottomLevelASCreationDescription& description)
{
  xiiGALBottomLevelASD3D11* pBottomLevelASD3D11 = XII_NEW(&m_Allocator, xiiGALBottomLevelASD3D11, this, description);

  if (pBottomLevelASD3D11->InitPlatform().Succeeded())
    return pBottomLevelASD3D11;

  XII_DELETE(&m_Allocator, pBottomLevelASD3D11);

  return pBottomLevelASD3D11;
}

void xiiGALDeviceD3D11::DestroyBottomLevelASPlatform(xiiGALBottomLevelAS* pBottomLevelAS)
{
  xiiGALBottomLevelASD3D11* pBottomLevelASD3D11 = static_cast<xiiGALBottomLevelASD3D11*>(pBottomLevelAS);

  pBottomLevelASD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pBottomLevelASD3D11);
}

xiiGALTopLevelAS* xiiGALDeviceD3D11::CreateTopLevelASPlatform(const xiiGALTopLevelASCreationDescription& description)
{
  xiiGALTopLevelASD3D11* pTopLevelASD3D11 = XII_NEW(&m_Allocator, xiiGALTopLevelASD3D11, this, description);

  if (pTopLevelASD3D11->InitPlatform().Succeeded())
    return pTopLevelASD3D11;

  XII_DELETE(&m_Allocator, pTopLevelASD3D11);

  return pTopLevelASD3D11;
}

void xiiGALDeviceD3D11::DestroyTopLevelASPlatform(xiiGALTopLevelAS* pTopLevelAS)
{
  xiiGALTopLevelASD3D11* pTopLevelASD3D11 = static_cast<xiiGALTopLevelASD3D11*>(pTopLevelAS);

  pTopLevelASD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pTopLevelASD3D11);
}

xiiGALPipelineResourceSignature* xiiGALDeviceD3D11::CreatePipelineResourceSignaturePlatform(const xiiGALPipelineResourceSignatureCreationDescription& description)
{
  xiiGALPipelineResourceSignatureD3D11* pPipelineResourceSignatureD3D11 = XII_NEW(&m_Allocator, xiiGALPipelineResourceSignatureD3D11, this, description);

  if (pPipelineResourceSignatureD3D11->InitPlatform().Succeeded())
    return pPipelineResourceSignatureD3D11;

  XII_DELETE(&m_Allocator, pPipelineResourceSignatureD3D11);

  return pPipelineResourceSignatureD3D11;
}

void xiiGALDeviceD3D11::DestroyPipelineResourceSignaturePlatform(xiiGALPipelineResourceSignature* pPipelineResourceSignature)
{
  xiiGALPipelineResourceSignatureD3D11* pPipelineResourceSignatureD3D11 = static_cast<xiiGALPipelineResourceSignatureD3D11*>(pPipelineResourceSignature);

  pPipelineResourceSignatureD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pPipelineResourceSignatureD3D11);
}

xiiGALPipelineState* xiiGALDeviceD3D11::CreatePipelineStatePlatform(const xiiGALPipelineStateCreationDescription& description)
{
  xiiGALPipelineStateD3D11* pPipelineStateD3D11 = XII_NEW(&m_Allocator, xiiGALPipelineStateD3D11, this, description);

  if (pPipelineStateD3D11->InitPlatform().Succeeded())
    return pPipelineStateD3D11;

  XII_DELETE(&m_Allocator, pPipelineStateD3D11);

  return pPipelineStateD3D11;
}

void xiiGALDeviceD3D11::DestroyPipelineStatePlatform(xiiGALPipelineState* pPipelineState)
{
  xiiGALPipelineStateD3D11* pPipelineStateD3D11 = static_cast<xiiGALPipelineStateD3D11*>(pPipelineState);

  pPipelineStateD3D11->DeInitPlatform().IgnoreResult();

  XII_DELETE(&m_Allocator, pPipelineStateD3D11);
}

void xiiGALDeviceD3D11::WaitIdlePlatform()
{
  ///\todo Idle all command queues.

  FlushPendingObjects();

  ///\todo Release stale resources.
}

void xiiGALDeviceD3D11::FillCapabilitiesPlatform()
{
  m_Description.m_GraphicsDeviceType = xiiGALGraphicsDeviceType::Direct3D12;

  /// \todo GraphicsD3D11: Assert that structure sizes has not been modified.

  // Set graphics adapter properties.
  {
    DXGI_ADAPTER_DESC1 dxgiAdapterDescription = {};
    m_pDXGIAdapter->GetDesc1(&dxgiAdapterDescription);
    m_AdapterDescription.m_sAdapterName = xiiStringUtf8(dxgiAdapterDescription.Description).GetData();

    if (dxgiAdapterDescription.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
      m_AdapterDescription.m_Type = xiiGALDeviceAdapterType::Software;
    else if (dxgiAdapterDescription.DedicatedVideoMemory != 0U)
      m_AdapterDescription.m_Type = xiiGALDeviceAdapterType::Discrete;
    else
      m_AdapterDescription.m_Type = xiiGALDeviceAdapterType::Integrated;

    m_AdapterDescription.m_Vendor             = xiiGALGraphicsUtilities::GetVendorFromID(dxgiAdapterDescription.VendorId);
    m_AdapterDescription.m_uiVendorID         = dxgiAdapterDescription.VendorId;
    m_AdapterDescription.m_uiDeviceID         = dxgiAdapterDescription.DeviceId;
    m_AdapterDescription.m_uiVideoOutputCount = 0U;

    // Enable features.
    m_AdapterDescription.m_Features.m_SeparablePrograms             = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_ShaderResourceQueries         = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_WireframeFill                 = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_MultithreadedResourceCreation = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_ComputeShaders                = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_GeometryShaders               = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_Tessellation                  = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_OcclusionQueries              = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_BinaryOcclusionQueries        = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_TimestampQueries              = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_PipelineStatisticsQueries     = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_DurationQueries               = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_DepthBiasClamp                = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_DepthClamp                    = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_IndependentBlend              = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_DualSourceBlend               = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_MultiViewport                 = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_TextureCompressionBC          = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_PixelUAVWritesAndAtomics      = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_TextureUAVExtendedFormats     = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_InstanceDataStepRate          = xiiGALDeviceFeatureState::Enabled;
    m_AdapterDescription.m_Features.m_TileShaders                   = xiiGALDeviceFeatureState::Disabled;
    m_AdapterDescription.m_Features.m_SubpassFramebufferFetch       = xiiGALDeviceFeatureState::Disabled;
    m_AdapterDescription.m_Features.m_TextureComponentSwizzle       = xiiGALDeviceFeatureState::Disabled;

    // Set memory properties.
    m_AdapterDescription.m_MemoryProperties.m_uiLocalMemory         = dxgiAdapterDescription.DedicatedVideoMemory;
    m_AdapterDescription.m_MemoryProperties.m_uiHostVisibleMemory   = dxgiAdapterDescription.SharedSystemMemory;
    m_AdapterDescription.m_MemoryProperties.m_uiUnifiedMemory       = 0U;
    m_AdapterDescription.m_MemoryProperties.m_uiMaxMemoryAllocation = 0U; // Unable to query.

    // Set draw command properties.
    m_AdapterDescription.m_DrawCommandProperties.m_uiMaxIndexValue        = 0U;
    m_AdapterDescription.m_DrawCommandProperties.m_uiMaxDrawIndirectCount = ~0U;
    m_AdapterDescription.m_DrawCommandProperties.m_CapabilityFlags        = xiiGALDrawCommandCapabilityFlags::DrawIndirect | xiiGALDrawCommandCapabilityFlags::DrawIndirectFirstInstance;

    // Set queue information.
    {
      auto& queueProperty                 = m_AdapterDescription.m_CommandQueueProperties.ExpandAndGetRef();
      queueProperty.m_Type                = xiiGALCommandQueueType::Graphics;
      queueProperty.m_uiMaxDeviceContexts = 1U;

      queueProperty.m_TextureCopyGranularity.PushBack(1U);
      queueProperty.m_TextureCopyGranularity.PushBack(1U);
      queueProperty.m_TextureCopyGranularity.PushBack(1U);
    }
  }

  // Enable features.
  {
    auto& deviceFeatures = m_AdapterDescription.m_Features;

    {
      bool bShaderFloat16Supported = false;

      D3D11_FEATURE_DATA_SHADER_MIN_PRECISION_SUPPORT d3d11MinPrecisionSupport = {};
      if (SUCCEEDED(m_pDeviceD3D11->CheckFeatureSupport(D3D11_FEATURE_SHADER_MIN_PRECISION_SUPPORT, &d3d11MinPrecisionSupport, sizeof(d3d11MinPrecisionSupport))))
      {
        bShaderFloat16Supported = (d3d11MinPrecisionSupport.PixelShaderMinPrecision & D3D11_SHADER_MIN_PRECISION_16_BIT) != 0 && (d3d11MinPrecisionSupport.AllOtherShaderStagesMinPrecision & D3D11_SHADER_MIN_PRECISION_16_BIT) != 0;
      }
      deviceFeatures.m_ShaderFloat16 = bShaderFloat16Supported ? xiiGALDeviceFeatureState::Enabled : xiiGALDeviceFeatureState::Disabled;
    }
  }

  // Buffer properties.
  {
    // Offsets passed to *SSetConstantBuffers1 are measured in shader constants, which are
    // 16 bytes (4*32-bit components). Each offset must be a multiple of 16 constants,
    // i.e. 256 bytes.
    m_AdapterDescription.m_BufferProperties.m_uiConstantBufferAlignment         = 256U;
    m_AdapterDescription.m_BufferProperties.m_uiStructuredBufferOffsetAlignment = D3D11_RAW_UAV_SRV_BYTE_ALIGNMENT;
  }

  // Texture properties.
  {
    m_AdapterDescription.m_TextureProperties.m_uiMaxTexture1DDimension     = D3D11_REQ_TEXTURE1D_U_DIMENSION;
    m_AdapterDescription.m_TextureProperties.m_uiMaxTexture1DArraySlices   = D3D11_REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION;
    m_AdapterDescription.m_TextureProperties.m_uiMaxTexture2DDimension     = D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;
    m_AdapterDescription.m_TextureProperties.m_uiMaxTexture2DArraySlices   = D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION;
    m_AdapterDescription.m_TextureProperties.m_uiMaxTexture3DDimension     = D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION;
    m_AdapterDescription.m_TextureProperties.m_uiMaxTextureCubeDimension   = D3D11_REQ_TEXTURECUBE_DIMENSION;
    m_AdapterDescription.m_TextureProperties.m_bTexture2DMSSupported       = true;
    m_AdapterDescription.m_TextureProperties.m_bTexture2DMSArraySupported  = true;
    m_AdapterDescription.m_TextureProperties.m_bTextureViewSupported       = true;
    m_AdapterDescription.m_TextureProperties.m_bCubeMapArraysSupported     = true;
    m_AdapterDescription.m_TextureProperties.m_bTextureView2DOn3DSupported = true;
  }

  // Sampler properties.
  {
    m_AdapterDescription.m_SamplerProperties.m_bBorderSamplingModeSupported   = true;
    m_AdapterDescription.m_SamplerProperties.m_bAnisotropicFilteringSupported = true;
    m_AdapterDescription.m_SamplerProperties.m_bLODBiasSupported              = true;
  }

  // Compute shader properties.
  {
    m_AdapterDescription.m_ComputeShaderProperties.m_uiSharedMemorySize          = 32U << 10U;
    m_AdapterDescription.m_ComputeShaderProperties.m_uiMaxThreadGroupInvocations = D3D11_CS_THREAD_GROUP_MAX_THREADS_PER_GROUP;
    m_AdapterDescription.m_ComputeShaderProperties.m_uiMaxThreadGroupSizeX       = D3D11_CS_THREAD_GROUP_MAX_X;
    m_AdapterDescription.m_ComputeShaderProperties.m_uiMaxThreadGroupSizeY       = D3D11_CS_THREAD_GROUP_MAX_Y;
    m_AdapterDescription.m_ComputeShaderProperties.m_uiMaxThreadGroupSizeZ       = D3D11_CS_THREAD_GROUP_MAX_Z;
    m_AdapterDescription.m_ComputeShaderProperties.m_uiMaxThreadGroupCountX      = D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION;
    m_AdapterDescription.m_ComputeShaderProperties.m_uiMaxThreadGroupCountY      = D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION;
    m_AdapterDescription.m_ComputeShaderProperties.m_uiMaxThreadGroupCountZ      = D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION;
  }

  // Draw command properties.
  {
    m_AdapterDescription.m_DrawCommandProperties.m_CapabilityFlags.Add(xiiGALDrawCommandCapabilityFlags::BaseVertex);

#if D3D11_REQ_DRAWINDEXED_INDEX_COUNT_2_TO_EXP >= 32
    m_AdapterDescription.m_DrawCommandProperties.m_uiMaxIndexValue = ~0u;
#else
    m_AdapterDescription.m_DrawCommandProperties.m_uiMaxIndexValue = 1u << D3D11_REQ_DRAWINDEXED_INDEX_COUNT_2_TO_EXP;
#endif
  }

  // Sparse memory properties.
  {
    D3D11_FEATURE_DATA_D3D11_OPTIONS1 d3d11TiledResources{};
    if (SUCCEEDED(m_pDeviceD3D11->CheckFeatureSupport(D3D11_FEATURE_D3D11_OPTIONS1, &d3d11TiledResources, sizeof(d3d11TiledResources))))
    {
      if (d3d11TiledResources.TiledResourcesTier >= D3D11_TILED_RESOURCES_TIER_1)
      {
        m_AdapterDescription.m_Features.m_SparseResources = xiiGALDeviceFeatureState::Enabled;

        m_AdapterDescription.m_SparseResourceProperties.m_uiAddressSpaceSize  = xiiUInt64{1} << (sizeof(void*) > 4 ? 40 : 32);
        m_AdapterDescription.m_SparseResourceProperties.m_uiResourceSpaceSize = xiiMath::MaxValue<xiiUInt32>();
        m_AdapterDescription.m_SparseResourceProperties.m_CapabilityFlags     = xiiGALSparseResourceCapabilityFlags::Buffer | xiiGALSparseResourceCapabilityFlags::BufferStandardBlock | xiiGALSparseResourceCapabilityFlags::Texture2D | xiiGALSparseResourceCapabilityFlags::Standard2DTileShape | xiiGALSparseResourceCapabilityFlags::Aliased | xiiGALSparseResourceCapabilityFlags::MixedResourceTypeSupport;

        // No 2, 8 or 16 sample multisample antialiasing (MSAA) support. Only 4x is required, except no 128 bpp formats.
        m_AdapterDescription.m_SparseResourceProperties.m_CapabilityFlags.Add(xiiGALSparseResourceCapabilityFlags::Texture4Samples | xiiGALSparseResourceCapabilityFlags::Standard2DMSTileShape);
        m_AdapterDescription.m_SparseResourceProperties.m_BindFlags = xiiGALBindFlags::ShaderResource | xiiGALBindFlags::UnorderedAccess;

#ifdef NTDDI_WIN10 // D3D11_TILED_RESOURCES_TIER_3 is not defined in Win8.1
        if (d3d11TiledResources.TiledResourcesTier >= D3D11_TILED_RESOURCES_TIER_2)
        {
          m_AdapterDescription.m_SparseResourceProperties.m_CapabilityFlags.Add(xiiGALSparseResourceCapabilityFlags::Texture3D | xiiGALSparseResourceCapabilityFlags::Standard3DTileShape);
        }
#endif

        // Some features are not correctly working in software renderer.
        if (m_AdapterDescription.m_Type == xiiGALDeviceAdapterType::Software)
        {
          // Reading from null-mapped tile doesn't return zero.
          m_AdapterDescription.m_SparseResourceProperties.m_CapabilityFlags.Remove(xiiGALSparseResourceCapabilityFlags::NonResidentStrict);
          // CheckAccessFullyMapped() in shader doesn't work.
          m_AdapterDescription.m_SparseResourceProperties.m_CapabilityFlags.Remove(xiiGALSparseResourceCapabilityFlags::ShaderResourceResidency);
          // Mip tails are not supported at all.
          m_AdapterDescription.m_SparseResourceProperties.m_CapabilityFlags.Remove(xiiGALSparseResourceCapabilityFlags::AlignedMipSize);
        }

        for (xiiUInt32 i = 0; i < m_AdapterDescription.m_CommandQueueProperties.GetCount(); ++i)
        {
          m_AdapterDescription.m_CommandQueueProperties[i].m_Type.Add(xiiGALCommandQueueType::SparseBinding);
        }
      }
    }
  }
}

ID3D11Resource* xiiGALDeviceD3D11::FindTemporaryBuffer(xiiUInt32 uiSize)
{
  const xiiUInt32 uiExpGrowthLimit = 16 * 1024 * 1024;

  uiSize = xiiMath::Max(uiSize, 256U);
  if (uiSize < uiExpGrowthLimit)
  {
    uiSize = xiiMath::PowerOfTwo_Ceil(uiSize);
  }
  else
  {
    uiSize = xiiMemoryUtils::AlignSize(uiSize, uiExpGrowthLimit);
  }

  ID3D11Resource* pResource = nullptr;
  auto            it        = m_FreeTempResources[TemporaryResourceType::Buffer].Find(uiSize);
  if (it.IsValid())
  {
    xiiDynamicArray<ID3D11Resource*>& resources = it.Value();
    if (!resources.IsEmpty())
    {
      pResource = resources[0];
      resources.RemoveAtAndSwap(0);
    }
  }

  if (pResource == nullptr)
  {
    D3D11_BUFFER_DESC desc;
    desc.ByteWidth           = uiSize;
    desc.Usage               = D3D11_USAGE_STAGING;
    desc.BindFlags           = 0;
    desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags           = 0;
    desc.StructureByteStride = 0;

    ID3D11Buffer* pBuffer = nullptr;
    if (!SUCCEEDED(m_pDeviceD3D11->CreateBuffer(&desc, nullptr, &pBuffer)))
    {
      return nullptr;
    }

    pResource = pBuffer;
  }

  auto& tempResource       = m_UsedTempResources[TemporaryResourceType::Buffer].ExpandAndGetRef();
  tempResource.m_pResource = pResource;
  tempResource.m_uiFrame   = m_uiFrameCounter;
  tempResource.m_uiHash    = uiSize;

  return pResource;
}

ID3D11Resource* xiiGALDeviceD3D11::FindTemporaryTexture(xiiUInt32 uiWidth, xiiUInt32 uiHeight, xiiUInt32 uiDepth, xiiEnum<xiiGALTextureFormat> format)
{
  xiiUInt32 data[] = {uiWidth, uiHeight, uiDepth, (xiiUInt32)format};
  xiiUInt32 uiHash = xiiHashingUtils::xxHash32(data, sizeof(data));

  ID3D11Resource* pResource = nullptr;
  auto            it        = m_FreeTempResources[TemporaryResourceType::Texture].Find(uiHash);
  if (it.IsValid())
  {
    xiiDynamicArray<ID3D11Resource*>& resources = it.Value();
    if (!resources.IsEmpty())
    {
      pResource = resources[0];
      resources.RemoveAtAndSwap(0);
    }
  }

  if (pResource == nullptr)
  {
    if (uiDepth == 1)
    {
      D3D11_TEXTURE2D_DESC desc;
      desc.Width              = uiWidth;
      desc.Height             = uiHeight;
      desc.MipLevels          = 1;
      desc.ArraySize          = 1;
      desc.Format             = xiiD3D11TypeConversions::GetFormat(format);
      desc.SampleDesc.Count   = 1;
      desc.SampleDesc.Quality = 0;
      desc.Usage              = D3D11_USAGE_STAGING;
      desc.BindFlags          = 0;
      desc.CPUAccessFlags     = D3D11_CPU_ACCESS_WRITE;
      desc.MiscFlags          = 0;

      ID3D11Texture2D* pTexture = nullptr;
      if (!SUCCEEDED(m_pDeviceD3D11->CreateTexture2D(&desc, nullptr, &pTexture)))
      {
        return nullptr;
      }

      pResource = pTexture;
    }
    else
    {
      XII_ASSERT_NOT_IMPLEMENTED;
      return nullptr;
    }
  }

  auto& tempResource       = m_UsedTempResources[TemporaryResourceType::Texture].ExpandAndGetRef();
  tempResource.m_pResource = pResource;
  tempResource.m_uiFrame   = m_uiFrameCounter;
  tempResource.m_uiHash    = uiHash;

  return pResource;
}

void xiiGALDeviceD3D11::FreeTemporaryResources(xiiUInt64 uiFrame)
{
  for (xiiUInt32 type = 0; type < TemporaryResourceType::ENUM_COUNT; ++type)
  {
    while (!m_UsedTempResources[type].IsEmpty())
    {
      auto& usedTempResource = m_UsedTempResources[type].PeekFront();
      if (usedTempResource.m_uiFrame == uiFrame)
      {
        auto it = m_FreeTempResources[type].Find(usedTempResource.m_uiHash);
        if (!it.IsValid())
        {
          it = m_FreeTempResources[type].Insert(usedTempResource.m_uiHash, xiiDynamicArray<ID3D11Resource*>(&m_Allocator));
        }

        it.Value().PushBack(usedTempResource.m_pResource);
        m_UsedTempResources[type].PopFront();
      }
      else
      {
        break;
      }
    }
  }
}

#if XII_ENABLED(XII_COMPILE_FOR_DEVELOPMENT)

bool xiiGALDeviceD3D11::HasSDKLayers()
{
  HRESULT hResult = D3D11CreateDevice(
    nullptr,
    D3D_DRIVER_TYPE_NULL, // There is no need to create a real hardware device.
    0,
    D3D11_CREATE_DEVICE_DEBUG, // Check for the SDK layers.
    nullptr,                   // Any feature level will do.
    0,
    D3D11_SDK_VERSION, // Always set this to D3D11_SDK_VERSION for Windows Store apps.
    nullptr,           // No need to keep the D3D device reference.
    nullptr,           // No need to know the feature level.
    nullptr            // No need to keep the D3D device context reference.
  );

  return SUCCEEDED(hResult);
}

#endif

void xiiGALDeviceD3D11::GetHardwareAdapter(IDXGIFactory5* pFactory, IDXGIAdapter4** ppAdapter, D3D_FEATURE_LEVEL featureLevel)
{
  IDXGIAdapter1* pDXGIAdapter = nullptr;
  *ppAdapter                  = nullptr;
  XII_SCOPE_EXIT(XII_GAL_D3D11_RELEASE(pDXGIAdapter));

  for (xiiUInt32 uiAdapterIndex = 0; pFactory->EnumAdapters1(uiAdapterIndex, &pDXGIAdapter) != DXGI_ERROR_NOT_FOUND; ++uiAdapterIndex)
  {
    DXGI_ADAPTER_DESC1 adapterDescription;
    pDXGIAdapter->GetDesc1(&adapterDescription);

    if (adapterDescription.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
    {
      // Skip software adapters.
      XII_GAL_D3D11_RELEASE(pDXGIAdapter);

      continue;
    }

    // Check to see if the adapter supports Direct3D 11, but don't create the actual device yet.
    if (SUCCEEDED(D3D11CreateDevice(pDXGIAdapter, D3D_DRIVER_TYPE_UNKNOWN, 0, 0, &featureLevel, 1, D3D11_SDK_VERSION, nullptr, nullptr, nullptr)))
    {
      break;
    }
    else
    {
      XII_GAL_D3D11_RELEASE(pDXGIAdapter);
    }
  }

  IDXGIAdapter4* pDXGIAdapter4 = nullptr;
  HRESULT        hResult       = pDXGIAdapter->QueryInterface(__uuidof(pDXGIAdapter4), reinterpret_cast<void**>(static_cast<IDXGIAdapter4**>(&pDXGIAdapter4)));
  if (FAILED(hResult))
  {
    xiiLog::Error("Failed to retrieve IDXGIAdapter4 adapter interface: {}", xiiHRESULTtoString(hResult));
  }
  else
  {
    *ppAdapter = pDXGIAdapter4;
  }
}

xiiDynamicArray<IDXGIAdapter4*> xiiGALDeviceD3D11::GetCompatibleAdapters(D3D_FEATURE_LEVEL minFeatureLevel)
{
  XII_ASSERT_DEV(m_pDXGIFactory != nullptr, "The DXGI Factory has not yet been initialized.");

  xiiDynamicArray<IDXGIAdapter4*> DXGIAdapters;

  IDXGIAdapter1* pDXGIAdapter = nullptr;
  for (xiiUInt32 uiAdapterIndex = 0; m_pDXGIFactory->EnumAdapters1(uiAdapterIndex, &pDXGIAdapter) != DXGI_ERROR_NOT_FOUND; ++uiAdapterIndex)
  {
    DXGI_ADAPTER_DESC1 adapterDescription;
    pDXGIAdapter->GetDesc1(&adapterDescription);

    if (SUCCEEDED(D3D11CreateDevice(pDXGIAdapter, D3D_DRIVER_TYPE_UNKNOWN, 0, 0, &minFeatureLevel, 1, D3D11_SDK_VERSION, nullptr, nullptr, nullptr)))
    {
      IDXGIAdapter4* pDXGIAdapter4 = nullptr;
      HRESULT        hResult       = pDXGIAdapter->QueryInterface(__uuidof(pDXGIAdapter4), reinterpret_cast<void**>(static_cast<IDXGIAdapter4**>(&pDXGIAdapter4)));
      if (FAILED(hResult))
      {
        xiiLog::Error("Failed to retrieve IDXGIAdapter4 adapter interface: {}", xiiHRESULTtoString(hResult));
      }
      else
      {
        DXGIAdapters.PushBack(pDXGIAdapter4);
      }
    }
    XII_GAL_D3D11_RELEASE(pDXGIAdapter);
  }

  return DXGIAdapters;
}

void xiiGALDeviceD3D11::EnumerateDisplayModes(D3D_FEATURE_LEVEL featureLevel, IDXGIAdapter4* pDXGIAdapter, xiiUInt32 uiOutputID, xiiEnum<xiiGALTextureFormat> format, xiiDynamicArray<xiiGALDisplayModeDescription>& displayModes)
{
  auto DXGIAdapters = GetCompatibleAdapters(featureLevel);

  DXGI_FORMAT  dxgiFormat = xiiD3D11TypeConversions::GetFormat(format);
  IDXGIOutput* pOutput    = nullptr;
  XII_SCOPE_EXIT(XII_GAL_D3D11_RELEASE(pOutput));

  if (pDXGIAdapter->EnumOutputs(uiOutputID, &pOutput) == DXGI_ERROR_NOT_FOUND)
  {
    DXGI_ADAPTER_DESC1 adapterDescription;
    pDXGIAdapter->GetDesc1(&adapterDescription);

    xiiLog::Error("Failed to enumerate output {0} of adapter {1} ({2}).", uiOutputID, adapterDescription.DeviceId, xiiStringUtf8(adapterDescription.Description).GetData());
    return;
  }

  // Retrieve the display mode count.
  xiiUInt32 uiModeCount = 0;
  if (SUCCEEDED(pOutput->GetDisplayModeList(dxgiFormat, 0U, &uiModeCount, NULL)))
  {
    // Retireve the display mode descriptions.
    xiiDynamicArray<DXGI_MODE_DESC> dxgiDisplayModes;
    dxgiDisplayModes.SetCount(uiModeCount);

    if (SUCCEEDED(pOutput->GetDisplayModeList(dxgiFormat, 0U, &uiModeCount, dxgiDisplayModes.GetData())))
    {
      displayModes.Clear();
      for (xiiUInt32 i = 0; i < uiModeCount; ++i)
      {
        const auto& dxgiDisplayMode = dxgiDisplayModes[i];
        auto&       galDisplayMode  = displayModes.ExpandAndGetRef();

        galDisplayMode.m_Resolution               = xiiSizeU32(dxgiDisplayMode.Width, dxgiDisplayMode.Height);
        galDisplayMode.m_TextureFormat            = xiiD3D11TypeConversions::GetGALFormat(dxgiDisplayMode.Format);
        galDisplayMode.m_uiRefreshRateNumerator   = dxgiDisplayMode.RefreshRate.Numerator;
        galDisplayMode.m_uiRefreshRateDenominator = dxgiDisplayMode.RefreshRate.Denominator;
        galDisplayMode.m_ScalingMode              = xiiD3D11TypeConversions::GetGALScalingMode(dxgiDisplayMode.Scaling);
        galDisplayMode.m_ScanLineOrder            = xiiD3D11TypeConversions::GetGALScanLineOrder(dxgiDisplayMode.ScanlineOrdering);
      }
    }
  }
}

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_Device_Implementation_DeviceD3D11);
