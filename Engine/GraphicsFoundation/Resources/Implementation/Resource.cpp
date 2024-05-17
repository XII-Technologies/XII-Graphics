#pragma once

#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/Resources/Resource.h>

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALResource, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;

XII_BEGIN_DYNAMIC_REFLECTED_TYPE(xiiGALResourceView, 1, xiiRTTINoAllocator)
XII_END_DYNAMIC_REFLECTED_TYPE;

xiiGALResource::xiiGALResource(xiiGALDevice* pDevice) :
  xiiGALDeviceObject(pDevice)
{
}

xiiGALResourceView::xiiGALResourceView(xiiGALDevice* pDevice) :
  xiiGALDeviceObject(pDevice)
{
}

XII_STATICLINK_FILE(GraphicsFoundation, GraphicsFoundation_Resources_Implementation_Resource);
