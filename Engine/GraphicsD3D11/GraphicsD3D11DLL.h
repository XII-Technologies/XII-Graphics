#pragma once

#include <Foundation/Basics.h>

// Configure the DLL Import/Export Define
#if XII_ENABLED(XII_COMPILE_ENGINE_AS_DLL)
#  ifdef BUILDSYSTEM_BUILDING_GRAPHICSD3D11_LIB
#    define XII_GRAPHICSD3D11_DLL XII_DECL_EXPORT
#  else
#    define XII_GRAPHICSD3D11_DLL XII_DECL_IMPORT
#  endif
#else
#  define XII_GRAPHICSD3D11_DLL
#endif

#define XII_GAL_D3D11_RELEASE(pObject) \
  do                                   \
  {                                    \
    if ((pObject) != nullptr)          \
    {                                  \
      (pObject)->Release();            \
      (pObject) = nullptr;             \
    }                                  \
  } while (0)

#define XII_GAL_D3D11_RELEASE_ARRAY(pArrayObject) \
  do                                              \
  {                                               \
    for (auto pObject : (pArrayObject))           \
    {                                             \
      if (pObject != nullptr)                     \
      {                                           \
        pObject->Release();                       \
        pObject = nullptr;                        \
      }                                           \
    }                                             \
  } while (0)

#define D3D11_BOOL(expression) (expression) ? TRUE : FALSE

#define FORCE_NTDDI_WIN10_19H1 0

////////// Forward Declarations //////////

class xiiGALCommandListD3D11;
class xiiGALCommandQueueD3D11;
class xiiGALDeviceD3D11;
class xiiGALPassD3D11;
class xiiGALSwapChainD3D11;
class xiiGALBottomLevelASD3D11;
class xiiGALBufferD3D11;
class xiiGALBufferViewD3D11;
class xiiGALFenceD3D11;
class xiiGALFramebufferD3D11;
class xiiGALQueryD3D11;
class xiiGALRenderPassD3D11;
class xiiGALSamplerD3D11;
class xiiGALTextureD3D11;
class xiiGALTextureViewD3D11;
class xiiGALTopLevelASD3D11;
class xiiGALInputLayoutD3D11;
class xiiGALShaderD3D11;
class xiiGALBlendStateD3D11;
class xiiGALDepthStencilStateD3D11;
class xiiGALRasterizerStateD3D11;
class xiiGALPipelineStateD3D11;
class xiiGALPipelineResourceSignatureD3D11;
class xiiGALShaderResourceVariableD3D11;
