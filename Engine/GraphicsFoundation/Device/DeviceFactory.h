#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <Foundation/Types/Delegate.h>
#include <GraphicsFoundation/Declarations/Descriptors.h>

struct XII_GRAPHICSFOUNDATION_DLL xiiGALDeviceImplementationDescription
{
  xiiEnum<xiiGALGraphicsDeviceType> m_APIType = xiiGALGraphicsDeviceType::Undefined;
  xiiString                         m_sShaderModel;
  xiiString                         m_sShaderCompiler;
};

struct XII_GRAPHICSFOUNDATION_DLL xiiGALDeviceFactory
{
  using CreatorFunc = xiiDelegate<xiiInternal::NewInstance<xiiGALDevice>(xiiAllocatorBase*, const xiiGALDeviceCreationDescription&)>;

  static xiiInternal::NewInstance<xiiGALDevice> CreateDevice(xiiStringView sImplementationName, xiiAllocatorBase* pAllocator, const xiiGALDeviceCreationDescription& description);

  static void RegisterImplementation(xiiStringView sImplementationName, const CreatorFunc& func, const xiiGALDeviceImplementationDescription& description);

  static void UnregisterImplementation(xiiStringView sImplementationName);

  static void GetShaderModelAndCompiler(xiiStringView sRendererName, xiiStringView& ref_sShaderModel, xiiStringView& ref_sShaderCompiler);
};

#include <GraphicsFoundation/Device/Implementation/DeviceFactory_inl.h>
