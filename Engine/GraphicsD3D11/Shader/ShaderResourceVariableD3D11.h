#pragma once

#include <GraphicsD3D11/GraphicsD3D11DLL.h>

#include <GraphicsFoundation/Shader/ShaderResourceVariable.h>

class XII_GRAPHICSD3D11_DLL xiiGALShaderResourceVariableD3D11 final : public xiiGALShaderResourceVariable
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALShaderResourceVariableD3D11, xiiGALShaderResourceVariable);

public:
  virtual void Set(xiiGALResource* pResource, xiiBitflags<xiiGALSetShaderResourceFlags> flags = xiiGALSetShaderResourceFlags::None) override final;

  virtual void SetArray(xiiArrayPtr<xiiGALResource* const> ppResources, xiiBitflags<xiiGALSetShaderResourceFlags> flags = xiiGALSetShaderResourceFlags::None) override final;

  virtual void SetBufferRange(xiiGALResource* pResource, xiiUInt64 uiOffset, xiiUInt64 uiSize, xiiUInt32 uiArrayIndex = 0U, xiiBitflags<xiiGALSetShaderResourceFlags> flags = xiiGALSetShaderResourceFlags::None) override final;

  virtual void SetBufferOffset(xiiUInt32 uiOffset, xiiUInt32 uiArrayIndex = 0U) override final;

  virtual xiiEnum<xiiGALShaderResourceVariableType> GetType() const override final;

  virtual void GetResourceDescription(xiiGALShaderResourceDescription& resourceDeccription) override final;

  virtual xiiUInt32 GetIndex() const override final;

  virtual xiiArrayPtr<xiiGALResource*> Get(xiiUInt32 uiIndex = 0U) const override final;

protected:
  friend class xiiGALDeviceD3D11;
  friend class xiiMemoryUtils;

  xiiGALShaderResourceVariableD3D11();

  virtual ~xiiGALShaderResourceVariableD3D11();
};

#include <GraphicsD3D11/Shader/Implementation/ShaderResourceVariableD3D11_inl.h>
