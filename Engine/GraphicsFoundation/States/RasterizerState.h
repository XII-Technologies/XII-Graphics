#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Declarations/DeviceObject.h>

/// \brief This describes the fill mode.
///
/// [D3D11_FILL_MODE]: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476131(v=vs.85).aspx
/// [D3D12_FILL_MODE]: https://msdn.microsoft.com/en-us/library/windows/desktop/dn770366(v=vs.85).aspx
/// This enumeration determines the fill mode to use when rendering triangles and mirrors the
/// [D3D11_FILL_MODE][]/[D3D12_FILL_MODE][] enumerations.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALFillMode
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    Undefined = 0U, ///< Undefined fill mode.
    Wireframe,      ///< Rasterize triangles using wireframe fill.
    Solid,          ///< Rasterize triangles using solid fill.

    ENUM_COUNT,

    Default = Undefined
  };
};

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALFillMode);

/// \brief This describes the cull mode.
///
/// [D3D11_CULL_MODE]: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476108(v=vs.85).aspx
/// [D3D12_CULL_MODE]: https://msdn.microsoft.com/en-us/library/windows/desktop/dn770354(v=vs.85).aspx
/// This enumeration defines which triangles are not drawn during the rasterization and mirrors
/// [D3D11_CULL_MODE][]/[D3D12_CULL_MODE][] enumerations.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALCullMode
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    Undefined = 0U, ///< Undefined cull mode.
    None,           ///< Draw all triangles.
    Front,          ///< Do not draw triangles that are front facing. Front and back facing triangles are determined by the front counter clockwise member in the rasterizer creation description.
    Back,           ///< Do not draw triangles that are back facing. Front and back facing triangles are determined by the front counter clockwise member in the rasterizer creation description.

    ENUM_COUNT,

    Default = Undefined
  };
};

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALCullMode);

/// \brief This describes the rasterizer state creation description.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALRasterizerStateCreationDescription : public xiiHashableStruct<xiiGALRasterizerStateCreationDescription>
{
  XII_DECLARE_POD_TYPE();

  xiiEnum<xiiGALFillMode> m_FillMode               = xiiGALFillMode::Solid; ///< Determines triangle fill mode. The default is Solid.
  xiiEnum<xiiGALCullMode> m_CullMode               = xiiGALCullMode::Back;  ///< Determines triangle cull mode. The default is Back.
  bool                    m_bFrontCounterClockwise = false;                 ///< Determines if a triangle is front or back facing. If this member is true, a triangle will be considered front-facing if its vertices are counter-clockwise on the render target and considered back-facing if they are clockwise. If this parameter is false, then the opposite is true. The default is false.
  bool                    m_bDepthClipEnable       = true;                  ///< Enable clipping against near and far clip planes. The default is true.
  bool                    m_bScissorEnable         = false;                 ///< Enable scissor-rectangle culling. All pixels outside an active scissor rectangle are culled. The default is false.
  bool                    m_bAntialiasedLineEnable = true;                  ///< Specifies whether to enable line antialiasing. The default is true.
  xiiInt32                m_iDepthBias             = 0;                     ///< Constant value added to the depth of a given pixel. The default is false.
  float                   m_fDepthBiasClamp        = 0.0f;                  ///< The maximum depth bias of a pixel. The default is 0.
  float                   m_fSlopeScaledDepthBias  = 0.0f;                  ///< Scalar that scales the given pixel's slope before adding to the pixel's depth. The default is 0.
};

/// \brief Interface that defines methods to manipulate a rasterizer state object.
class XII_GRAPHICSFOUNDATION_DLL xiiGALRasterizerState : public xiiGALDeviceObject
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALRasterizerState, xiiGALDeviceObject);

public:
  /// \brief This returns the creation description for this object.
  XII_NODISCARD const xiiGALRasterizerStateCreationDescription& GetDescription() const;

protected:
  friend class xiiGALDevice;

  xiiGALRasterizerState(xiiGALDevice* pDevice, const xiiGALRasterizerStateCreationDescription& creationDescription);

  virtual ~xiiGALRasterizerState();

  virtual xiiResult InitPlatform() = 0;

  virtual xiiResult DeInitPlatform() = 0;

protected:
  xiiGALRasterizerStateCreationDescription m_Description;
};

#include <GraphicsFoundation/States/Implementation/RasterizerState_inl.h>
