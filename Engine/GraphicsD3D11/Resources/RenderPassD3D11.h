#pragma once

#include <GraphicsD3D11/GraphicsD3D11DLL.h>

#include <GraphicsFoundation/Resources/RenderPass.h>

class XII_GRAPHICSD3D11_DLL xiiGALRenderPassD3D11 final : public xiiGALRenderPass
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALRenderPassD3D11, xiiGALRenderPass);

protected:
  friend class xiiGALDeviceD3D11;
  friend class xiiMemoryUtils;

  xiiGALRenderPassD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALRenderPassCreationDescription& creationDescription);

  virtual ~xiiGALRenderPassD3D11();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;
};

#include <GraphicsD3D11/Resources/Implementation/RenderPassD3D11_inl.h>
