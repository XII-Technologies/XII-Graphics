#include <GraphicsD3D11/GraphicsD3D11PCH.h>

XII_STATICLINK_LIBRARY(GraphicsD3D11)
{
  if (bReturn)
    return;

  XII_STATICLINK_REFERENCE(GraphicsD3D11_Startup);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_CommandEncoder_Implementation_CommandListD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_CommandEncoder_Implementation_CommandQueueD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_Device_Implementation_DeviceD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_Device_Implementation_SwapChainD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_Resources_Implementation_BottomLevelASD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_Resources_Implementation_BufferD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_Resources_Implementation_BufferViewD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_Resources_Implementation_FenceD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_Resources_Implementation_FramebufferD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_Resources_Implementation_QueryD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_Resources_Implementation_RenderPassD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_Resources_Implementation_SamplerD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_Resources_Implementation_TextureD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_Resources_Implementation_TextureViewD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_Resources_Implementation_TopLevelASD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_Shader_Implementation_InputLayoutD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_Shader_Implementation_ShaderD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_Shader_Implementation_ShaderResourceVariableD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_States_Implementation_BlendStateD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_States_Implementation_DepthStencilStateD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_States_Implementation_PipelineStateD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_States_Implementation_PipelineResourceSignatureD3D11);
  XII_STATICLINK_REFERENCE(GraphicsD3D11_States_Implementation_RasterizerStateD3D11);
}
