#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Declarations/DeviceObject.h>
#include <GraphicsFoundation/Declarations/GraphicsTypes.h>

/// \brief This describes the stencil operation.
///
/// [D3D11_STENCIL_OP]: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476219(v=vs.85).aspx
/// [D3D12_STENCIL_OP]: https://msdn.microsoft.com/en-us/library/windows/desktop/dn770409(v=vs.85).aspx
/// This enumeration describes the stencil operation and generally mirrors
/// [D3D11_STENCIL_OP][]/[D3D12_STENCIL_OP][] enumeration.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALStencilOperation
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    Undefined = 0U,    ///< Undefined stencil operation.
    Keep,              ///< Keep the existing stencil data.
    Zero,              ///< Set the stencil data to 0.
    Replace,           ///< Set the stencil data to the reference value.
    IncrementSaturate, ///< Increment the current stencil value, and clamp to the maximum representable unsigned value.
    DecrementSaturate, ///< Decrement the current stencil value, and clamp to 0.
    Invert,            ///< Bitwise invert the current stencil buffer value.
    IncrementWrap,     ///< Increment the current stencil value, and wrap the value to zero when incrementing the maximum representable unsigned value.
    DecrementWrap,     ///< Decrement the current stencil value, and wrap the value to the maximum representable unsigned value when decrementing a value of zero.

    ENUM_COUNT,

    Default = Undefined
  };
};

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALStencilOperation);

/// \brief This describes the stencil operation that are performed on the results of the depth test.
///
/// [D3D11_DEPTH_STENCILOP_DESC]: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476109(v=vs.85).aspx
/// [D3D12_DEPTH_STENCILOP_DESC]: https://msdn.microsoft.com/en-us/library/windows/desktop/dn770355(v=vs.85).aspx
/// The structure generally mirrors [D3D11_DEPTH_STENCILOP_DESC][]/[D3D12_DEPTH_STENCILOP_DESC][] structure.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALStencilOperationDescription : public xiiHashableStruct<xiiGALStencilOperationDescription>
{
  XII_DECLARE_POD_TYPE();

  xiiEnum<xiiGALStencilOperation>   m_StencilFailOperation      = xiiGALStencilOperation::Keep;     ///< The stencil operation to perform when stencil testing fails. The default is Keep.
  xiiEnum<xiiGALStencilOperation>   m_StencilDepthFailOperation = xiiGALStencilOperation::Keep;     ///< The stencil operation to perform when stencil testing passes and depth testing fails. The default is Keep.
  xiiEnum<xiiGALStencilOperation>   m_StencilPassOperation      = xiiGALStencilOperation::Keep;     ///< The stencil operation to perform when stencil testing and depth testing both pass. The default is Keep.
  xiiEnum<xiiGALComparisonFunction> m_ComparisonFunction        = xiiGALComparisonFunction::Always; ///< A function that compares stencil data against existing stencil data. The default is Always.
};

/// \brief This describes the depth stencil state creation description.
///
/// [D3D11_DEPTH_STENCIL_DESC]: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476110(v=vs.85).aspx
/// [D3D12_DEPTH_STENCIL_DESC]: https://msdn.microsoft.com/en-us/library/windows/desktop/dn770356(v=vs.85).aspx
/// The structure generally mirrors [D3D11_DEPTH_STENCIL_DESC][]/[D3D12_DEPTH_STENCIL_DESC][] structure.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALDepthStencilStateCreationDescription : public xiiHashableStruct<xiiGALDepthStencilStateCreationDescription>
{
  XII_DECLARE_POD_TYPE();

  bool                              m_bDepthEnable            = true;                           ///< Enable depth-stencil operations. When set to false, the depth test always passes, depth writes are disabled, and no stencil operations are performed. The default is true.
  bool                              m_bDepthWriteEnable       = true;                           ///< Enable or disable writes to a depth buffer. The default is true.
  xiiEnum<xiiGALComparisonFunction> m_ComparisonDepthFunction = xiiGALComparisonFunction::Less; ///< A function that compares depth data against existing depth data. The default is Less.
  bool                              m_bStencilEnable          = false;                          ///< Enable stencil operations. The default is false.
  xiiUInt8                          m_uiStencilReadMask       = 0xFFU;                          ///< Identify which bits of the depth-stencil buffer are accessed when reading stencil data. The default is 0xFF.
  xiiUInt8                          m_uiStencilWriteMask      = 0xFFU;                          ///< Identify which bits of the depth-stencil buffer are accessed when writing stencil data. The default is 0xFF.
  xiiGALStencilOperationDescription m_FrontFace;                                                ///< Identify stencil operations for the front-facing triangles.
  xiiGALStencilOperationDescription m_BackFace;                                                 ///< Identify stencil operations for the back-facing triangles.
};

/// \brief Interface that defines methods to manipulate a depth stencil state object.
class XII_GRAPHICSFOUNDATION_DLL xiiGALDepthStencilState : public xiiGALDeviceObject
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALDepthStencilState, xiiGALDeviceObject);

public:
  /// \brief This returns the creation description for this object.
  XII_NODISCARD const xiiGALDepthStencilStateCreationDescription& GetDescription() const;

protected:
  friend class xiiGALDevice;

  xiiGALDepthStencilState(xiiGALDevice* pDevice, const xiiGALDepthStencilStateCreationDescription& creationDescription);

  virtual ~xiiGALDepthStencilState();

  virtual xiiResult InitPlatform() = 0;

  virtual xiiResult DeInitPlatform() = 0;

protected:
  xiiGALDepthStencilStateCreationDescription m_Description;
};

#include <GraphicsFoundation/States/Implementation/DepthStencilState_inl.h>
