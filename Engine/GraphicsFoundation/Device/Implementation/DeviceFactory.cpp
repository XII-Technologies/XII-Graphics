#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/Device/DeviceFactory.h>

struct CreatorFuncInfo
{
  xiiGALDeviceFactory::CreatorFunc m_Func;
  xiiString                        m_sShaderModel;
  xiiString                        m_sShaderCompiler;
};

static xiiHashTable<xiiString, CreatorFuncInfo> s_CreatorFunctions;

CreatorFuncInfo* GetCreatorFuncInfo(xiiStringView sImplementationName)
{
  auto pFuncInfo = s_CreatorFunctions.GetValue(sImplementationName);
  if (pFuncInfo == nullptr)
  {
    xiiStringBuilder sPluginName = "xiiGraphics";
    sPluginName.Append(sImplementationName);

    XII_VERIFY(xiiPlugin::LoadPlugin(sPluginName).Succeeded(), "Graphics API plugin '{}' not found.", sPluginName);

    pFuncInfo = s_CreatorFunctions.GetValue(sImplementationName);
    XII_ASSERT_DEV(pFuncInfo != nullptr, "Graphics API plugin '{}' is not registered.", sImplementationName);
  }

  return pFuncInfo;
}

xiiInternal::NewInstance<xiiGALDevice> xiiGALDeviceFactory::CreateDevice(xiiStringView sImplementationName, xiiAllocatorBase* pAllocator, const xiiGALDeviceCreationDescription& description)
{
  if (auto pFuncInfo = GetCreatorFuncInfo(sImplementationName))
  {
    return pFuncInfo->m_Func(pAllocator, description);
  }

  return xiiInternal::NewInstance<xiiGALDevice>(nullptr, pAllocator);
}

void xiiGALDeviceFactory::RegisterImplementation(xiiStringView sImplementationName, const CreatorFunc& func, const xiiGALDeviceImplementationDescription& description)
{
  CreatorFuncInfo info;
  info.m_Func            = func;
  info.m_sShaderModel    = description.m_sShaderModel;
  info.m_sShaderCompiler = description.m_sShaderCompiler;

  XII_VERIFY(s_CreatorFunctions.Insert(sImplementationName, info) == false, "Graphics API implementation is already registered.");
}

void xiiGALDeviceFactory::UnregisterImplementation(xiiStringView sImplementationName)
{
  XII_VERIFY(s_CreatorFunctions.Remove(sImplementationName), "Graphics API implementation is not registered.");
}

void xiiGALDeviceFactory::GetShaderModelAndCompiler(xiiStringView sRendererName, xiiStringView& ref_sShaderModel, xiiStringView& ref_sShaderCompiler)
{
  if (auto pFuncInfo = GetCreatorFuncInfo(sRendererName))
  {
    ref_sShaderModel    = pFuncInfo->m_sShaderModel;
    ref_sShaderCompiler = pFuncInfo->m_sShaderCompiler;
  }
}

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_Device_Implementation_DeviceFactory);
