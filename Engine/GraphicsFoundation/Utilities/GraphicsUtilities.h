#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Declarations/Descriptors.h>
#include <GraphicsFoundation/Resources/Texture.h>
#include <GraphicsFoundation/States/PipelineResourceSignature.h>

class XII_GRAPHICSFOUNDATION_DLL xiiGALGraphicsUtilities
{
public:
  /// \brief This returns the graphics adapter vendor type from the given ID.
  static XII_NODISCARD xiiEnum<xiiGALGraphicsAdapterVendor> GetVendorFromID(xiiUInt32 uiID);

  /// \brief Returns true if all components of the xiiGALTextureComponentMapping are xiiGALTextureComponentSwizzle::Identity.
  static XII_NODISCARD bool IsIdentityComponentMapping(const xiiGALTextureComponentMapping& mapping);

  /// \brief This returns the basic texture information for a particular format.
  ///
  /// \param format - The texture format for which to provide the information.
  ///
  /// \return A const reference to the xiiGALTextureFormatDescription structure containing the texture format description.
  ///
  /// \remarks This method must be externally synchronized.
  static XII_NODISCARD const xiiGALTextureFormatDescription& GetTextureFormatProperties(xiiEnum<xiiGALTextureFormat> format);

  /// \brief This returns the sparse texture format information for the given texture format, resource dimension and sample count.
  static XII_NODISCARD const xiiGALSparseTextureProperties GetSparseTextureProperties(xiiEnum<xiiGALTextureFormat> format, xiiEnum<xiiGALResourceDimension> dimension, xiiUInt32 uiSampleCount);

  /// \brief This returns the valid pipeline resource flags for a given shader resource type.
  static XII_NODISCARD xiiBitflags<xiiGALPipelineResourceFlags> GetValidPipelineResourceFlags(xiiEnum<xiiGALShaderResourceType> type);

  /// \brief This returns the mip level size of a given texture. This is typically used when retrieving the frame buffer size for a particular texture.
  static XII_NODISCARD xiiVec3U32 GetMipLevelSize(xiiUInt32 uiMipLevelSize, const xiiGALTextureCreationDescription& textureDescription);

  /// \brief This returns the mip size for a given mip level.
  static XII_NODISCARD xiiUInt32 GetMipSize(xiiUInt32 uiSize, xiiUInt32 uiMipLevel);

  /// \brief This returns the default texture view type for a source format and the view type that are matched with the bind flags.
  ///
  /// \param format    - The source texture format.
  /// \param viewType  - The view type to be created.
  /// \param bindFlags - The texture bind flags.
  static XII_NODISCARD xiiEnum<xiiGALTextureFormat> GetDefaultTextureViewFormat(xiiEnum<xiiGALTextureFormat> format, xiiEnum<xiiGALTextureViewType> viewType, xiiBitflags<xiiGALBindFlags> bindFlags);
};

#include <GraphicsFoundation/Utilities/Implementation/GraphicsUtilities_inl.h>
