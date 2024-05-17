#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <GraphicsD3D11/CommandEncoder/CommandListD3D11.h>
#include <GraphicsD3D11/CommandEncoder/CommandQueueD3D11.h>
#include <GraphicsD3D11/Device/DeviceD3D11.h>

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALCommandQueueD3D11, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALCommandQueueD3D11::xiiGALCommandQueueD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALCommandQueueCreationDescription& creationDescription) :
  xiiGALCommandQueue(pDeviceD3D11, creationDescription), m_pImmediateContext(pDeviceD3D11->GetImmediateContext()), m_WaitForGPUEventHandle{CreateEvent(nullptr, false, false, nullptr)}
{
}

xiiGALCommandQueueD3D11::~xiiGALCommandQueueD3D11() = default;

xiiResult xiiGALCommandQueueD3D11::InitPlatform()
{
  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);

  D3D11_FENCE_FLAG fenceFlags = D3D11_FENCE_FLAG_SHARED;
  XII_HRESULT_TO_FAILURE_LOG(pDeviceD3D11->GetD3D11Device()->CreateFence(0U, fenceFlags, IID_PPV_ARGS(&m_pD3D11DFence)));

  m_pImmediateContext->Signal(m_pD3D11DFence, 0U);

  return XII_SUCCESS;
}

xiiResult xiiGALCommandQueueD3D11::DeInitPlatform()
{
  m_SwapChainCommandListReferences.Clear();

  for (xiiUInt32 i = 0; i < m_CommandLists.GetCount(); ++i)
  {
    auto pCommandList = m_CommandLists[i];

    XII_DEFAULT_DELETE(pCommandList)
  }
  m_CommandLists.Clear();

  CloseHandle(m_WaitForGPUEventHandle);

  XII_GAL_D3D11_RELEASE(m_pD3D11DFence);

  return XII_SUCCESS;
}

void xiiGALCommandQueueD3D11::SetDebugNamePlatform(xiiStringView sName)
{
  if (m_pImmediateContext != nullptr)
  {
    xiiStringBuilder sb;
    if (FAILED(m_pImmediateContext->SetPrivateData(WKPDID_D3DDebugObjectName, sName.GetElementCount(), sName.GetData(sb))))
    {
      xiiLog::Error("Failed to set the Direct3D11 immediate device context debug name.");
    }
  }
}

xiiUInt64 xiiGALCommandQueueD3D11::GetCompletedFenceValue()
{
  xiiUInt64 uiCompletedFenceValue = m_pD3D11DFence->GetCompletedValue();
  XII_ASSERT_DEV(uiCompletedFenceValue != UINT64_MAX, "If the device has been removed, the return value will be UINT64_MAX");

  xiiUInt64 uiCurrentFenceValue = m_LastCompletedFenceValue.load();
  while (!m_LastCompletedFenceValue.compare_exchange_weak(uiCurrentFenceValue, xiiMath::Max(uiCurrentFenceValue, uiCompletedFenceValue)))
  {
    // If exchange fails, uiCurrentFenceValue will hold the actual value of m_LastCompletedFenceValue
  }
  return m_LastCompletedFenceValue.load();
}

xiiUInt64 xiiGALCommandQueueD3D11::WaitForIdle()
{
  XII_LOCK(m_QueueMutex);

  xiiUInt64 uiLastSignaledFenceValue = m_NextFenceValue.fetch_add(1);

  m_pImmediateContext->Signal(m_pD3D11DFence, uiLastSignaledFenceValue);

  if (GetCompletedFenceValue() < uiLastSignaledFenceValue)
  {
    m_pD3D11DFence->SetEventOnCompletion(uiLastSignaledFenceValue, m_WaitForGPUEventHandle);
    WaitForSingleObject(m_WaitForGPUEventHandle, INFINITE);
    XII_ASSERT_ALWAYS(GetCompletedFenceValue() == uiLastSignaledFenceValue, "Unexpected signaled fence value");
  }
  return uiLastSignaledFenceValue;
}

