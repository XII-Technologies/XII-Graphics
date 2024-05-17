#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/Shader/ShaderByteCode.h>
#include <GraphicsFoundation/Shader/Types.h>

// clang-format off
XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALShaderResourceType, 1)
  XII_ENUM_CONSTANT(xiiGALShaderResourceType::Unknown),
  XII_ENUM_CONSTANT(xiiGALShaderResourceType::ConstantBuffer),
  XII_ENUM_CONSTANT(xiiGALShaderResourceType::TextureSRV),
  XII_ENUM_CONSTANT(xiiGALShaderResourceType::BufferSRV),
  XII_ENUM_CONSTANT(xiiGALShaderResourceType::TextureUAV),
  XII_ENUM_CONSTANT(xiiGALShaderResourceType::BufferUAV),
  XII_ENUM_CONSTANT(xiiGALShaderResourceType::Sampler),
  XII_ENUM_CONSTANT(xiiGALShaderResourceType::InputAttachment),
  XII_ENUM_CONSTANT(xiiGALShaderResourceType::AccelerationStructure),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALShaderTextureType, 1)
  XII_ENUM_CONSTANT(xiiGALShaderTextureType::Unknown),
  XII_ENUM_CONSTANT(xiiGALShaderTextureType::Texture1D),
  XII_ENUM_CONSTANT(xiiGALShaderTextureType::Texture1DArray),
  XII_ENUM_CONSTANT(xiiGALShaderTextureType::Texture2D),
  XII_ENUM_CONSTANT(xiiGALShaderTextureType::Texture2DArray),
  XII_ENUM_CONSTANT(xiiGALShaderTextureType::Texture2DMS),
  XII_ENUM_CONSTANT(xiiGALShaderTextureType::Texture2DMSArray),
  XII_ENUM_CONSTANT(xiiGALShaderTextureType::Texture3D),
  XII_ENUM_CONSTANT(xiiGALShaderTextureType::TextureCube),
  XII_ENUM_CONSTANT(xiiGALShaderTextureType::TextureCubeArray),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALShaderPrimitiveType, 1)
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::Unknown),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::Void),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::Bool),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::Int8),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::Int16),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::Int32),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::Int64),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::UInt8),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::UInt16),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::UInt32),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::UInt64),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::Float16),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::Float32),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::Double),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::Min8Float),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::Min10Float),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::Min16Float),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::Min12Int),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::Min16Int),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::Min16UInt),
  XII_ENUM_CONSTANT(xiiGALShaderPrimitiveType::String),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALShaderVariableClassType, 1)
  XII_ENUM_CONSTANT(xiiGALShaderVariableClassType::Unknown),
  XII_ENUM_CONSTANT(xiiGALShaderVariableClassType::Scalar),
  XII_ENUM_CONSTANT(xiiGALShaderVariableClassType::Array),
  XII_ENUM_CONSTANT(xiiGALShaderVariableClassType::MatrixRows),
  XII_ENUM_CONSTANT(xiiGALShaderVariableClassType::MatrixColumns),
  XII_ENUM_CONSTANT(xiiGALShaderVariableClassType::Struct),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALShaderByteCode, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

