#pragma once

#include <GraphicsD3D11/GraphicsD3D11DLL.h>

#include <GraphicsFoundation/CommandEncoder/CommandQueue.h>

#include <atomic>

struct ID3D11DeviceContext1;

class XII_GRAPHICSD3D11_DLL xiiGALCommandQueueD3D11 final : public xiiGALCommandQueue
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALCommandQueueD3D11, xiiGALCommandQueue);

public:
  virtual xiiUInt64 GetNextFenceValue() const override final;

  virtual xiiUInt64 GetCompletedFenceValue() override final;

  virtual xiiUInt64 WaitForIdle() override final;

  virtual xiiGALCommandList* BeginCommandList(xiiStringView sScopeName) override final;

  void AddSwapChainCommandListReference(xiiGALCommandListD3D11* pCommandListD3D11);
  void RemoveSwapChainCommandListReference(xiiGALCommandListD3D11* pCommandListD3D11);
  void ReleaseSwapChainCommanListReferences();

protected:
  virtual xiiUInt64 SubmitPlatform(xiiGALCommandList* pCommandList, bool bReset) override final;

protected:
  friend class xiiGALDeviceD3D11;
  friend class xiiMemoryUtils;

  xiiGALCommandQueueD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALCommandQueueCreationDescription& creationDescription);

  virtual ~xiiGALCommandQueueD3D11();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;

  virtual void SetDebugNamePlatform(xiiStringView sName) override final;

protected:
  ID3D11DeviceContext4* m_pImmediateContext = nullptr;

  xiiMutex                                 m_QueueMutex;
  xiiDeque<xiiGALCommandList*>             m_CommandLists;
  xiiDynamicArray<xiiGALCommandListD3D11*> m_SwapChainCommandListReferences;

  // A value that will be signaled by the command queue next.
  std::atomic<xiiUInt64> m_NextFenceValue{1};

  // Last fence value completed by the GPU
  std::atomic<xiiUInt64> m_LastCompletedFenceValue{0};

  // The fence is signaled right after the command list has been submitted to the command queue for execution.
  // All command lists with fence value less or equal to the signaled value are guaranteed to be finished by the GPU.
  ID3D11Fence* m_pD3D11DFence          = nullptr;
  HANDLE       m_WaitForGPUEventHandle = {};
};

#include <GraphicsD3D11/CommandEncoder/Implementation/CommandQueueD3D11_inl.h>
