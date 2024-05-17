#pragma once

#include <GraphicsNull/GraphicsNullDLL.h>

#include <GraphicsFoundation/Resources/Query.h>

class XII_GRAPHICSNULL_DLL xiiGALQueryNull final : public xiiGALQuery
{
public:
  virtual bool GetData(void* pData, xiiUInt32 uiDataSize, bool bAutoInvalidate = true) override final;

  virtual void Invalidate() override final;

protected:
  friend class xiiGALDeviceNull;
  friend class xiiMemoryUtils;

  xiiGALQueryNull(xiiGALDeviceNull* pDeviceNull, const xiiGALQueryCreationDescription& creationDescription);

  virtual ~xiiGALQueryNull();

  virtual xiiResult InitPlatform() override final;

  virtual xiiResult DeInitPlatform() override final;
};

#include <GraphicsNull/Resources/Implementation/QueryNull_inl.h>
