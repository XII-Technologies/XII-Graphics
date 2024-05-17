#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <GraphicsD3D11/Device/DeviceD3D11.h>
#include <GraphicsD3D11/Resources/FenceD3D11.h>

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALFenceD3D11, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALFenceD3D11::xiiGALFenceD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALFenceCreationDescription& creationDescription) :
  xiiGALFence(pDeviceD3D11, creationDescription), m_pFenceCompleteEvent{CreateEvent(NULL, TRUE, FALSE, NULL)}
{
}

xiiGALFenceD3D11::~xiiGALFenceD3D11() = default;

xiiResult xiiGALFenceD3D11::InitPlatform()
{
  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);

#if 0
  if (m_Description.m_Type != xiiGALFenceType::CpuWaitOnly)
  {
    xiiLog::Error("Only xiiGALFenceType::CpuWaitOnly is supported in Direct3D11.");
    return XII_FAILURE;
  }
#endif

  if (m_pFenceCompleteEvent == NULL)
  {
    xiiLog::Error("Failed to create fence complete event.");
    return XII_FAILURE;
  }

  D3D11_FENCE_FLAG fenceFlags = D3D11_FENCE_FLAG_SHARED;
  XII_HRESULT_TO_FAILURE_LOG(pDeviceD3D11->GetD3D11Device()->CreateFence(0U, fenceFlags, IID_PPV_ARGS(&m_pD3D11Fence)));

  return XII_SUCCESS;
}

xiiResult xiiGALFenceD3D11::DeInitPlatform()
{
  XII_GAL_D3D11_RELEASE(m_pD3D11Fence);

  if (m_pFenceCompleteEvent != NULL && m_pFenceCompleteEvent != INVALID_HANDLE_VALUE)
  {
    CloseHandle(m_pFenceCompleteEvent);
  }
  return XII_SUCCESS;
}

xiiUInt64 xiiGALFenceD3D11::GetCompletedValue()
{
  xiiUInt64 uiValue = m_pD3D11Fence->GetCompletedValue();
  XII_ASSERT_DEV(uiValue != xiiMath::MaxValue<xiiUInt64>(), "If the device has been removed, the return value will be UINT64_MAX.");
  return uiValue;
}

void xiiGALFenceD3D11::Signal(xiiUInt64 uiValue)
{
  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);

  XII_ASSERT_DEV(m_Description.m_Type == xiiGALFenceType::General, "The fence must be created with xiiGALFenceType::General.");
  XII_ASSERT_DEV(m_pDevice->GetDescription().m_DeviceFeatures.m_NativeFence == xiiGALDeviceFeatureState::Enabled, "CPU fence signal requires the device Native Fence feature.");

  HRESULT hResult = pDeviceD3D11->GetImmediateContext()->Signal(m_pD3D11Fence, uiValue);
  if (FAILED(hResult))
  {
    xiiLog::Error("Failed to signal Direct3D11 fence: {}", xiiHRESULTtoString(hResult));
  }
}

void xiiGALFenceD3D11::Wait(xiiUInt64 uiValue)
{
  if (GetCompletedValue() >= uiValue)
    return;

  if (m_pFenceCompleteEvent != NULL)
  {
    m_pD3D11Fence->SetEventOnCompletion(uiValue, m_pFenceCompleteEvent);

    WaitForSingleObject(m_pFenceCompleteEvent, INFINITE);
  }
  else
  {
    while (GetCompletedValue() < uiValue)
    {
      xiiThreadUtils::Sleep(xiiTime::Microseconds(1U));
    }
  }
}

void xiiGALFenceD3D11::SetDebugNamePlatform(xiiStringView sName)
{
  if (m_pD3D11Fence != nullptr)
  {
    xiiStringBuilder sb;
    if (FAILED(m_pD3D11Fence->SetPrivateData(WKPDID_D3DDebugObjectName, sName.GetElementCount(), sName.GetData(sb))))
    {
      xiiLog::Error("Failed to set the Direct3D11 fence debug name.");
    }
  }
}

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_Resources_Implementation_FenceD3D11);
