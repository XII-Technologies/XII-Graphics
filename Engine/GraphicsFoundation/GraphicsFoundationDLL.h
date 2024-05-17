#pragma once

#include <Foundation/Basics.h>

// Configure the DLL Import/Export Define
#if XII_ENABLED(XII_COMPILE_ENGINE_AS_DLL)
#  ifdef BUILDSYSTEM_BUILDING_GRAPHICSFOUNDATION_LIB
#    define XII_GRAPHICSFOUNDATION_DLL XII_DECL_EXPORT
#  else
#    define XII_GRAPHICSFOUNDATION_DLL XII_DECL_IMPORT
#  endif
#else
#  define XII_GRAPHICSFOUNDATION_DLL
#endif

#include <Foundation/Algorithm/HashableStruct.h>
#include <Foundation/Reflection/Reflection.h>
#include <Foundation/Types/Id.h>
#include <Foundation/Types/RefCounted.h>

////////// Forward Declarations //////////

struct xiiGALSwapChainCreationDescription;
struct xiiGALCommandQueueCreationDescription;
struct xiiGALCommandListCreationDescription;
struct xiiGALBLASTriangleDescription;
struct xiiGALBLASBoundingBoxDescription;
struct xiiGALBottomLevelASCreationDescription;
struct xiiGALScratchBufferSizeDescription;
struct xiiGALBufferCreationDescription;
struct xiiGALBufferData;
struct xiiGALBufferFormat;
struct xiiGALBufferViewCreationDescription;
struct xiiGALFenceCreationDescription;
struct xiiGALFramebufferCreationDescription;
struct xiiGALQueryDataOcclusion;
struct xiiGALQueryDataBinaryOcclusion;
struct xiiGALQueryDataTimestamp;
struct xiiGALQueryDataPipelineStatistics;
struct xiiGALQueryDataDuration;
struct xiiGALQueryCreationDescription;
struct xiiGALRenderPassAttachmentDescription;
struct xiiGALAttachmentReferenceDescription;
struct xiiGALShadingRateAttachmentDescription;
struct xiiGALSubPassDescription;
struct xiiGALSubPassDependencyDescription;
struct xiiGALRenderPassCreationDescription;
struct xiiGALSamplerCreationDescription;
struct xiiGALTextureCreationDescription;
struct xiiGALTextureSubResourceData;
struct xiiGALTextureData;
struct xiiGALMappedTextureSubresource;
struct xiiGALSparseTextureProperties;
struct xiiGALTextureComponentMapping;
struct xiiGALTextureViewCreationDescription;
struct xiiGALTopLevelASBuildDescription;
struct xiiGALTopLevelASInstanceDescription;
struct xiiGALTopLevelASCreationDescription;
struct xiiGALLayoutElement;
struct xiiGALInputLayoutCreationDescription;
struct xiiGALShaderResourceDescription;
struct xiiGALShaderVariableDescription;
struct xiiGALShaderCreationDescription;
struct xiiGALRenderTargetBlendDescription;
struct xiiGALBlendStateCreationDescription;
struct xiiGALStencilOperationDescription;
struct xiiGALDepthStencilStateCreationDescription;
struct xiiGALRasterizerStateCreationDescription;
struct xiiGALPipelineStateCreationDescription;
struct xiiGALPipelineResourceSignatureCreationDescription;

class xiiGALCommandList;
class xiiGALCommandQueue;
class xiiGALObject;
class xiiGALDeviceObject;
class xiiGALDevice;
class xiiGALSwapChain;
class xiiGALResource;
class xiiGALBottomLevelAS;
class xiiGALBuffer;
class xiiGALBufferView;
class xiiGALFence;
class xiiGALFramebuffer;
class xiiGALQuery;
class xiiGALRenderPass;
class xiiGALSampler;
class xiiGALTexture;
class xiiGALTextureView;
class xiiGALTopLevelAS;
class xiiGALInputLayout;
class xiiGALShader;
class xiiGALShaderByteCode;
class xiiGALShaderResourceVariable;
class xiiGALBlendState;
class xiiGALDepthStencilState;
class xiiGALPipelineResourceSignature;
class xiiGALPipelineState;
class xiiGALRasterizerState;

struct xiiGALPipelineResourceDescription;
