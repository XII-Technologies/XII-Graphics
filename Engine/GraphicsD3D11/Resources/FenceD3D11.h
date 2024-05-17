#pragma once

#include <GraphicsD3D11/GraphicsD3D11DLL.h>

#include <GraphicsFoundation/Resources/Fence.h>

struct ID3D11Fence;

class XII_GRAPHICSD3D11_DLL xiiGALFenceD3D11 final : public xiiGALFence
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALFenceD3D11, xiiGALFence);

public:
  virtual xiiUInt64 GetCompletedValue() override final;

  virtual void Signal(xiiUInt64 uiValue) override final;

  virtual void Wait(xiiUInt64 uiValue) override final;

  ID3D11Fence* GetD3D11Fence() const;

protected:
  friend class xiiGALDeviceD3D11;
  friend class xiiMemoryUtils;

  xiiGALFenceD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALFenceCreationDescription& creationDescription);

  virtual ~xiiGALFenceD3D11();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;

  virtual void SetDebugNamePlatform(xiiStringView sName) override final;

protected:
  ID3D11Fence* m_pD3D11Fence = nullptr;

  const HANDLE m_pFenceCompleteEvent;
};

#include <GraphicsD3D11/Resources/Implementation/FenceD3D11_inl.h>