xiiGALCommandList* xiiGALCommandQueueD3D11::BeginCommandList(xiiStringView sScopeName)
{
  // Try to find a command list that has been reset.
  // TODO: We want to store resetted command lists at the front of the queue.
  // Hence, the moment we encounter a command list that has not been reset, we establish that there are no other command lists
  // to search.
  for (xiiUInt32 i = 0; i < m_CommandLists.GetCount(); ++i)
  {
    auto pCommandList = m_CommandLists[i];

    if (pCommandList->GetRecordingState() == xiiGALCommandList::RecordingState::Reset)
    {
      pCommandList->Begin(sScopeName);

      return pCommandList;
    }
  }

  // Allocate a new command list.
  xiiGALCommandListCreationDescription commandListDescription = {.m_QueueType = m_Description.m_QueueType};
  xiiGALDeviceD3D11*                   pDeviceD3D11           = static_cast<xiiGALDeviceD3D11*>(m_pDevice);
  xiiGALCommandList*                   pCommandListD3D11      = XII_DEFAULT_NEW(xiiGALCommandListD3D11, pDeviceD3D11, this, commandListDescription);

  m_CommandLists.PushFront(pCommandListD3D11);

  xiiStringBuilder sb;
  sb.SetFormat("Command List {}", m_CommandLists.GetCount());
  pCommandListD3D11->SetDebugName(sb);

  pCommandListD3D11->Begin(sScopeName);

  return pCommandListD3D11;
}

void xiiGALCommandQueueD3D11::AddSwapChainCommandListReference(xiiGALCommandListD3D11* pCommandListD3D11)
{
  m_SwapChainCommandListReferences.PushBack(pCommandListD3D11);
}

void xiiGALCommandQueueD3D11::RemoveSwapChainCommandListReference(xiiGALCommandListD3D11* pCommandListD3D11)
{
  for (xiiUInt32 i = 0; i < m_SwapChainCommandListReferences.GetCount(); ++i)
  {
    auto pCommandListReferenceD3D11 = m_SwapChainCommandListReferences[i];

    if (pCommandListReferenceD3D11 == pCommandListD3D11)
    {
      XII_ASSERT_DEV(pCommandListReferenceD3D11->GetRecordingState() != xiiGALCommandList::RecordingState::Reset, "Attempting to remove a command list in recording state is an error, until the command list has been reset.");

      m_SwapChainCommandListReferences.RemoveAtAndSwap(i);
      break;
    }
  }
}

void xiiGALCommandQueueD3D11::ReleaseSwapChainCommanListReferences()
{
  for (xiiUInt32 i = 0; i < m_SwapChainCommandListReferences.GetCount(); ++i)
  {
    auto pCommandListReferenceD3D11 = m_SwapChainCommandListReferences[i];

    if (pCommandListReferenceD3D11->GetRecordingState() != xiiGALCommandList::RecordingState::Reset)
    {
      pCommandListReferenceD3D11->ReleaseInternalCommandList();
    }
  }
  m_SwapChainCommandListReferences.Clear();
}

xiiUInt64 xiiGALCommandQueueD3D11::SubmitPlatform(xiiGALCommandList* pCommandList, bool bReset)
{
  XII_LOCK(m_QueueMutex);

  if (pCommandList == nullptr)
    return GetCompletedFenceValue();

  // Increment the value before submitting the list
  xiiUInt64 uiFenceValue = m_NextFenceValue.fetch_add(1);

  xiiGALCommandListD3D11* pCommandListD3D11 = static_cast<xiiGALCommandListD3D11*>(pCommandList);

  m_pImmediateContext->ExecuteCommandList(pCommandListD3D11->GetD3D11CommandList(), FALSE);

  // Signal the fence. This must be done atomically with command list submission.
  m_pImmediateContext->Signal(m_pD3D11DFence, uiFenceValue);

  if (bReset && (pCommandListD3D11->GetRecordingState() != xiiGALCommandList::RecordingState::Reset))
  {
    pCommandListD3D11->Reset();
  }
  return uiFenceValue;
}

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_CommandEncoder_Implementation_CommandQueueD3D11);
