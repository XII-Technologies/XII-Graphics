#pragma once

#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/Declarations/DeviceObject.h>

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALDeviceObject, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE

xiiGALDeviceObject::xiiGALDeviceObject(xiiGALDevice* pDevice) :
  m_pDevice(pDevice)
{
}

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_Declarations_Implementation_DeviceObject);
