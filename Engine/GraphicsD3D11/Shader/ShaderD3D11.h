#pragma once

#include <GraphicsD3D11/GraphicsD3D11DLL.h>

#include <GraphicsFoundation/Shader/InputLayout.h>
#include <GraphicsFoundation/Shader/Shader.h>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;
struct ID3D11HullShader;
struct ID3D11DomainShader;
struct ID3D11ComputeShader;

class XII_GRAPHICSD3D11_DLL xiiGALShaderD3D11 final : public xiiGALShader
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALShaderD3D11, xiiGALShader);

public:
  ID3D11VertexShader*   GetVertexShader() const;
  ID3D11PixelShader*    GetPixelShader() const;
  ID3D11GeometryShader* GetGeometryShader() const;
  ID3D11HullShader*     GetHullShader() const;
  ID3D11DomainShader*   GetDomainShader() const;
  ID3D11ComputeShader*  GetComputeShader() const;

protected:
  friend class xiiGALDeviceD3D11;
  friend class xiiMemoryUtils;

  xiiGALShaderD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALShaderCreationDescription& creationDescription);

  virtual ~xiiGALShaderD3D11();

  virtual xiiResult InitPlatform();

  virtual xiiResult DeInitPlatform();

  virtual void SetDebugNamePlatform(xiiStringView sName) override final;

protected:
  ID3D11VertexShader*   m_pVertexShader   = nullptr;
  ID3D11HullShader*     m_pHullShader     = nullptr;
  ID3D11DomainShader*   m_pDomainShader   = nullptr;
  ID3D11GeometryShader* m_pGeometryShader = nullptr;
  ID3D11PixelShader*    m_pPixelShader    = nullptr;
  ID3D11ComputeShader*  m_pComputeShader  = nullptr;
};

#include <GraphicsD3D11/Shader/Implementation/ShaderD3D11_inl.h>
