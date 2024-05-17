#pragma once

#include <GraphicsD3D11/GraphicsD3D11DLL.h>

#include <GraphicsFoundation/Declarations/GraphicsTypes.h>
#include <GraphicsFoundation/Resources/Texture.h>
#include <GraphicsFoundation/Shader/InputLayout.h>
#include <GraphicsFoundation/States/BlendState.h>
#include <GraphicsFoundation/States/DepthStencilState.h>
#include <GraphicsFoundation/States/RasterizerState.h>

#include <d3d11_4.h>
#include <dxgitype.h>

#include <Foundation/Basics/Platform/Win/IncludeWindows.h>

class XII_GRAPHICSD3D11_DLL xiiD3D11TypeConversions
{
public:
  /// \brief Helper function to hash D3D11 enumerations.
  template <typename T, typename R = typename std::underlying_type<T>::type>
  static R GetUnderlyingValue(T value)
  {
    return static_cast<typename std::underlying_type<T>::type>(value);
  }

  /// \brief Helper function to hash D3D11 flags.
  template <typename T>
  static auto GetUnderlyingFlagsValue(T value)
  {
    return static_cast<typename T::MaskType>(value);
  }

  static D3D11_BLEND    GetBlendFactor(xiiEnum<xiiGALBlendFactor> e);
  static D3D11_BLEND_OP GetBlendOp(xiiEnum<xiiGALBlendOperation> e);

  static D3D11_COMPARISON_FUNC GetComparisonFunc(xiiEnum<xiiGALComparisonFunction> e);
  static D3D11_STENCIL_OP      GetStencilOp(xiiEnum<xiiGALStencilOperation> e);

  static D3D11_FILL_MODE GetFillMode(xiiEnum<xiiGALFillMode> e);
  static D3D11_CULL_MODE GetCullMode(xiiEnum<xiiGALCullMode> e);

  static xiiUInt8 GetColorWriteMask(xiiBitflags<xiiGALColorMask> e);

  static DXGI_FORMAT                  GetFormat(xiiEnum<xiiGALTextureFormat> e);
  static xiiEnum<xiiGALTextureFormat> GetGALFormat(DXGI_FORMAT e);

  static DXGI_MODE_SCALING            GetScalingMode(xiiEnum<xiiGALScalingMode> e);
  static xiiEnum<xiiGALScalingMode>   GetGALScalingMode(DXGI_MODE_SCALING e);
  static DXGI_MODE_SCANLINE_ORDER     GetScanLineOrder(xiiEnum<xiiGALScanLineOrder> e);
  static xiiEnum<xiiGALScanLineOrder> GetGALScanLineOrder(DXGI_MODE_SCANLINE_ORDER e);

  static D3D11_FILTER               GetFilter(xiiEnum<xiiGALFilterType> minFilter, xiiEnum<xiiGALFilterType> magFilter, xiiEnum<xiiGALFilterType> mipFilter);
  static D3D11_TEXTURE_ADDRESS_MODE GetTextureAddressMode(xiiEnum<xiiGALTextureAddressMode> e);

  static D3D11_QUERY GetQueryType(xiiEnum<xiiGALQueryType> e);

  static xiiUInt32   GetBindFlags(xiiBitflags<xiiGALBindFlags> e);
  static xiiUInt32   GetCPUAccessFlags(xiiBitflags<xiiGALCPUAccessFlag> e);
  static D3D11_USAGE GetUsage(xiiEnum<xiiGALResourceUsage> e);

  static DXGI_FORMAT GetDXGIFormatFromType(xiiEnum<xiiGALValueType> e, xiiUInt32 uiComponentCount, bool bIsNormalized);

  static xiiUInt32 GetMiscTextureFlags(xiiBitflags<xiiGALMiscTextureFlags> e);

  static D3D11_INPUT_CLASSIFICATION GetElementFrequency(xiiEnum<xiiGALInputElementFrequency> e);

  static xiiEnum<xiiGALResourceUsage>        GetGALUsage(D3D11_USAGE e);
  static xiiBitflags<xiiGALBindFlags>        GetGALBindFlags(xiiUInt32 e);
  static xiiBitflags<xiiGALCPUAccessFlag>    GetGALCPUAccessFlags(xiiUInt32 e);
  static xiiBitflags<xiiGALMiscTextureFlags> GetGALMiscTextureFlags(xiiUInt32 e);

  static void GetMapTypeAndFlags(xiiEnum<xiiGALMapType> type, xiiBitflags<xiiGALMapFlags> flags, D3D11_MAP& out_mapType, xiiUInt32& out_mapFlags);

  static D3D_PRIMITIVE_TOPOLOGY GetPrimitiveTopology(xiiEnum<xiiGALPrimitiveTopology> e);
};

#include <GraphicsD3D11/Utilities/Implementation/D3D11TypeConversions_inl.h>
