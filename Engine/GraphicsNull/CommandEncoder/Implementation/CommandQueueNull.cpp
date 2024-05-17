#include <GraphicsNull/GraphicsNullPCH.h>

#include <GraphicsNull/CommandEncoder/CommandListNull.h>
#include <GraphicsNull/CommandEncoder/CommandQueueNull.h>
#include <GraphicsNull/Device/DeviceNull.h>

xiiGALCommandQueueNull::xiiGALCommandQueueNull(xiiGALDeviceNull* pDeviceNull, const xiiGALCommandQueueCreationDescription& creationDescription) :
  xiiGALCommandQueue(pDeviceNull, creationDescription)
{
  xiiGALCommandListCreationDescription commandListDescription = {.m_QueueType = m_Description.m_QueueType};
  m_pDefaultCommandList                                       = XII_DEFAULT_NEW(xiiGALCommandListNull, pDeviceNull, commandListDescription);
}

xiiGALCommandQueueNull::~xiiGALCommandQueueNull()
{
  m_pDefaultCommandList.Clear();
}

xiiResult xiiGALCommandQueueNull::InitPlatform()
{
  return XII_SUCCESS;
}

xiiResult xiiGALCommandQueueNull::DeInitPlatform()
{
  return XII_SUCCESS;
}

xiiGALCommandList* xiiGALCommandQueueNull::BeginCommandList(xiiStringView sScopeName)
{
  return m_pDefaultCommandList.Borrow();
}

xiiUInt64 xiiGALCommandQueueNull::SubmitPlatform(xiiGALCommandList* pCommandList, bool bReset)
{
  return 0U;
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_CommandEncoder_Implementation_CommandQueueNull);
