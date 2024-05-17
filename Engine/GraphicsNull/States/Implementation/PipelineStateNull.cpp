#include <GraphicsNull/GraphicsNullPCH.h>

#include <GraphicsNull/Device/DeviceNull.h>
#include <GraphicsNull/States/PipelineStateNull.h>

xiiGALPipelineStateNull::xiiGALPipelineStateNull(xiiGALDeviceNull* pDeviceNull, const xiiGALPipelineStateCreationDescription& creationDescription) :
  xiiGALPipelineState(pDeviceNull, creationDescription)
{
}

xiiGALPipelineStateNull::~xiiGALPipelineStateNull() = default;

xiiResult xiiGALPipelineStateNull::InitPlatform()
{
  return XII_SUCCESS;
}

xiiResult xiiGALPipelineStateNull::DeInitPlatform()
{
  return XII_SUCCESS;
}

void xiiGALPipelineStateNull::SetConstantBufferPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALBuffer* pConstantBuffer)
{
}

void xiiGALPipelineStateNull::SetShaderResourceBufferViewPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALBufferView* pBufferView)
{
}

void xiiGALPipelineStateNull::SetShaderResourceTextureViewPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALTextureView* pTextureView)
{
}

void xiiGALPipelineStateNull::SetUnorderedAccessBufferViewPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALBufferView* pBufferView)
{
}

void xiiGALPipelineStateNull::SetUnorderedAccessTextureViewPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALTextureView* pTextureView)
{
}

void xiiGALPipelineStateNull::SetSamplerPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALSampler* pSampler)
{
}

void xiiGALPipelineStateNull::ResetBoundResources()
{
}

XII_STATICLINK_FILE(GraphicsNull, GraphicsNull_States_Implementation_PipelineStateNull);
