#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <Foundation/Math/Mat3.h>
#include <Foundation/Math/Transform.h>

/// \brief A wrapper class that converts a xiiMat3 into the correct data layout for shaders.
class xiiShaderMat3
{
public:
  XII_DECLARE_POD_TYPE();

  XII_ALWAYS_INLINE xiiShaderMat3() = default;

  XII_ALWAYS_INLINE xiiShaderMat3(const xiiMat3& m) { *this = m; }

  XII_FORCE_INLINE void operator=(const xiiMat3& m)
  {
    for (xiiUInt32 c = 0; c < 3; ++c)
    {
      m_Data[c * 4 + 0] = m.Element(c, 0);
      m_Data[c * 4 + 1] = m.Element(c, 1);
      m_Data[c * 4 + 2] = m.Element(c, 2);
      m_Data[c * 4 + 3] = 0.0f;
    }
  }

private:
  float m_Data[12];
};

/// \brief A wrapper class that converts a xiiTransform into the correct data layout for shaders.
class xiiShaderTransform
{
public:
  XII_DECLARE_POD_TYPE();

  XII_ALWAYS_INLINE xiiShaderTransform() = default;

  inline void operator=(const xiiTransform& t) { *this = t.GetAsMat4(); }

  inline void operator=(const xiiMat4& t)
  {
    float data[16];
    t.GetAsArray(data, xiiMatrixLayout::RowMajor);

    for (xiiUInt32 i = 0; i < 12; ++i)
    {
      m_Data[i] = data[i];
    }
  }

  inline void operator=(const xiiMat3& t)
  {
    float data[9];
    t.GetAsArray(data, xiiMatrixLayout::RowMajor);

    m_Data[0] = data[0];
    m_Data[1] = data[1];
    m_Data[2] = data[2];
    m_Data[3] = 0;

    m_Data[4] = data[3];
    m_Data[5] = data[4];
    m_Data[6] = data[5];
    m_Data[7] = 0;

    m_Data[8]  = data[6];
    m_Data[9]  = data[7];
    m_Data[10] = data[8];
    m_Data[11] = 0;
  }

  inline xiiMat4 GetAsMat4() const
  {
    xiiMat4 res;
    res.SetRow(0, reinterpret_cast<const xiiVec4&>(m_Data[0]));
    res.SetRow(1, reinterpret_cast<const xiiVec4&>(m_Data[4]));
    res.SetRow(2, reinterpret_cast<const xiiVec4&>(m_Data[8]));
    res.SetRow(3, xiiVec4(0, 0, 0, 1));

    return res;
  }

  inline xiiVec3 GetTranslationVector() const
  {
    return xiiVec3(m_Data[3], m_Data[7], m_Data[11]);
  }

private:
  float m_Data[12];
};

/// \brief A wrapper class that converts a bool into the correct data layout for shaders.
class xiiShaderBool
{
public:
  XII_DECLARE_POD_TYPE();

  XII_ALWAYS_INLINE xiiShaderBool() = default;

  XII_ALWAYS_INLINE xiiShaderBool(bool b) { m_uiData = b ? 0xFFFFFFFFU : 0U; }

  XII_ALWAYS_INLINE void operator=(bool b) { m_uiData = b ? 0xFFFFFFFFU : 0U; }

private:
  xiiUInt32 m_uiData;
};
