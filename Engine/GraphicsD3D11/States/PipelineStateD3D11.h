#pragma once

#include <GraphicsD3D11/GraphicsD3D11DLL.h>

#include <GraphicsFoundation/States/PipelineState.h>

#include <GraphicsD3D11/Resources/RenderPassD3D11.h>
#include <GraphicsD3D11/Shader/InputLayoutD3D11.h>
#include <GraphicsD3D11/Shader/ShaderD3D11.h>
#include <GraphicsD3D11/States/BlendStateD3D11.h>
#include <GraphicsD3D11/States/DepthStencilStateD3D11.h>
#include <GraphicsD3D11/States/PipelineResourceSignatureD3D11.h>
#include <GraphicsD3D11/States/RasterizerStateD3D11.h>

class XII_GRAPHICSD3D11_DLL xiiGALPipelineStateD3D11 final : public xiiGALPipelineState
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALPipelineStateD3D11, xiiGALPipelineState);

public:
  ID3D11BlendState*        GetD3D11BlendState() const;
  ID3D11RasterizerState*   GetD3D11RasterizerState() const;
  ID3D11DepthStencilState* GetD3D11DepthStencilState() const;
  ID3D11InputLayout*       GetD3D11InputLayout() const;
  ID3D11VertexShader*      GetD3D11VertexShader() const;
  ID3D11PixelShader*       GetD3D11PixelShader() const;
  ID3D11GeometryShader*    GetD3D11GeometryShader() const;
  ID3D11DomainShader*      GetD3D11DomainShader() const;
  ID3D11HullShader*        GetD3D11HullShader() const;
  ID3D11ComputeShader*     GetD3D11ComputeShader() const;

  xiiResult CommitShaderResources(xiiGALCommandListD3D11* pCommandListD3D11);

  bool UnsetResourceViews(const xiiGALResource* pResource);
  bool UnsetUnorderedAccessViews(const xiiGALResource* pResource);

protected:
  friend class xiiGALDeviceD3D11;
  friend class xiiMemoryUtils;

  xiiGALPipelineStateD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALPipelineStateCreationDescription& creationDescription);

  virtual ~xiiGALPipelineStateD3D11();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;

  virtual void SetConstantBufferPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALBuffer* pConstantBuffer) override final;
  virtual void SetShaderResourceBufferViewPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALBufferView* pBufferView) override final;
  virtual void SetShaderResourceTextureViewPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALTextureView* pTextureView) override final;
  virtual void SetUnorderedAccessBufferViewPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALBufferView* pBufferView) override final;
  virtual void SetUnorderedAccessTextureViewPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALTextureView* pTextureView) override final;
  virtual void SetSamplerPlatform(const xiiGALPipelineResourceDescription& bindingInformation, xiiGALSampler* pSampler) override final;
  virtual void ResetBoundResources() override final;

protected:
  xiiGALShaderD3D11*            m_pShaderD3D11            = nullptr;
  xiiGALBlendStateD3D11*        m_pBlendStateD3D11        = nullptr;
  xiiGALInputLayoutD3D11*       m_pInputLayoutD3D11       = nullptr;
  xiiGALRasterizerStateD3D11*   m_pRasterizerStateD3D11   = nullptr;
  xiiGALDepthStencilStateD3D11* m_pDepthStencilStateD3D11 = nullptr;

  xiiGALRenderPassD3D11*                m_pRenderPassD3D11                = nullptr;
  xiiGALPipelineResourceSignatureD3D11* m_pPipelineResourceSignatureD3D11 = nullptr;

  ID3D11Buffer*         m_pBoundConstantBuffers[xiiGALShaderStage::ENUM_COUNT][XII_GAL_MAX_CONSTANT_BUFFER_COUNT] = {};
  xiiGAL::ModifiedRange m_BoundConstantBuffersRange[xiiGALShaderStage::ENUM_COUNT];

  xiiHybridArray<ID3D11ShaderResourceView*, 16> m_pBoundShaderResourceViews[xiiGALShaderStage::ENUM_COUNT] = {};
  xiiHybridArray<xiiGALResource*, 16>           m_ResourcesForResourceViews[xiiGALShaderStage::ENUM_COUNT];
  xiiGAL::ModifiedRange                         m_BoundShaderResourceViewsRange[xiiGALShaderStage::ENUM_COUNT];

  xiiHybridArray<ID3D11UnorderedAccessView*, 16> m_BoundUnoderedAccessViews;
  xiiHybridArray<xiiGALResource*, 16>            m_ResourcesForUnorderedAccessViews;
  xiiGAL::ModifiedRange                          m_BoundUnoderedAccessViewsRange;

  ID3D11SamplerState*   m_pBoundSamplerStates[xiiGALShaderStage::ENUM_COUNT][XII_GAL_MAX_SAMPLER_COUNT] = {};
  xiiGAL::ModifiedRange m_BoundSamplerStatesRange[xiiGALShaderStage::ENUM_COUNT];

  ID3D11DeviceChild* m_pBoundShaders[xiiGALShaderStage::ENUM_COUNT] = {};
};

#include <GraphicsD3D11/States/Implementation/PipelineStateD3D11_inl.h>
