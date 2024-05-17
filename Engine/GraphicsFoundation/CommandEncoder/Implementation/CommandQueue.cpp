#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/CommandEncoder/CommandList.h>
#include <GraphicsFoundation/CommandEncoder/CommandQueue.h>

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALCommandQueue, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALCommandQueue::xiiGALCommandQueue(xiiGALDevice* pDevice, const xiiGALCommandQueueCreationDescription& creationDescription) :
  xiiGALDeviceObject(pDevice), m_Description(creationDescription)
{
}

xiiGALCommandQueue::~xiiGALCommandQueue() = default;

xiiUInt64 xiiGALCommandQueue::Submit(xiiGALCommandList* pCommandList, bool bReset)
{
  if (pCommandList->GetRecordingState() == xiiGALCommandList::RecordingState::Recording)
  {
    pCommandList->End();
  }

  return SubmitPlatform(pCommandList, bReset);
}

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_CommandEncoder_Implementation_CommandQueue);
