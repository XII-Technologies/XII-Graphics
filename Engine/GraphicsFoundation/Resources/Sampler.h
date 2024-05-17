#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Declarations/DeviceObject.h>
#include <GraphicsFoundation/Declarations/GraphicsTypes.h>

/// \brief This describes the sampler flags.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALSamplerFlags
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    None                           = 0U,         ///< No sampler flags.
    Subsampled                     = XII_BIT(0), ///< Specifies that the sampler will read from a subsampled texture created with the miscellaneous texture subsampled flag.
    SubsampledCoarseReconstruction = XII_BIT(1), ///< Specifies that the GPU is allowed to use fast approximation when reconstructing full-resolution value from the subsampled texture accessed by the sampler.

    ENUM_COUNT = 3U,

    Default = None
  };

  struct Bits
  {
    StorageType Subsampled : 1;
    StorageType SubsampledCoarseReconstruction : 1;
  };
};

XII_DECLARE_FLAGS_OPERATORS(xiiGALSamplerFlags);

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALSamplerFlags);

/// \brief This describes the sampler creation description.
///
/// To create an anisotropic filter, all three filters must either be xiiGALFilterType::Anisotropic or xiiGALFilterType::ComparisonAnisotropic.
///
/// The MipFilter member cannot be comparison filter except for xiiGALFilterType::Anisotropic if all three filters have that value.
///
/// The MinFilter and MagFilter members must either be regular filters or comparison filters, as mixing comparison filters and regular filters is an error.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALSamplerCreationDescription : public xiiHashableStruct<xiiGALSamplerCreationDescription>
{
  XII_DECLARE_POD_TYPE();

  xiiStringView                     m_sName;                                                ///< Resource name. The default is an empty string view.
  xiiEnum<xiiGALFilterType>         m_MinFilter          = xiiGALFilterType::Linear;        ///< Texture minification filter. The default is Linear.
  xiiEnum<xiiGALFilterType>         m_MagFilter          = xiiGALFilterType::Linear;        ///< Texture magnification filter. The default is Linear.
  xiiEnum<xiiGALFilterType>         m_MipFilter          = xiiGALFilterType::Linear;        ///< Texture mip filter. The default is Linear.
  xiiEnum<xiiGALTextureAddressMode> m_AddressU           = xiiGALTextureAddressMode::Clamp; ///< Texture address mode for U coordinate. The default is Clamp.
  xiiEnum<xiiGALTextureAddressMode> m_AddressV           = xiiGALTextureAddressMode::Clamp; ///< Texture address mode for V coordinate. The default is Clamp.
  xiiEnum<xiiGALTextureAddressMode> m_AddressW           = xiiGALTextureAddressMode::Clamp; ///< Texture address mode for W coordinate. The default is Clamp.
  xiiBitflags<xiiGALSamplerFlags>   m_Flags              = xiiGALSamplerFlags::None;        ///< Sampler flags.
  bool                              m_bUnormalizedCoords = false;                           ///< Indicates whether to use unnormalized texture coordinates.
                                                                                            ///  \remarks When set to true, the range of the image coordinates used to lookup the texel is in the range of 0 to the image size in each dimension.
                                                                                            ///           When set to False, the range of image coordinates is 0.0 to 1.0.
                                                                                            ///
                                                                                            ///           Unnormalized coordinates are only supported in Vulkan and Metal.
  float                             m_fMipLODBias        = 0.0f;                            ///< Offset from the calculated mipmap level. The default is 0;
  xiiUInt32                         m_uiMaxAnisotropy    = 0U;                              ///< Maximum anisotropy level for the anisotropic filter. The default is 0.
  xiiEnum<xiiGALComparisonFunction> m_ComparisonFunction = xiiGALComparisonFunction::Never; ///< A function that compares sampled data against existing sampled data when comparisons filter used. The default is Never.
  xiiColor                          m_BorderColor        = xiiColor::Black;                 ///< Border color to use if the texture address border is specified for AddressU, AddressV, or AddressW. The default is xiiColor::Black.
  float                             m_fMinLOD            = 0.0f;                            ///< Specifies the minimum value that LOD is clamped to before accessing the texture MIP levels. The default is 0.
  float                             m_fMaxLOD            = xiiMath::MaxValue<float>();      ///< Specifies the maximum value that LOD is clamped to before accessing the texture MIP levels. The default is xiiMath::MaxValue<float>().
};

/// \brief Interface that defines methods to manipulate a texture sampler object used to perform texture filtering.
class XII_GRAPHICSFOUNDATION_DLL xiiGALSampler : public xiiGALDeviceObject
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALSampler, xiiGALDeviceObject);

public:
  /// \brief This returns the creation description for this object.
  XII_NODISCARD const xiiGALSamplerCreationDescription& GetDescription() const;

protected:
  friend class xiiGALDevice;

  xiiGALSampler(xiiGALDevice* pDevice, const xiiGALSamplerCreationDescription& creationDescription);

  virtual ~xiiGALSampler();

  virtual xiiResult InitPlatform() = 0;

  virtual xiiResult DeInitPlatform() = 0;

protected:
  xiiGALSamplerCreationDescription m_Description;
};

#include <GraphicsFoundation/Resources/Implementation/Sampler_inl.h>
