#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <GraphicsD3D11/Device/DeviceD3D11.h>
#include <GraphicsD3D11/Shader/InputLayoutD3D11.h>
#include <GraphicsD3D11/Shader/ShaderD3D11.h>
#include <GraphicsFoundation/Utilities/GraphicsUtilities.h>

#include <GraphicsD3D11/Utilities/D3D11TypeConversions.h>

static const char* GALSemanticToD3D11[] = {"POSITION", "NORMAL", "TANGENT", "COLOR", "COLOR", "COLOR", "COLOR", "COLOR", "COLOR", "COLOR", "COLOR",
                                           "TEXCOORD", "TEXCOORD", "TEXCOORD", "TEXCOORD", "TEXCOORD", "TEXCOORD", "TEXCOORD", "TEXCOORD", "TEXCOORD", "TEXCOORD", "BITANGENT", "BONEINDICES",
                                           "BONEINDICES", "BONEWEIGHTS", "BONEWEIGHTS"};

XII_CHECK_AT_COMPILETIME_MSG(XII_ARRAY_SIZE(GALSemanticToD3D11) == xiiGALInputLayoutSemantic::ENUM_COUNT, "GALSemanticToD3D11 array size does not match input layout semantic count.");

static UINT GALSemanticToIndexD3D11[] = {0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 1, 0, 1};
XII_CHECK_AT_COMPILETIME_MSG(XII_ARRAY_SIZE(GALSemanticToIndexD3D11) == xiiGALInputLayoutSemantic::ENUM_COUNT, "GALSemanticToIndexD3D11 array size does not match vertex attribute semantic count.");

// clang-format off
XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALInputLayoutD3D11, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

xiiGALInputLayoutD3D11::xiiGALInputLayoutD3D11(xiiGALDeviceD3D11* pDeviceD3D11, const xiiGALInputLayoutCreationDescription& creationDescription) :
  xiiGALInputLayout(pDeviceD3D11, creationDescription)
{
}

xiiGALInputLayoutD3D11::~xiiGALInputLayoutD3D11() = default;

xiiResult xiiGALInputLayoutD3D11::InitPlatform()
{
  xiiGALDeviceD3D11* pDeviceD3D11 = static_cast<xiiGALDeviceD3D11*>(m_pDevice);
  xiiGALShaderD3D11* pShaderD3D11 = static_cast<xiiGALShaderD3D11*>(pDeviceD3D11->GetShader(m_Description.m_hShader));

  if (pShaderD3D11 == nullptr || !pShaderD3D11->GetDescription().HasByteCodeForStage(xiiGALShaderStage::Vertex))
  {
    xiiLog::Error("Shader is invalid, or does not have Vertex shader bytecode.");
    return XII_FAILURE;
  }

  xiiHybridArray<D3D11_INPUT_ELEMENT_DESC, 8> inputElementDescriptions;

  const xiiUInt32 uiLayoutCount = m_Description.m_LayoutElements.GetCount();

  for (xiiUInt32 uiLayout = 0; uiLayout < uiLayoutCount; ++uiLayout)
  {
    const auto& inputLayout = m_Description.m_LayoutElements[uiLayout];

    D3D11_INPUT_ELEMENT_DESC& layoutElement = inputElementDescriptions.ExpandAndGetRef();
    layoutElement.SemanticName              = GALSemanticToD3D11[inputLayout.m_Semantic];
    layoutElement.SemanticIndex             = GALSemanticToIndexD3D11[inputLayout.m_Semantic];
    layoutElement.AlignedByteOffset         = inputLayout.m_uiRelativeOffset;
    layoutElement.InputSlot                 = inputLayout.m_uiBufferSlot;
    layoutElement.Format                    = xiiD3D11TypeConversions::GetFormat(inputLayout.m_Format);
    layoutElement.InputSlotClass            = xiiD3D11TypeConversions::GetElementFrequency(inputLayout.m_Frequency);
    layoutElement.InstanceDataStepRate      = (inputLayout.m_Frequency == xiiGALInputElementFrequency::PerVertex) ? 0U : inputLayout.m_uiInstanceDataStepRate;

    if (layoutElement.Format == DXGI_FORMAT_UNKNOWN)
    {
      xiiLog::Error("Vertex input layout format {0} of input layout at index {1} is unknown!", inputLayout.m_Format, uiLayout);
      return XII_FAILURE;
    }
  }

  auto& byteCode = pShaderD3D11->GetDescription().m_ByteCodes[xiiGALShaderStage::GetStageIndex(xiiGALShaderStage::Vertex)];
  if (FAILED(pDeviceD3D11->GetD3D11Device()->CreateInputLayout(inputElementDescriptions.GetData(), inputElementDescriptions.GetCount(), byteCode->GetByteCode(), byteCode->GetSize(), &m_pInputLayout)))
  {
    xiiLog::Error("Failed to create the Direct3D11 input layout.");
    return XII_FAILURE;
  }
  return XII_SUCCESS;
}

xiiResult xiiGALInputLayoutD3D11::DeInitPlatform()
{
  XII_GAL_D3D11_RELEASE(m_pInputLayout);

  return XII_SUCCESS;
}

void xiiGALInputLayoutD3D11::SetDebugNamePlatform(xiiStringView sName)
{
  if (m_pInputLayout != nullptr)
  {
    xiiStringBuilder sb;
    if (FAILED(m_pInputLayout->SetPrivateData(WKPDID_D3DDebugObjectName, sName.GetElementCount(), sName.GetData(sb))))
    {
      xiiLog::Error("Failed to set the Direct3D11 input layout debug name.");
    }
  }
}

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_Shader_Implementation_InputLayoutD3D11);
