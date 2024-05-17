
XII_ALWAYS_INLINE const xiiGALShaderCreationDescription& xiiGALShader::GetDescription() const
{
  return m_Description;
}

XII_ALWAYS_INLINE xiiArrayPtr<const xiiGALVertexInputLayout> xiiGALShader::GetVertexInputLayout() const
{
  if (m_Description.HasByteCodeForStage(xiiGALShaderStage::Vertex))
  {
    return m_Description.m_ByteCodes[xiiGALShaderStage::GetStageIndex(xiiGALShaderStage::Vertex)]->m_VertexInputLayout;
  }
  return xiiArrayPtr<const xiiGALVertexInputLayout>();
}

XII_ALWAYS_INLINE xiiUInt32 xiiGALShaderPrimitiveType::GetPrimitiveTypeSize(xiiEnum<xiiGALShaderPrimitiveType> type)
{
  switch (type)
  {
    case xiiGALShaderPrimitiveType::Bool:
      return sizeof(xiiUInt32);
    case xiiGALShaderPrimitiveType::Int8:
      return sizeof(xiiInt8);
    case xiiGALShaderPrimitiveType::Int16:
      return sizeof(xiiInt16);
    case xiiGALShaderPrimitiveType::Int32:
      return sizeof(xiiInt32);
    case xiiGALShaderPrimitiveType::Int64:
      return sizeof(xiiInt64);
    case xiiGALShaderPrimitiveType::UInt8:
      return sizeof(xiiUInt8);
    case xiiGALShaderPrimitiveType::UInt16:
      return sizeof(xiiUInt16);
    case xiiGALShaderPrimitiveType::UInt32:
      return sizeof(xiiUInt32);
    case xiiGALShaderPrimitiveType::UInt64:
      return sizeof(xiiUInt64);
    case xiiGALShaderPrimitiveType::Float16:
      return sizeof(float) / 2;
    case xiiGALShaderPrimitiveType::Float32:
      return sizeof(float);
    case xiiGALShaderPrimitiveType::Double:
      return sizeof(double);
    case xiiGALShaderPrimitiveType::Min8Float:
      return 8U;
    case xiiGALShaderPrimitiveType::Min10Float:
      return 10U;
    case xiiGALShaderPrimitiveType::Min16Float:
      return 16U;
    case xiiGALShaderPrimitiveType::Min12Int:
      return 12U;
    case xiiGALShaderPrimitiveType::Min16Int:
      return 16U;
    case xiiGALShaderPrimitiveType::Min16UInt:
      return 16U;

    default:
      XII_ASSERT_DEV(false, "The requested type is not a primitive type");
  }
  return 0;
}

XII_ALWAYS_INLINE bool xiiGALShaderPrimitiveType::IsNumberType(xiiEnum<xiiGALShaderPrimitiveType> type)
{
  switch (type)
  {
    case xiiGALShaderPrimitiveType::Bool:
    case xiiGALShaderPrimitiveType::Int8:
    case xiiGALShaderPrimitiveType::Int16:
    case xiiGALShaderPrimitiveType::Int32:
    case xiiGALShaderPrimitiveType::Int64:
    case xiiGALShaderPrimitiveType::UInt8:
    case xiiGALShaderPrimitiveType::UInt16:
    case xiiGALShaderPrimitiveType::UInt32:
    case xiiGALShaderPrimitiveType::UInt64:
    case xiiGALShaderPrimitiveType::Float16:
    case xiiGALShaderPrimitiveType::Float32:
    case xiiGALShaderPrimitiveType::Double:
    case xiiGALShaderPrimitiveType::Min8Float:
    case xiiGALShaderPrimitiveType::Min10Float:
    case xiiGALShaderPrimitiveType::Min16Float:
    case xiiGALShaderPrimitiveType::Min12Int:
    case xiiGALShaderPrimitiveType::Min16Int:
    case xiiGALShaderPrimitiveType::Min16UInt:
      return true;
  }
  return false;
}

XII_FORCE_INLINE xiiGALShaderCreationDescription::xiiGALShaderCreationDescription() :
  xiiHashableStruct()
{
}

XII_FORCE_INLINE xiiGALShaderCreationDescription::~xiiGALShaderCreationDescription()
{
  for (xiiUInt32 i = 0; i < xiiGALShaderStage::ENUM_COUNT; ++i)
  {
    xiiGALShaderByteCode* pByteCode = m_ByteCodes[i];
    m_ByteCodes[i]                  = nullptr;

    if (pByteCode != nullptr && pByteCode->GetRefCount() == 0)
    {
      XII_DEFAULT_DELETE(pByteCode);
    }
  }
}

XII_FORCE_INLINE bool xiiGALShaderCreationDescription::HasByteCodeForStage(xiiGALShaderStage::Enum stage) const
{
  if (stage == xiiGALShaderStage::Unknown)
    return false;

  const xiiUInt32 uiStageIndex = xiiGALShaderStage::GetStageIndex(stage);
  return m_ByteCodes[uiStageIndex] != nullptr && m_ByteCodes[uiStageIndex]->IsValid();
}
