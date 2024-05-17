#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Declarations/DeviceObject.h>
#include <GraphicsFoundation/Declarations/GraphicsTypes.h>
#include <GraphicsFoundation/Shader/ShaderByteCode.h>

/// \brief This describes the shader creation description.
struct XII_GRAPHICSFOUNDATION_DLL xiiGALShaderCreationDescription : public xiiHashableStruct<xiiGALShaderCreationDescription>
{
  xiiGALShaderCreationDescription();
  ~xiiGALShaderCreationDescription();

  bool HasByteCodeForStage(xiiGALShaderStage::Enum stage) const;

  xiiStringView                  m_sName;                                                    ///< Resource name. The default is an empty string view.
  xiiBitflags<xiiGALShaderStage> m_ShaderStage                 = xiiGALShaderStage::Unknown; ///< The shader stages. The default is xiiGALShaderStage::Unknown.
  bool                           m_bUseCombinedTextureSamplers = false;                      ///< If set to true, textures will be combined with texture samplers.
                                                                                             ///<
                                                                                             ///< The m_bCombinedSamplerSuffix member defines the suffix added to the texture variable name to get corresponding sampler name.
                                                                                             ///< When using combined samplers, the sampler assigned to the shader resource view is automatically set when
                                                                                             ///< the view is bound. Otherwise, samplers need to be explicitly set similar to other shader variables.
                                                                                             ///<
                                                                                             ///< This member has no effect if the shader is used in the PSO that uses pipeline resource signature(s).

  xiiStringView m_sCombinedSamplerSuffix = "_Sampler"; ///< If m_bUseCombinedTextureSamplers is true, defines the suffix added to the texture variable name to get corresponding sampler name. For example,
                                                       ///< for the default value "_Sampler", a texture named "Tex" will be combined with the sampler named "Tex_Sampler". If m_bUseCombinedTextureSamplers is false, this member is ignored.
                                                       ///<
                                                       ///< This member has no effect if the shader is used in the PSO that uses pipeline resource signature(s).

  xiiScopedRefPointer<xiiGALShaderByteCode> m_ByteCodes[xiiGALShaderStage::ENUM_COUNT]; ///< The shader byte code per stage.
};

/// \brief Interface that defines methods to manipulate a shader object.
class XII_GRAPHICSFOUNDATION_DLL xiiGALShader : public xiiGALDeviceObject
{
  XII_ADD_DYNAMIC_REFLECTION(xiiGALShader, xiiGALDeviceObject);

public:
  /// \brief This returns the creation description for this object.
  XII_NODISCARD const xiiGALShaderCreationDescription& GetDescription() const;

  /// \brief This returns the vertex input layout of the vertex shader.
  xiiArrayPtr<const xiiGALVertexInputLayout> GetVertexInputLayout() const;

protected:
  friend class xiiGALDevice;

  xiiGALShader(xiiGALDevice* pDevice, const xiiGALShaderCreationDescription& creationDescription);

  virtual ~xiiGALShader();

  virtual xiiResult InitPlatform() = 0;

  virtual xiiResult DeInitPlatform() = 0;

protected:
  xiiGALShaderCreationDescription m_Description;
};


#include <GraphicsFoundation/Shader/Implementation/Shader_inl.h>
