#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Declarations/Constants.h>
#include <GraphicsFoundation/Declarations/DeviceObject.h>

/// \brief This describes the blend factor.
///
/// [D3D11_BLEND]: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476086(v=vs.85).aspx
/// [D3D12_BLEND]: https://msdn.microsoft.com/en-us/library/windows/desktop/dn770338(v=vs.85).aspx
/// It generally mirrors the [D3D11_BLEND][] and [D3D12_BLEND][] enumerations, and is used
/// to define source and destination blend factors for color and alpha channels.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALBlendFactor
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    Undefined = 0U,          ///< Undefined blend factor.
    Zero,                    ///< The blend factor is zero.
    One,                     ///< The blend factor is one.
    SourceColor,             ///< The blend factor is RGB data from a pixel shader.
    InverseSourceColor,      ///< The blend factor is 1-RGB, where RGB is the data from a pixel shader.
    SourceAlpha,             ///< The blend factor is alpha (A) data from a pixel shader.
    InverseSourceAlpha,      ///< The blend factor is 1-A, where A is alpha data from a pixel shader.
    DestinationAlpha,        ///< The blend factor is alpha (A) data from a render target.
    InverseDestinationAlpha, ///< The blend factor is 1-A, where A is alpha data from a render target.
    DestinationColor,        ///< The blend factor is RGB data from a render target.
    InverseDestinationColor, ///< The blend factor is 1-RGB, where RGB is the data from a render target.
    SourceAlphaSaturate,     ///< The blend factor is (f,f,f,1), where f = min(As, 1-Ad), As is alpha data from a pixel shader, and Ad is alpha data from a render target.
    BlendFactor,             ///< The blend factor is the constant blend factor set in the device.
    InverseBlendFactor,      ///< The blend factor is one minus constant blend factor set in the device.
    SourceOneColor,          ///< The blend factor is the second RGB data output from a pixel shader.
    InverseSourceOneColor,   ///< The blend factor is 1-RGB, where RGB is the second RGB data output from a pixel shader.
    SourceOneAlpha,          ///< The blend factor is the second alpha (A) data output from a pixel shader.
    InverseSourceOneAlpha,   ///< The blend factor is 1-A, where A is the second alpha data output from a pixel shader.

    ENUM_COUNT,

    Default = Undefined
  };
};

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALBlendFactor);

/// \brief This describes the blend operation.
///
/// [D3D11_BLEND_OP]: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476088(v=vs.85).aspx
/// [D3D12_BLEND_OP]: https://msdn.microsoft.com/en-us/library/windows/desktop/dn770340(v=vs.85).aspx
/// It generally mirrors the [D3D11_BLEND_OP][] and [D3D12_BLEND_OP][] enumerations, and is used to define RGB and Alpha
/// blending operations.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALBlendOperation
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    Undefined = 0U,  ///< Undefined blend operation.
    Add,             ///< Add source and destination color components.
    Subtract,        ///< Subtract destination color components from source color components.
    ReverseSubtract, ///< Subtract source color components from destination color components.
    Min,             ///< Compute the minimum of source and destination color components.
    Max,             ///< Compute the maximum of source and destination color components.

    ENUM_COUNT,

    Default = Undefined
  };
};

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALBlendOperation);

/// \brief This describes the color component write flags.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALColorMask
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    None  = 0U,         ///< Do not write to any components.
    Red   = XII_BIT(0), ///< Write to the red component.
    Green = XII_BIT(1), ///< Write to the green component.
    Blue  = XII_BIT(2), ///< Write to the blue component.
    Alpha = XII_BIT(3), ///< Write to the alpha component.

    ENUM_COUNT = 5U,

    RG   = Red | Green,                ///< Write to the red and green components.
    RGB  = Red | Green | Blue,         ///< Write to the red, green and blue components.
    RGBA = Red | Green | Blue | Alpha, ///< Write to the red, green, blue, and alpha components.

    Default = (((Red | Green) | Blue) | Alpha) ///< Write to all components.
  };

  struct Bits
  {
    StorageType Red : 1;
    StorageType Green : 1;
    StorageType Blue : 1;
    StorageType Alpha : 1;

    StorageType RG : 1;
    StorageType RGB : 1;
    StorageType RGBA : 1;
  };
};

XII_DECLARE_FLAGS_OPERATORS(xiiGALColorMask);

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALColorMask);

