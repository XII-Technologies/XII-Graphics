#pragma once

#include <GraphicsNull/GraphicsNullDLL.h>

#include <GraphicsFoundation/Shader/ShaderResourceVariable.h>

class XII_GRAPHICSNULL_DLL xiiGALShaderResourceVariableNull final : public xiiGALShaderResourceVariable
{
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
  friend class xiiGALDeviceNull;
  friend class xiiMemoryUtils;

  xiiGALShaderResourceVariableNull();

  virtual ~xiiGALShaderResourceVariableNull();
};

#include <GraphicsNull/Shader/Implementation/ShaderResourceVariableNull_inl.h>
