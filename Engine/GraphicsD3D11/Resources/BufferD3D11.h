#pragma once

#include <GraphicsD3D11/GraphicsD3D11DLL.h>

#include <GraphicsFoundation/Resources/Buffer.h>

struct ID3D11Buffer;

class XII_GRAPHICSD3D11_DLL xiiGALBufferD3D11 final : public xiiGALBuffer
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALBufferD3D11, xiiGALBuffer);

public:
  virtual void SetState(xiiBitflags<xiiGALResourceStateFlags> stateFlags) override final;

  virtual xiiBitflags<xiiGALResourceStateFlags> GetState() const override final;

  virtual xiiGALMemoryProperties GetMemoryProperties() const override final;

  virtual void FlushMappedRange(xiiUInt64 uiStartOffset, xiiUInt64 uiSize) override final;

  virtual void InvalidateMappedRange(xiiUInt64 uiStartOffset, xiiUInt64 uiSize) override final;

  virtual xiiGALSparseBufferProperties GetSparseProperties() const override final;

  ID3D11Buffer* GetBuffer() const;

  xiiEnum<xiiGALValueType> GetIndexFormat() const;

protected:
  friend class xiiGALDeviceD3D11;
  friend class xiiMemoryUtils;

  xiiGALBufferD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALBufferCreationDescription& creationDescription);

  virtual ~xiiGALBufferD3D11();

  virtual xiiResult InitPlatform(const xiiGALBufferData* pInitialData) override final;

  virtual xiiResult DeInitPlatform() override final;

  virtual void SetDebugNamePlatform(xiiStringView sName) override final;

protected:
  ID3D11Buffer* m_pBuffer = nullptr;

  xiiEnum<xiiGALValueType> m_IndexFormat = xiiGALValueType::Undefined; // Strictly index buffers.
};

#include <GraphicsD3D11/Resources/Implementation/BufferD3D11_inl.h>
