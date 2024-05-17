
XII_ALWAYS_INLINE ID3D11BlendState* xiiGALPipelineStateD3D11::GetD3D11BlendState() const
{
  return m_pBlendStateD3D11 ? m_pBlendStateD3D11->GetBlendState() : nullptr;
}

XII_ALWAYS_INLINE ID3D11RasterizerState* xiiGALPipelineStateD3D11::GetD3D11RasterizerState() const
{
  return m_pRasterizerStateD3D11 ? m_pRasterizerStateD3D11->GetRasterizerState() : nullptr;
}

XII_ALWAYS_INLINE ID3D11DepthStencilState* xiiGALPipelineStateD3D11::GetD3D11DepthStencilState() const
{
  return m_pDepthStencilStateD3D11 ? m_pDepthStencilStateD3D11->GetDepthStencilState() : nullptr;
}

XII_ALWAYS_INLINE ID3D11InputLayout* xiiGALPipelineStateD3D11::GetD3D11InputLayout() const
{
  return m_pInputLayoutD3D11 ? m_pInputLayoutD3D11->GetInputLayout() : nullptr;
}

XII_ALWAYS_INLINE ID3D11VertexShader* xiiGALPipelineStateD3D11::GetD3D11VertexShader() const
{
  return m_pShaderD3D11->GetVertexShader();
}

XII_ALWAYS_INLINE ID3D11PixelShader* xiiGALPipelineStateD3D11::GetD3D11PixelShader() const
{
  return m_pShaderD3D11->GetPixelShader();
}

XII_ALWAYS_INLINE ID3D11GeometryShader* xiiGALPipelineStateD3D11::GetD3D11GeometryShader() const
{
  return m_pShaderD3D11->GetGeometryShader();
}

XII_ALWAYS_INLINE ID3D11DomainShader* xiiGALPipelineStateD3D11::GetD3D11DomainShader() const
{
  return m_pShaderD3D11->GetDomainShader();
}

XII_ALWAYS_INLINE ID3D11HullShader* xiiGALPipelineStateD3D11::GetD3D11HullShader() const
{
  return m_pShaderD3D11->GetHullShader();
}

XII_ALWAYS_INLINE ID3D11ComputeShader* xiiGALPipelineStateD3D11::GetD3D11ComputeShader() const
{
  return m_pShaderD3D11->GetComputeShader();
}
