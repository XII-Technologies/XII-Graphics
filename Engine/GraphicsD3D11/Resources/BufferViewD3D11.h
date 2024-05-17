#pragma once

#include <GraphicsD3D11/GraphicsD3D11DLL.h>

#include <GraphicsFoundation/Resources/BufferView.h>

struct ID3D11View;

class XII_GRAPHICSD3D11_DLL xiiGALBufferViewD3D11 final : public xiiGALBufferView
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALBufferViewD3D11, xiiGALBufferView);

public:
  ID3D11View* GetBufferView() const;

protected:
  friend class xiiGALDeviceD3D11;
  friend class xiiMemoryUtils;

  xiiGALBufferViewD3D11(xiiGALDeviceD3D11* pDeviceD3D11, xiiGALBuffer* pBuffer, const xiiGALBufferViewCreationDescription& creationDescription);

  virtual ~xiiGALBufferViewD3D11();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;

  virtual void SetDebugNamePlatform(xiiStringView sName) override final;

  xiiResult CreateSRV(ID3D11ShaderResourceView** ppShaderResourceView);
  xiiResult CreateUAV(ID3D11UnorderedAccessView** ppUnorderedAccessView);

protected:
  ID3D11View* m_pBufferView = nullptr;
};

#include <GraphicsD3D11/Resources/Implementation/BufferViewD3D11_inl.h>
