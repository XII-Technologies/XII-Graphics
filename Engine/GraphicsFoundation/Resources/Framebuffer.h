#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <Foundation/Math/Size.h>
#include <GraphicsFoundation/Declarations/DeviceObject.h>
#include <GraphicsFoundation/Declarations/GraphicsTypes.h>

/// \brief This describes the frame buffer creation description.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALFramebufferCreationDescription : public xiiHashableStruct<xiiGALFramebufferCreationDescription>
{
  XII_DECLARE_POD_TYPE();

  xiiStringView                                m_sName;                                  ///< Resource name. The default is an empty string view.
  xiiGALRenderPassHandle                       m_hRenderPass;                            ///< The handle to the render pass that the frame buffer will be compatible with.
  xiiHybridArray<xiiGALTextureViewHandle, 16U> m_Attachments;                            ///< An array of attachments.
  xiiSizeU32                                   m_FramebufferSize   = xiiSizeU32(0U, 0U); ///< The size of the frame buffer. The default is (0, 0).
  xiiUInt32                                    m_uiArraySliceCount = 0U;                 ///< The number of array slices in the frame buffer. The default is 0.
};

/// \brief Interface that defines methods to manipulate a frame buffer object.
class XII_GRAPHICSFOUNDATION_DLL xiiGALFramebuffer : public xiiGALDeviceObject
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALFramebuffer, xiiGALDeviceObject);

public:
  /// \brief This returns the creation description for this object.
  XII_NODISCARD const xiiGALFramebufferCreationDescription& GetDescription() const;

protected:
  friend class xiiGALDevice;

  xiiGALFramebuffer(xiiGALDevice* pDevice, const xiiGALFramebufferCreationDescription& creationDescription);

  virtual ~xiiGALFramebuffer();

  virtual xiiResult InitPlatform() = 0;

  virtual xiiResult DeInitPlatform() = 0;

protected:
  xiiGALFramebufferCreationDescription m_Description;
};

#include <GraphicsFoundation/Resources/Implementation/Framebuffer_inl.h>
