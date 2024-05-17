#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Declarations/DeviceObject.h>

/// \brief This describes the fence type.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALFenceType
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    CpuWaitOnly = 0U, ///< A basic fence that may be used for signaling the fence from GPU, and waiting for the fence on CPU.
    General,          ///< A general fence that may be used for signaling the fence from GPU, waiting for the fence on CPU, and waiting for the fence on GPU. If the native fence feature is enabled, the fence may also be used for signaling the fence on CPU, and waiting on GPU for a value that will be enqueued for signal later.

    ENUM_COUNT,

    Default = CpuWaitOnly
  };
};

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALFenceType);

/// \brief This describes the fence creation description.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALFenceCreationDescription : public xiiHashableStruct<xiiGALFenceCreationDescription>
{
  XII_DECLARE_POD_TYPE();

  xiiEnum<xiiGALFenceType> m_Type = xiiGALFenceType::CpuWaitOnly; ///< The fence type.
};

/// \brief Interface that defines methods to manipulate a fence object.
///
/// \remarks When a fence that was previously signaled by IDeviceContext::EnqueueSignal() is destroyed, it may block the GPU until all prior commands have completed execution.
///
/// \remarks In Direct3D12 and Vulkan backends, fence is thread-safe.
class XII_GRAPHICSFOUNDATION_DLL xiiGALFence : public xiiGALDeviceObject
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALFence, xiiGALDeviceObject);

public:
  /// \brief This returns the creation description for this object.
  XII_NODISCARD const xiiGALFenceCreationDescription& GetDescription() const;

  /// \brief This returns the last completed value signaled by the GPU.
  ///
  /// \remarks In Direct3D11 backend, this method is not thread-safe (even if the fence object is protected by a mutex) and must only be called by the same thread that signals the fence via an enqueue.
  XII_NODISCARD virtual xiiUInt64 GetCompletedValue() = 0;

  /// \brief This sets the fence to the specified value.
  ///
  /// \param uiValue - The new value the fence should reach. The value must be greater than the current value of the fence.
  ///
  /// \note Fence value will be changed immediately on the CPU. Use device to enqueue a signal command that will change the value on the GPU after all previously submitted commands are complete.
  ///
  /// \note The fence must have been created with type General.
  virtual void Signal(xiiUInt64 uiValue) = 0;

  /// \brief This waits until the fence reaches or exceeds the specified value on the host.
  ///
  /// \param uiValue - The value that the fence is waiting to reach.
  ///
  /// \note The method blocks the execution of the calling thread until the wait is complete.
  virtual void Wait(xiiUInt64 uiValue) = 0;

protected:
  friend class xiiGALDevice;

  xiiGALFence(xiiGALDevice* pDevice, const xiiGALFenceCreationDescription& creationDescription);

  virtual ~xiiGALFence();

  virtual xiiResult InitPlatform() = 0;

  virtual xiiResult DeInitPlatform() = 0;

  void UpdateLastCompletedFenceValue(xiiUInt64 uiValue);

protected:
  xiiGALFenceCreationDescription m_Description;

  xiiAtomicInteger64 m_uiLastCompletedFenceValue;
};

#include <GraphicsFoundation/Resources/Implementation/Fence_inl.h>
