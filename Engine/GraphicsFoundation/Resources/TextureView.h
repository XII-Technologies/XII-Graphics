#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Declarations/GraphicsTypes.h>
#include <GraphicsFoundation/Resources/Resource.h>

/// \brief This describes the unordered access view flags.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALUnorderedAccessViewFlags
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    Unspecified = 0U,           ///< Access mode is unspecified.
    Read        = XII_BIT(0),   ///< Allow read operations on the unordered access view.
    Write       = XII_BIT(1),   ///< Allow write operations on the unordered access view.
    ReadWrite   = Read | Write, ///< Allow read and write operations on the unordered access view.

    ENUM_COUNT = 4U,

    Default = Unspecified
  };

  struct Bits
  {
    StorageType Read : 1;
    StorageType Write : 1;
    StorageType ReadWrite : 1;
  };
};

XII_DECLARE_FLAGS_OPERATORS(xiiGALUnorderedAccessViewFlags);

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALUnorderedAccessViewFlags);

/// \brief This describes the texture view flags.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALTextureViewFlags
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    None               = 0U,         ///< No texture view flags.
    AllowMipGeneration = XII_BIT(0), ///< Allow automatic mipmap generation for this view. This flag is only allowed for xiiGALTextureViewType::ShaderResource view type. The texture must be created with the xiiGALMiscTextureFlags::GenerateMips flag.

    ENUM_COUNT = 2U,

    Default = None
  };

  struct Bits
  {
    StorageType AllowMipGeneration : 1;
  };
};

XII_DECLARE_FLAGS_OPERATORS(xiiGALTextureViewFlags);

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALTextureViewFlags);

/// \brief This describes the texture component swizzle.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALTextureComponentSwizzle
{
  using StorageType = xiiUInt8;

  enum Enum : StorageType
  {
    Identity = 0U, ///< Identity swizzle (e.g. R->R, G->G, B->B, A->A).
    Zero,          ///< The component is set to zero.
    One,           ///< The component is set to one.
    R,             ///< The component is set to the value of the red channel of the texture.
    G,             ///< The component is set to the value of the green channel of the texture.
    B,             ///< The component is set to the value of the blue channel of the texture.
    A,             ///< The component is set to the value of the alpha channel of the texture.

    ENUM_COUNT,

    Default = Identity
  };
};

XII_DECLARE_REFLECTABLE_TYPE(XII_GRAPHICSFOUNDATION_DLL, xiiGALTextureComponentSwizzle);

/// \brief This describes the per-channel texture component mapping.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALTextureComponentMapping : public xiiHashableStruct<xiiGALTextureComponentMapping>
{
  XII_DECLARE_POD_TYPE();

  xiiEnum<xiiGALTextureComponentSwizzle> m_R = xiiGALTextureComponentSwizzle::Identity; ///< Defines the component placed in the red component of the output vector.
  xiiEnum<xiiGALTextureComponentSwizzle> m_G = xiiGALTextureComponentSwizzle::Identity; ///< Defines the component placed in the green component of the output vector.
  xiiEnum<xiiGALTextureComponentSwizzle> m_B = xiiGALTextureComponentSwizzle::Identity; ///< Defines the component placed in the blue component of the output vector.
  xiiEnum<xiiGALTextureComponentSwizzle> m_A = xiiGALTextureComponentSwizzle::Identity; ///< Defines the component placed in the alpha component of the output vector.
};

/// \brief This describes the texture view creation description.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALTextureViewCreationDescription : public xiiHashableStruct<xiiGALTextureViewCreationDescription>
{
  XII_DECLARE_POD_TYPE();

  xiiGALTextureHandle                         m_hTexture;                                                                ///< The handle to the texture of which the view is created.
  xiiEnum<xiiGALTextureViewType>              m_ViewType                  = xiiGALTextureViewType::Undefined;            ///< Describes the texture view type. The default is Undefined.
  xiiEnum<xiiGALResourceDimension>            m_ResourceDimension         = xiiGALResourceDimension::Undefined;          ///< The view interpretation of the original texture. If default value xiiGALResourceDimension::Undefined is provided, the view type will match the type of the referenced texture.
  xiiEnum<xiiGALTextureFormat>                m_Format                    = xiiGALTextureFormat::Unknown;                ///< The view format. If default value xiiGALTextureFormat::Unknown is provided, the view format will match the referenced texture format.
  xiiUInt32                                   m_uiMostDetailedMip         = 0U;                                          ///< The most detailed mip level to use. The default is 0.
  xiiUInt32                                   m_uiMipLevelCount           = 0U;                                          ///< The total number of mip levels for the view of the texture. Render target and depth stencil views can address only one mip level. If 0 is provided, then for a shader resource view all mip levels will be referenced, and for a render target or a depth stencil view, one mip level will be referenced. The default is 0.
  xiiUInt32                                   m_uiFirstArrayOrDepthSlice  = 0U;                                          ///< For a texture array, first array slice to address in the view. For a 3D texture, first depth slice to address the view. The default is 0.
  xiiUInt32                                   m_uiArrayOrDepthSlicesCount = 0U;                                          ///< For a texture array, number of array slices to address in the view. For a 3D texture, number of depth slices to address in the view. Set to 0 to address all array or depth slices. The default is 0.
  xiiBitflags<xiiGALUnorderedAccessViewFlags> m_AccessFlags               = xiiGALUnorderedAccessViewFlags::Unspecified; ///< For an unordered access view, allowed access flags. The default is Unspecified.
  xiiBitflags<xiiGALTextureViewFlags>         m_Flags                     = xiiGALTextureViewFlags::None;                ///< Texture view flags. The default is None.
  xiiGALTextureComponentMapping               m_ComponentSwizzle;                                                        ///< Texture component swizzle.
};

/// \brief Interface that defines methods to manipulate a texture view object.
///
/// \note The texture view holds strong references to the texture. The texture will not be destroyed until all views are released.
class XII_GRAPHICSFOUNDATION_DLL xiiGALTextureView : public xiiGALResourceView
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALTextureView, xiiGALResourceView);

public:
  /// \brief This returns the creation description for this object.
  XII_NODISCARD const xiiGALTextureViewCreationDescription& GetDescription() const;

  /// \brief Returns the texture of which the texture view is created with.
  XII_NODISCARD xiiGALTexture* GetTexture() const;

protected:
  friend class xiiGALDevice;

  xiiGALTextureView(xiiGALDevice* pDevice, xiiGALTexture* pTexture, const xiiGALTextureViewCreationDescription& creationDescription);

  virtual ~xiiGALTextureView();

  virtual xiiResult InitPlatform() = 0;

  virtual xiiResult DeInitPlatform() = 0;

protected:
  xiiGALTexture* m_pTexture = nullptr;

  xiiGALTextureViewCreationDescription m_Description;
};

#include <GraphicsFoundation/Resources/Implementation/TextureView_inl.h>
