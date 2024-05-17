#pragma once

#include <Foundation/Basics.h>

// Configure the DLL Import/Export Define
#if XII_ENABLED(XII_COMPILE_ENGINE_AS_DLL)
#  ifdef BUILDSYSTEM_BUILDING_GRAPHICSNULL_LIB
#    define XII_GRAPHICSNULL_DLL XII_DECL_EXPORT
#  else
#    define XII_GRAPHICSNULL_DLL XII_DECL_IMPORT
#  endif
#else
#  define XII_GRAPHICSNULL_DLL
#endif

////////// Forward Declarations //////////

class xiiGALCommandListNull;
class xiiGALCommandQueueNull;
class xiiGALDeviceNull;
class xiiGALPassNull;
class xiiGALSwapChainNull;
class xiiGALBottomLevelASNull;
class xiiGALBufferNull;
class xiiGALBufferViewNull;
class xiiGALFenceNull;
class xiiGALFramebufferNull;
class xiiGALQueryNull;
class xiiGALRenderPassNull;
class xiiGALSamplerNull;
class xiiGALTextureNull;
class xiiGALTextureViewNull;
class xiiGALTopLevelASNull;
class xiiGALInputLayoutNull;
class xiiGALShaderNull;
class xiiGALShaderResourceVariableNull;
class xiiGALBlendStateNull;
class xiiGALDepthStencilStateNull;
class xiiGALRasterizerStateNull;
class xiiGALPipelineResourceSignatureNull;
class xiiGALPipelineStateNull;
