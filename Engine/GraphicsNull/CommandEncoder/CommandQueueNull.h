#pragma once

#include <GraphicsNull/GraphicsNullDLL.h>

#include <Foundation/Types/UniquePtr.h>

#include <GraphicsFoundation/CommandEncoder/CommandQueue.h>

class XII_GRAPHICSNULL_DLL xiiGALCommandQueueNull final : public xiiGALCommandQueue
{
public:
  /// \brief This returns the value of the internal fence that will be signaled the next time.
  virtual xiiUInt64 GetNextFenceValue() const override final;

  /// \brief This returns the last completed value of the internal fence.
  virtual xiiUInt64 GetCompletedFenceValue() override final;

  /// \brief This blocks execution until all pending GPU commands are complete.
  virtual xiiUInt64 WaitForIdle() override final;

  virtual xiiGALCommandList* BeginCommandList(xiiStringView sScopeName) override final;

protected:
  virtual xiiUInt64 SubmitPlatform(xiiGALCommandList* pCommandList, bool bReset) override final;

protected:
  friend class xiiGALDeviceNull;
  friend class xiiMemoryUtils;

  xiiGALCommandQueueNull(xiiGALDeviceNull* pDeviceNull, const xiiGALCommandQueueCreationDescription& creationDescription);

  virtual ~xiiGALCommandQueueNull();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;

protected:
  xiiUniquePtr<xiiGALCommandListNull> m_pDefaultCommandList;
};

#include <GraphicsNull/CommandEncoder/Implementation/CommandQueueNull_inl.h>
