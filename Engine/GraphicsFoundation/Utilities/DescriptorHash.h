#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <GraphicsFoundation/Declarations/Descriptors.h>

/// \brief Provides hash functions for GAL descriptor objects.
class XII_GRAPHICSFOUNDATION_DLL xiiGALDescriptorHash
{
public:
  static xiiUInt32 Hash(const xiiGALPipelineStateCreationDescription& description);
  static bool      Equal(const xiiGALPipelineStateCreationDescription& a, const xiiGALPipelineStateCreationDescription& b);

  static xiiUInt32 Hash(const xiiGALPipelineResourceSignatureCreationDescription& description);
  static bool      Equal(const xiiGALPipelineResourceSignatureCreationDescription& a, const xiiGALPipelineResourceSignatureCreationDescription& b);
};