void xiiGALShaderVariableDescription::CopyDataFormVariant(xiiUInt8* pDestination, xiiVariant* pValue, const xiiGALShaderVariableDescription& description)
{
  /// \todo Perform row-column major conversion on matrices.

  xiiResult conversionResult = XII_FAILURE;

  XII_ASSERT_DEV(description.m_Class == xiiGALShaderVariableClassType::Scalar || xiiGALShaderVariableClassType::Array || xiiGALShaderVariableClassType::MatrixColumns || xiiGALShaderVariableClassType::MatrixRows, "");
  XII_ASSERT_DEV(xiiGALShaderPrimitiveType::IsNumberType(description.m_PrimitiveType), "Only number types are supported.");

  if (pValue != nullptr)
  {
    switch (description.m_PrimitiveType)
    {
      case xiiGALShaderPrimitiveType::Bool:
      {
        if (description.m_uiColumnCount != 1 || description.m_uiRowCount != 1)
        {
          XII_ASSERT_NOT_IMPLEMENTED;
        }
        else
        {
          *reinterpret_cast<xiiShaderBool*>(pDestination) = pValue->ConvertTo<bool>(&conversionResult);
        }
      }
      break;
      case xiiGALShaderPrimitiveType::Int8:
      {
        if (description.m_uiColumnCount != 1 || description.m_uiRowCount != 1)
        {
          XII_ASSERT_NOT_IMPLEMENTED;
        }
        else
        {
          *reinterpret_cast<xiiInt8*>(pDestination) = pValue->ConvertTo<xiiInt8>(&conversionResult);
        }
      }
      break;
      case xiiGALShaderPrimitiveType::Int16:
      {
        if (description.m_uiColumnCount != 1 || description.m_uiRowCount != 1)
        {
          XII_ASSERT_NOT_IMPLEMENTED;
        }
        else
        {
          *reinterpret_cast<xiiInt16*>(pDestination) = pValue->ConvertTo<xiiInt16>(&conversionResult);
        }
      }
      break;
      case xiiGALShaderPrimitiveType::Int32:
      {
        if (description.m_uiRowCount != 1 || description.m_uiColumnCount == 0 || description.m_uiColumnCount > 4)
        {
          XII_ASSERT_NOT_IMPLEMENTED;
        }
        else
        {
          switch (description.m_uiColumnCount)
          {
            case 1:
              *reinterpret_cast<xiiInt32*>(pDestination) = pValue->ConvertTo<xiiInt32>(&conversionResult);
              break;
            case 2:
              *reinterpret_cast<xiiVec2I32*>(pDestination) = pValue->Get<xiiVec2I32>();
              break;
            case 3:
              *reinterpret_cast<xiiVec3I32*>(pDestination) = pValue->Get<xiiVec3I32>();
              break;
            case 4:
              *reinterpret_cast<xiiVec4I32*>(pDestination) = pValue->Get<xiiVec4I32>();
              break;

              XII_DEFAULT_CASE_NOT_IMPLEMENTED;
          }
        }
      }
      break;
      case xiiGALShaderPrimitiveType::Int64:
      {
        if (description.m_uiRowCount != 1 || description.m_uiColumnCount == 0 || description.m_uiColumnCount > 4)
        {
          XII_ASSERT_NOT_IMPLEMENTED;
        }
        else
        {
          switch (description.m_uiColumnCount)
          {
            case 1:
              *reinterpret_cast<xiiInt64*>(pDestination) = pValue->ConvertTo<xiiInt64>(&conversionResult);
              break;
            case 2:
              *reinterpret_cast<xiiVec2I64*>(pDestination) = pValue->Get<xiiVec2I64>();
              break;
            case 3:
              *reinterpret_cast<xiiVec3I64*>(pDestination) = pValue->Get<xiiVec3I64>();
              break;
            case 4:
              *reinterpret_cast<xiiVec4I64*>(pDestination) = pValue->Get<xiiVec4I64>();
              break;

              XII_DEFAULT_CASE_NOT_IMPLEMENTED;
          }
        }
      }
      break;
      case xiiGALShaderPrimitiveType::UInt8:
      {
        if (description.m_uiColumnCount != 1 || description.m_uiRowCount != 1)
        {
          XII_ASSERT_NOT_IMPLEMENTED;
        }
        else
        {
          *reinterpret_cast<xiiUInt8*>(pDestination) = pValue->ConvertTo<xiiUInt8>(&conversionResult);
        }
      }
      break;
      case xiiGALShaderPrimitiveType::UInt16:
      {
        if (description.m_uiColumnCount != 1 || description.m_uiRowCount != 1)
        {
          XII_ASSERT_NOT_IMPLEMENTED;
        }
        else
        {
          *reinterpret_cast<xiiUInt16*>(pDestination) = pValue->ConvertTo<xiiUInt16>(&conversionResult);
        }
      }
      break;
      case xiiGALShaderPrimitiveType::UInt32:
      {
        if (description.m_uiRowCount != 1 || description.m_uiColumnCount == 0 || description.m_uiColumnCount > 4)
        {
          XII_ASSERT_NOT_IMPLEMENTED;
        }
        else
        {
          switch (description.m_uiColumnCount)
          {
            case 1:
              *reinterpret_cast<xiiUInt32*>(pDestination) = pValue->ConvertTo<xiiUInt32>(&conversionResult);
              break;
            case 2:
              *reinterpret_cast<xiiVec2U32*>(pDestination) = pValue->Get<xiiVec2U32>();
              break;
            case 3:
              *reinterpret_cast<xiiVec3U32*>(pDestination) = pValue->Get<xiiVec3U32>();
              break;
            case 4:
              *reinterpret_cast<xiiVec4U32*>(pDestination) = pValue->Get<xiiVec4U32>();
              break;

              XII_DEFAULT_CASE_NOT_IMPLEMENTED;
          }
        }
      }
      break;
      case xiiGALShaderPrimitiveType::UInt64:
      {
        if (description.m_uiRowCount != 1 || description.m_uiColumnCount == 0 || description.m_uiColumnCount > 4)
        {
          XII_ASSERT_NOT_IMPLEMENTED;
        }
        else
        {
          switch (description.m_uiColumnCount)
          {
            case 1:
              *reinterpret_cast<xiiUInt64*>(pDestination) = pValue->ConvertTo<xiiUInt64>(&conversionResult);
              break;
            case 2:
              *reinterpret_cast<xiiVec2U64*>(pDestination) = pValue->Get<xiiVec2U64>();
              break;
            case 3:
              *reinterpret_cast<xiiVec3U64*>(pDestination) = pValue->Get<xiiVec3U64>();
              break;
            case 4:
              *reinterpret_cast<xiiVec4U64*>(pDestination) = pValue->Get<xiiVec4U64>();
              break;

              XII_DEFAULT_CASE_NOT_IMPLEMENTED;
          }
        }
      }
      break;
      case xiiGALShaderPrimitiveType::Float32:
      {
        if (description.m_uiRowCount == 4 && description.m_uiColumnCount == 4)
        {
          *reinterpret_cast<xiiMat4*>(pDestination) = pValue->Get<xiiMat4>();
        }
        else if (description.m_uiRowCount == 3 && description.m_uiColumnCount == 3)
        {
          *reinterpret_cast<xiiMat3*>(pDestination) = pValue->Get<xiiMat3>();
        }
        else if (description.m_uiRowCount != 1 || description.m_uiColumnCount == 0 || description.m_uiColumnCount > 4)
        {
          XII_ASSERT_NOT_IMPLEMENTED;
        }
        else if (description.m_uiRowCount == 1)
        {
          switch (description.m_uiColumnCount)
          {
            case 1:
              *reinterpret_cast<float*>(pDestination) = pValue->ConvertTo<float>(&conversionResult);
              break;
            case 2:
              *reinterpret_cast<xiiVec2*>(pDestination) = pValue->Get<xiiVec2>();
              break;
            case 3:
              *reinterpret_cast<xiiVec3*>(pDestination) = pValue->Get<xiiVec3>();
              break;
            case 4:
            {
              if (pValue->GetType() == xiiVariant::Type::Color || pValue->GetType() == xiiVariant::Type::ColorGamma)
              {
                const xiiColor tmp                        = pValue->ConvertTo<xiiColor>();
                *reinterpret_cast<xiiVec4*>(pDestination) = *reinterpret_cast<const xiiVec4*>(&tmp);
              }
              else
              {
                *reinterpret_cast<xiiVec4*>(pDestination) = pValue->Get<xiiVec4>();
              }
            }
            break;

              XII_DEFAULT_CASE_NOT_IMPLEMENTED;
          }
        }
      }
      break;
      case xiiGALShaderPrimitiveType::Double:
      {
        if (description.m_uiRowCount == 4 && description.m_uiColumnCount == 4)
        {
          *reinterpret_cast<xiiMat4d*>(pDestination) = pValue->Get<xiiMat4d>();
        }
        else if (description.m_uiRowCount == 3 && description.m_uiColumnCount == 3)
        {
          *reinterpret_cast<xiiMat3d*>(pDestination) = pValue->Get<xiiMat3d>();
        }
        else if (description.m_uiRowCount != 1 || description.m_uiColumnCount == 0 || description.m_uiColumnCount > 4)
        {
          XII_ASSERT_NOT_IMPLEMENTED;
        }
        else if (description.m_uiRowCount == 1)
        {
          switch (description.m_uiColumnCount)
          {
            case 1:
              *reinterpret_cast<float*>(pDestination) = pValue->ConvertTo<float>(&conversionResult);
              break;
            case 2:
              *reinterpret_cast<xiiVec2d*>(pDestination) = pValue->Get<xiiVec2d>();
              break;
            case 3:
              *reinterpret_cast<xiiVec3d*>(pDestination) = pValue->Get<xiiVec3d>();
              break;
            case 4:
              *reinterpret_cast<xiiVec4d*>(pDestination) = pValue->Get<xiiVec4d>();
              break;

              XII_DEFAULT_CASE_NOT_IMPLEMENTED;
          }
        }
      }
      break;

        XII_DEFAULT_CASE_NOT_IMPLEMENTED;
    }
  }
}

xiiGALShaderByteCode::xiiGALShaderByteCode() = default;

xiiGALShaderByteCode::xiiGALShaderByteCode(const xiiArrayPtr<const xiiUInt8>& byteCode)
{
  CopyFrom(byteCode);
}

void xiiGALShaderByteCode::CopyFrom(const xiiArrayPtr<const xiiUInt8>& pByteCode)
{
  XII_ASSERT_DEV(pByteCode.GetPtr() != nullptr && pByteCode.GetCount() != 0, "The given shader byte code is invalid!");

  m_ByteCode = pByteCode;
}

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_Shader_Implementation_ShaderByteCode);