/// \brief This describes the logic operation.
///
/// [D3D12_LOGIC_OP]: https://msdn.microsoft.com/en-us/library/windows/desktop/dn770379(v=vs.85).aspx
/// It generally mirrors [D3D12_LOGIC_OP][] enum, and is only available on the DirectX backend.
/// \sa [D3D12_LOGIC_OP on MSDN][D3D12_LOGIC_OP]
struct XII_GRAPHICSFOUNDATION_DLL xiiGALLogicOperation
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    Clear = 0U,   ///< Clear the render target.
    Set,          ///< Set the render target.
    Copy,         ///< Copy the render target.
    CopyInverted, ///< Perform an inverted-copy of the render target.
    NoOperation,  ///< No operation is performed on the render target.
    Invert,       ///< Invert the render target.
    AND,          ///< Perform a logical AND operation on the render target.
    NAND,         ///< Perform a logical NAND operation on the render target.
    OR,           ///< Perform a logical OR operation on the render target.
    NOR,          ///< Perform a logical NOR operation on the render target.
    XOR,          ///< Perform a logical XOR operation on the render target.
    Equivalent,   ///< Perform a logical equal operation on the render target.
    AndReversed,  ///< Perform a logical AND and reverse operation on the render target.
    AndInverted,  ///< Perform a logical AND and invert operation on the render target.
    OrReversed,   ///< Perform a logical OR and reverse operation on the render target.
    OrInverted,   ///< Perform a logical OR and invert operation on the render target.

    ENUM_COUNT,

    Default = Clear
  };
};

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALLogicOperation);

/// \brief This describes the blend state for a single render target.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALRenderTargetBlendDescription : public xiiHashableStruct<xiiGALRenderTargetBlendDescription>
{
  XII_DECLARE_POD_TYPE();

  bool                          m_bBlendEnable          = false;                     ///< Enable or disable blending for this render target. The default is false.
  xiiEnum<xiiGALBlendFactor>    m_SourceBlend           = xiiGALBlendFactor::One;    ///< Specifies the blend factor to apply to the RGB value output from the pixel shader. The default is One.
  xiiEnum<xiiGALBlendFactor>    m_DestinationBlend      = xiiGALBlendFactor::Zero;   ///< Specifies the blend factor to apply to the RGB value in the render target. The default is Zero.
  xiiEnum<xiiGALBlendOperation> m_BlendOperation        = xiiGALBlendOperation::Add; ///< Defines how to combine the source and destination RGB values after applying the source and destination blend factors. The default is Add.
  xiiEnum<xiiGALBlendFactor>    m_SourceBlendAlpha      = xiiGALBlendFactor::One;    ///< Specifies the blend factor to apply to the alpha value output from the pixel shader. The default is One.
  xiiEnum<xiiGALBlendFactor>    m_DestinationBlendAlpha = xiiGALBlendFactor::Zero;   ///< Specifies the blend factor to apply to the alpha value in the render target. The default is Zero.
  xiiEnum<xiiGALBlendOperation> m_BlendOperationAlpha   = xiiGALBlendOperation::Add; ///< Defines how to combine the source and destination alpha values after applying the source and destination blend alpha factors. The default is Add.
  xiiBitflags<xiiGALColorMask>  m_ColorMask             = xiiGALColorMask::RGBA;     ///< Render target color write mask. The default is default is RGBA.
};

/// \brief This describes the blend state for all render targets in the graphics pipeline.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALBlendStateCreationDescription : public xiiHashableStruct<xiiGALBlendStateCreationDescription>
{
  XII_DECLARE_POD_TYPE();

  bool                                                                               m_bAlphaToCoverage     = false;                             ///< Specifies whether to use alpha-to-coverage as a multisampling technique when setting a pixel to a render target. The default is false.
  bool                                                                               m_bIndependentBlend    = false;                             ///< Specifies whether to enable independent blending in simultaneous render targets. If set to false, only m_RenderTargets[0] is used. The default is false.
  bool                                                                               m_LogicOperationEnable = false;                             ///< Enable or disable a logical operation for this render target. The default is false.
  xiiEnum<xiiGALLogicOperation>                                                      m_LogicOperation       = xiiGALLogicOperation::NoOperation; ///< Defines logical operation for the render target. The default is NoOperation.
  xiiStaticArray<xiiGALRenderTargetBlendDescription, XII_GAL_MAX_RENDERTARGET_COUNT> m_RenderTargets;                                            ///< An array of render target blend descriptions that describe the blend states for each render targets at an index.
};

/// \brief Interface that defines methods to manipulate a blend state object.
class XII_GRAPHICSFOUNDATION_DLL xiiGALBlendState : public xiiGALDeviceObject
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALBlendState, xiiGALDeviceObject);

public:
  /// \brief This returns the creation description for this object.
  XII_NODISCARD const xiiGALBlendStateCreationDescription& GetDescription() const;

protected:
  friend class xiiGALDevice;

  xiiGALBlendState(xiiGALDevice* pDevice, const xiiGALBlendStateCreationDescription& creationDescription);

  virtual ~xiiGALBlendState();

  virtual xiiResult InitPlatform() = 0;

  virtual xiiResult DeInitPlatform() = 0;

protected:
  xiiGALBlendStateCreationDescription m_Description;
};

#include <GraphicsFoundation/States/Implementation/BlendState_inl.h>
