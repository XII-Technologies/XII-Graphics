#pragma once

#include <GraphicsD3D11/GraphicsD3D11DLL.h>

#include <GraphicsFoundation/States/BlendState.h>

struct ID3D11BlendState;

class XII_GRAPHICSD3D11_DLL xiiGALBlendStateD3D11 final : public xiiGALBlendState
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALBlendStateD3D11, xiiGALBlendState);

public:
  ID3D11BlendState* GetBlendState() const;

protected:
  friend class xiiGALDeviceD3D11;
  friend class xiiMemoryUtils;

  xiiGALBlendStateD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALBlendStateCreationDescription& creationDescription);

  virtual ~xiiGALBlendStateD3D11();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;

  virtual void SetDebugNamePlatform(xiiStringView sName) override final;

protected:
  ID3D11BlendState* m_pBlendState = nullptr;
};

#include <GraphicsD3D11/States/Implementation/BlendStateD3D11_inl.h>
