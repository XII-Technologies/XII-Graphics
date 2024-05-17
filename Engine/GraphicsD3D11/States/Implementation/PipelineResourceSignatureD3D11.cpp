#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <GraphicsD3D11/Device/DeviceD3D11.h>
#include <GraphicsD3D11/States/PipelineResourceSignatureD3D11.h>

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALPipelineResourceSignatureD3D11, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALPipelineResourceSignatureD3D11::xiiGALPipelineResourceSignatureD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALPipelineResourceSignatureCreationDescription& creationDescription) :
  xiiGALPipelineResourceSignature(pDeviceD3D11, creationDescription)
{
}

xiiGALPipelineResourceSignatureD3D11::~xiiGALPipelineResourceSignatureD3D11() = default;

xiiResult xiiGALPipelineResourceSignatureD3D11::InitPlatform()
{
  return XII_SUCCESS;
}

xiiResult xiiGALPipelineResourceSignatureD3D11::DeInitPlatform()
{
  return XII_SUCCESS;
}

bool xiiGALPipelineResourceSignatureD3D11::IsCompatibleWith(const xiiGALPipelineResourceSignature* pPipelineResourceSignature) const
{
  const xiiGALPipelineResourceSignatureD3D11* pPipelineResourceSignatureD3D11 = static_cast<const xiiGALPipelineResourceSignatureD3D11*>(pPipelineResourceSignature);

  ///\todo GraphicsD3D11: Implement pipeline resource signature compatibility.
  return false;
}

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_States_Implementation_PipelineResourceSignatureD3D11);
