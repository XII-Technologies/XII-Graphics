#include <GraphicsFoundation/GraphicsFoundationPCH.h>

#include <GraphicsFoundation/Declarations/GraphicsTypes.h>

// clang-format off

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALGraphicsDeviceType, 1)
  XII_ENUM_CONSTANT(xiiGALGraphicsDeviceType::Undefined),
  XII_ENUM_CONSTANT(xiiGALGraphicsDeviceType::Null),
  XII_ENUM_CONSTANT(xiiGALGraphicsDeviceType::OpenGLES),
  XII_ENUM_CONSTANT(xiiGALGraphicsDeviceType::OpenGL),
  XII_ENUM_CONSTANT(xiiGALGraphicsDeviceType::Direct3D11),
  XII_ENUM_CONSTANT(xiiGALGraphicsDeviceType::Direct3D12),
  XII_ENUM_CONSTANT(xiiGALGraphicsDeviceType::Vulkan),
  XII_ENUM_CONSTANT(xiiGALGraphicsDeviceType::Metal),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALDeviceFeatureState, 1)
  XII_ENUM_CONSTANT(xiiGALDeviceFeatureState::Disabled),
  XII_ENUM_CONSTANT(xiiGALDeviceFeatureState::Enabled),
  XII_ENUM_CONSTANT(xiiGALDeviceFeatureState::Optional),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALGraphicsAdapterVendor, 1)
  XII_ENUM_CONSTANT(xiiGALGraphicsAdapterVendor::Unknown),
  XII_ENUM_CONSTANT(xiiGALGraphicsAdapterVendor::Nvidia),
  XII_ENUM_CONSTANT(xiiGALGraphicsAdapterVendor::AMD),
  XII_ENUM_CONSTANT(xiiGALGraphicsAdapterVendor::Intel),
  XII_ENUM_CONSTANT(xiiGALGraphicsAdapterVendor::ARM),
  XII_ENUM_CONSTANT(xiiGALGraphicsAdapterVendor::Qualcomm),
  XII_ENUM_CONSTANT(xiiGALGraphicsAdapterVendor::ImaginationTechnologies),
  XII_ENUM_CONSTANT(xiiGALGraphicsAdapterVendor::Microsoft),
  XII_ENUM_CONSTANT(xiiGALGraphicsAdapterVendor::Apple),
  XII_ENUM_CONSTANT(xiiGALGraphicsAdapterVendor::Mesa),
  XII_ENUM_CONSTANT(xiiGALGraphicsAdapterVendor::Broadcom),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALDeviceValidationLevel, 1)
  XII_ENUM_CONSTANT(xiiGALDeviceValidationLevel::Disabled),
  XII_ENUM_CONSTANT(xiiGALDeviceValidationLevel::Standard),
  XII_ENUM_CONSTANT(xiiGALDeviceValidationLevel::All),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALDeviceEventType, 1)
  XII_ENUM_CONSTANT(xiiGALDeviceEventType::Unknown),
  XII_ENUM_CONSTANT(xiiGALDeviceEventType::AfterInitialization),
  XII_ENUM_CONSTANT(xiiGALDeviceEventType::BeforeShutdown),
  XII_ENUM_CONSTANT(xiiGALDeviceEventType::BeforeBeginFrame),
  XII_ENUM_CONSTANT(xiiGALDeviceEventType::AfterBeginFrame),
  XII_ENUM_CONSTANT(xiiGALDeviceEventType::BeforeEndFrame),
  XII_ENUM_CONSTANT(xiiGALDeviceEventType::AfterEndFrame),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALValueType, 1)
  XII_ENUM_CONSTANT(xiiGALValueType::Undefined),
  XII_ENUM_CONSTANT(xiiGALValueType::Int8),
  XII_ENUM_CONSTANT(xiiGALValueType::Int16),
  XII_ENUM_CONSTANT(xiiGALValueType::Int32),
  XII_ENUM_CONSTANT(xiiGALValueType::UInt8),
  XII_ENUM_CONSTANT(xiiGALValueType::UInt16),
  XII_ENUM_CONSTANT(xiiGALValueType::UInt32),
  XII_ENUM_CONSTANT(xiiGALValueType::Float16),
  XII_ENUM_CONSTANT(xiiGALValueType::Float32),
  XII_ENUM_CONSTANT(xiiGALValueType::Float64),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALShaderStage, 1)
  XII_BITFLAGS_CONSTANT(xiiGALShaderStage::Unknown),
  XII_BITFLAGS_CONSTANT(xiiGALShaderStage::Vertex),
  XII_BITFLAGS_CONSTANT(xiiGALShaderStage::Pixel),
  XII_BITFLAGS_CONSTANT(xiiGALShaderStage::Geometry),
  XII_BITFLAGS_CONSTANT(xiiGALShaderStage::Hull),
  XII_BITFLAGS_CONSTANT(xiiGALShaderStage::Domain),
  XII_BITFLAGS_CONSTANT(xiiGALShaderStage::Compute),
  XII_BITFLAGS_CONSTANT(xiiGALShaderStage::Amplification),
  XII_BITFLAGS_CONSTANT(xiiGALShaderStage::Mesh),
  XII_BITFLAGS_CONSTANT(xiiGALShaderStage::RayGeneration),
  XII_BITFLAGS_CONSTANT(xiiGALShaderStage::RayMiss),
  XII_BITFLAGS_CONSTANT(xiiGALShaderStage::RayClosestHit),
  XII_BITFLAGS_CONSTANT(xiiGALShaderStage::RayAnyHit),
  XII_BITFLAGS_CONSTANT(xiiGALShaderStage::RayIntersection),
  XII_BITFLAGS_CONSTANT(xiiGALShaderStage::Callable),
  XII_BITFLAGS_CONSTANT(xiiGALShaderStage::Tile),
  XII_BITFLAGS_CONSTANT(xiiGALShaderStage::AllGraphics),
  XII_BITFLAGS_CONSTANT(xiiGALShaderStage::AllMesh),
  XII_BITFLAGS_CONSTANT(xiiGALShaderStage::AllRayTracing),
XII_END_STATIC_REFLECTED_BITFLAGS;

const char* xiiGALShaderStage::Names[ENUM_COUNT] = {
  "Vertex",
  "Pixel",
  "Geometry",
  "Hull",
  "Domain",
  "Compute",
  "Amplification",
  "Mesh",
  "RayGeneration",
  "RayMiss",
  "RayClosestHit",
  "RayAnyHit",
  "RayIntersection",
  "Callable",
  "Tile",
};

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALBindFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALBindFlags::None),
  XII_BITFLAGS_CONSTANT(xiiGALBindFlags::VertexBuffer),
  XII_BITFLAGS_CONSTANT(xiiGALBindFlags::IndexBuffer),
  XII_BITFLAGS_CONSTANT(xiiGALBindFlags::UniformBuffer),
  XII_BITFLAGS_CONSTANT(xiiGALBindFlags::ShaderResource),
  XII_BITFLAGS_CONSTANT(xiiGALBindFlags::StreamOutput),
  XII_BITFLAGS_CONSTANT(xiiGALBindFlags::RenderTarget),
  XII_BITFLAGS_CONSTANT(xiiGALBindFlags::DepthStencil),
  XII_BITFLAGS_CONSTANT(xiiGALBindFlags::UnorderedAccess),
  XII_BITFLAGS_CONSTANT(xiiGALBindFlags::IndirectDrawArguments),
  XII_BITFLAGS_CONSTANT(xiiGALBindFlags::InputAttachment),
  XII_BITFLAGS_CONSTANT(xiiGALBindFlags::RayTracing),
  XII_BITFLAGS_CONSTANT(xiiGALBindFlags::ShadingRate),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALResourceUsage, 1)
  XII_ENUM_CONSTANT(xiiGALResourceUsage::Immutable),
  XII_ENUM_CONSTANT(xiiGALResourceUsage::Dynamic),
  XII_ENUM_CONSTANT(xiiGALResourceUsage::Staging),
  XII_ENUM_CONSTANT(xiiGALResourceUsage::Unified),
  XII_ENUM_CONSTANT(xiiGALResourceUsage::Sparse),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALCPUAccessFlag, 1)
  XII_BITFLAGS_CONSTANT(xiiGALCPUAccessFlag::None),
  XII_BITFLAGS_CONSTANT(xiiGALCPUAccessFlag::Read),
  XII_BITFLAGS_CONSTANT(xiiGALCPUAccessFlag::Write),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALMapType, 1)
  XII_ENUM_CONSTANT(xiiGALMapType::Read),
  XII_ENUM_CONSTANT(xiiGALMapType::Write),
  XII_ENUM_CONSTANT(xiiGALMapType::ReadWrite),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALMapFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALMapFlags::None),
  XII_BITFLAGS_CONSTANT(xiiGALMapFlags::DoNotWait),
  XII_BITFLAGS_CONSTANT(xiiGALMapFlags::Discard),
  XII_BITFLAGS_CONSTANT(xiiGALMapFlags::NoOverWrite),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALResourceDimension, 1)
  XII_ENUM_CONSTANT(xiiGALResourceDimension::Undefined),
  XII_ENUM_CONSTANT(xiiGALResourceDimension::Buffer),
  XII_ENUM_CONSTANT(xiiGALResourceDimension::Texture1D),
  XII_ENUM_CONSTANT(xiiGALResourceDimension::Texture1DArray),
  XII_ENUM_CONSTANT(xiiGALResourceDimension::Texture2D),
  XII_ENUM_CONSTANT(xiiGALResourceDimension::Texture2DArray),
  XII_ENUM_CONSTANT(xiiGALResourceDimension::Texture3D),
  XII_ENUM_CONSTANT(xiiGALResourceDimension::TextureCube),
  XII_ENUM_CONSTANT(xiiGALResourceDimension::TextureCubeArray),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALTextureViewType, 1)
  XII_ENUM_CONSTANT(xiiGALTextureViewType::Undefined),
  XII_ENUM_CONSTANT(xiiGALTextureViewType::ShaderResource),
  XII_ENUM_CONSTANT(xiiGALTextureViewType::RenderTarget),
  XII_ENUM_CONSTANT(xiiGALTextureViewType::DepthStencil),
  XII_ENUM_CONSTANT(xiiGALTextureViewType::ReadOnlyDepthStencil),
  XII_ENUM_CONSTANT(xiiGALTextureViewType::UnorderedAccess),
  XII_ENUM_CONSTANT(xiiGALTextureViewType::ShadingRate),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALBufferViewType, 1)
  XII_ENUM_CONSTANT(xiiGALBufferViewType::Undefined),
  XII_ENUM_CONSTANT(xiiGALBufferViewType::ShaderResource),
  XII_ENUM_CONSTANT(xiiGALBufferViewType::UnorderedAccess),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALTextureFormat, 1)
  XII_ENUM_CONSTANT(xiiGALTextureFormat::Unknown),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGBA32Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGBA32Float),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGBA32UInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGBA32SInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGB32Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGB32Float),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGB32UInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGB32SInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGBA16Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGBA16Float),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGBA16UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGBA16UInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGBA16SNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGBA16SInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RG32Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RG32Float),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RG32UInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RG32SInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R32G8X24Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::D32FloatS8X24UInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R32FloatX8X24Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::X32TypelessG8X24UInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGB10A2Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGB10A2UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGB10A2UInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RG11B10Float),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGBA8Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGBA8UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGBA8UNormalizedSRGB),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGBA8UInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGBA8SNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGBA8SInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RG16Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RG16Float),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RG16UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RG16UInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RG16SNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RG16SInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R32Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::D32Float),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R32Float),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R32UInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R32SInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R24G8Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::D24UNormalizedS8UInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R24UNormalizedX8Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::X24TypelessG8UInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RG8Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RG8UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RG8UInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RG8SNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RG8SInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R16Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R16Float),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::D16UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R16UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R16UInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R16SNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R16SInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R8Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R8UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R8UInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R8SNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R8SInt),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::A8UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R1UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RGB9E5SharedExponent),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::RG8BG8UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::GR8GB8UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC1Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC1UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC1UNormalizedSRGB),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC2Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC2UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC2UNormalizedSRGB),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC3Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC3UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC3UNormalizedSRGB),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC4Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC4UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC4SNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC5Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC5UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC5SNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::B5G6R5UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::B5G5R5A1UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BGRA8UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BGRX8UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::R10G10B10XRBiasA2UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BGRA8Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BGRA8UNormalizedSRGB),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BGRX8Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BGRX8UNormalizedSRGB),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC6HTypeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC6HUF16),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC6HSF16),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC7Typeless),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC7UNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormat::BC7UNormalizedSRGB),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALFilterType, 1)
  XII_ENUM_CONSTANT(xiiGALFilterType::Unknown),
  XII_ENUM_CONSTANT(xiiGALFilterType::Point),
  XII_ENUM_CONSTANT(xiiGALFilterType::Linear),
  XII_ENUM_CONSTANT(xiiGALFilterType::Anisotropic),
  XII_ENUM_CONSTANT(xiiGALFilterType::ComparisonPoint),
  XII_ENUM_CONSTANT(xiiGALFilterType::ComparisonLinear),
  XII_ENUM_CONSTANT(xiiGALFilterType::ComparisonAnisotropic),
  XII_ENUM_CONSTANT(xiiGALFilterType::MinimumPoint),
  XII_ENUM_CONSTANT(xiiGALFilterType::MinimumLinear),
  XII_ENUM_CONSTANT(xiiGALFilterType::MinimumAnisotropic),
  XII_ENUM_CONSTANT(xiiGALFilterType::MaximumPoint),
  XII_ENUM_CONSTANT(xiiGALFilterType::MaximumLinear),
  XII_ENUM_CONSTANT(xiiGALFilterType::MaximumAnisotropic),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALTextureAddressMode, 1)
  XII_ENUM_CONSTANT(xiiGALTextureAddressMode::Unknown),
  XII_ENUM_CONSTANT(xiiGALTextureAddressMode::Wrap),
  XII_ENUM_CONSTANT(xiiGALTextureAddressMode::Mirror),
  XII_ENUM_CONSTANT(xiiGALTextureAddressMode::Clamp),
  XII_ENUM_CONSTANT(xiiGALTextureAddressMode::Border),
  XII_ENUM_CONSTANT(xiiGALTextureAddressMode::MirrorOnce),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALComparisonFunction, 1)
  XII_ENUM_CONSTANT(xiiGALComparisonFunction::Unknown),
  XII_ENUM_CONSTANT(xiiGALComparisonFunction::Never),
  XII_ENUM_CONSTANT(xiiGALComparisonFunction::Less),
  XII_ENUM_CONSTANT(xiiGALComparisonFunction::Equal),
  XII_ENUM_CONSTANT(xiiGALComparisonFunction::LessEqual),
  XII_ENUM_CONSTANT(xiiGALComparisonFunction::Greater),
  XII_ENUM_CONSTANT(xiiGALComparisonFunction::NotEqual),
  XII_ENUM_CONSTANT(xiiGALComparisonFunction::GreaterEqual),
  XII_ENUM_CONSTANT(xiiGALComparisonFunction::Always),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALPrimitiveTopology, 1)
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::Undefined),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::TriangleList),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::TriangleStrip),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::PointList),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::LineList),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::LineStrip),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::TriangleListAdjacent),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::TriangleStripAdjacent),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::LineListAdjacent),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::LineStripAdjacent),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList1),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList2),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList3),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList4),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList5),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList6),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList7),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList8),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList9),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList10),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList11),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList12),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList13),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList14),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList15),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList16),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList17),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList18),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList19),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList20),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList21),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList22),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList23),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList24),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList25),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList26),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList27),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList28),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList29),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList30),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList31),
  XII_ENUM_CONSTANT(xiiGALPrimitiveTopology::ControlPointPatchList32),
XII_END_STATIC_REFLECTED_ENUM;

const char* xiiGALPrimitiveTopology::Names[ENUM_COUNT] = {
  "TriangleList",
  "TriangleStrip",
  "PointList",
  "LineList",
  "LineStrip",
  "TriangleListAdjacent",
  "TriangleStripAdjacent",
  "LineListAdjacent",
  "LineStripAdjacent",
  "ControlPointPatchList1",
  "ControlPointPatchList2",
  "ControlPointPatchList3",
  "ControlPointPatchList4",
  "ControlPointPatchList5",
  "ControlPointPatchList6",
  "ControlPointPatchList7",
  "ControlPointPatchList8",
  "ControlPointPatchList9",
  "ControlPointPatchList10",
  "ControlPointPatchList11",
  "ControlPointPatchList12",
  "ControlPointPatchList13",
  "ControlPointPatchList14",
  "ControlPointPatchList15",
  "ControlPointPatchList16",
  "ControlPointPatchList17",
  "ControlPointPatchList18",
  "ControlPointPatchList19",
  "ControlPointPatchList20",
  "ControlPointPatchList21",
  "ControlPointPatchList22",
  "ControlPointPatchList23",
  "ControlPointPatchList24",
  "ControlPointPatchList25",
  "ControlPointPatchList26",
  "ControlPointPatchList27",
  "ControlPointPatchList28",
  "ControlPointPatchList29",
  "ControlPointPatchList30",
  "ControlPointPatchList31",
  "ControlPointPatchList32",
};

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALMemoryProperties, 1)
  XII_BITFLAGS_CONSTANT(xiiGALMemoryProperties::Unknown),
  XII_BITFLAGS_CONSTANT(xiiGALMemoryProperties::HostCoherent),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALDeviceAdapterType, 1)
  XII_ENUM_CONSTANT(xiiGALDeviceAdapterType::Unknown),
  XII_ENUM_CONSTANT(xiiGALDeviceAdapterType::Software),
  XII_ENUM_CONSTANT(xiiGALDeviceAdapterType::Integrated),
  XII_ENUM_CONSTANT(xiiGALDeviceAdapterType::Discrete),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALScalingMode, 1)
  XII_ENUM_CONSTANT(xiiGALScalingMode::Unspecified),
  XII_ENUM_CONSTANT(xiiGALScalingMode::Centered),
  XII_ENUM_CONSTANT(xiiGALScalingMode::Stretched),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALScanLineOrder, 1)
  XII_ENUM_CONSTANT(xiiGALScanLineOrder::Unspecified),
  XII_ENUM_CONSTANT(xiiGALScanLineOrder::Progressive),
  XII_ENUM_CONSTANT(xiiGALScanLineOrder::UpperFieldFirst),
  XII_ENUM_CONSTANT(xiiGALScanLineOrder::LowerFieldFirst),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALSwapChainUsageFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALSwapChainUsageFlags::None),
  XII_BITFLAGS_CONSTANT(xiiGALSwapChainUsageFlags::RenderTarget),
  XII_BITFLAGS_CONSTANT(xiiGALSwapChainUsageFlags::ShaderResource),
  XII_BITFLAGS_CONSTANT(xiiGALSwapChainUsageFlags::InputAttachment),
  XII_BITFLAGS_CONSTANT(xiiGALSwapChainUsageFlags::CopySource),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALSurfaceTransform, 1)
  XII_ENUM_CONSTANT(xiiGALSurfaceTransform::Optimal),
  XII_ENUM_CONSTANT(xiiGALSurfaceTransform::Identity),
  XII_ENUM_CONSTANT(xiiGALSurfaceTransform::Rotate90),
  XII_ENUM_CONSTANT(xiiGALSurfaceTransform::Rotate180),
  XII_ENUM_CONSTANT(xiiGALSurfaceTransform::Rotate270),
  XII_ENUM_CONSTANT(xiiGALSurfaceTransform::HorizontalMirror),
  XII_ENUM_CONSTANT(xiiGALSurfaceTransform::HorizontalMirrorRotate90),
  XII_ENUM_CONSTANT(xiiGALSurfaceTransform::HorizontalMirrorRotate180),
  XII_ENUM_CONSTANT(xiiGALSurfaceTransform::HorizontalMirrorRotate270),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALQueryType, 1)
  XII_ENUM_CONSTANT(xiiGALQueryType::Undefined),
  XII_ENUM_CONSTANT(xiiGALQueryType::Occlusion),
  XII_ENUM_CONSTANT(xiiGALQueryType::BinaryOcclusion),
  XII_ENUM_CONSTANT(xiiGALQueryType::Timestamp),
  XII_ENUM_CONSTANT(xiiGALQueryType::PipelineStatistics),
  XII_ENUM_CONSTANT(xiiGALQueryType::Duration),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALWaveFeature, 1)
  XII_BITFLAGS_CONSTANT(xiiGALWaveFeature::Unknown),
  XII_BITFLAGS_CONSTANT(xiiGALWaveFeature::Basic),
  XII_BITFLAGS_CONSTANT(xiiGALWaveFeature::Vote),
  XII_BITFLAGS_CONSTANT(xiiGALWaveFeature::Arithmetic),
  XII_BITFLAGS_CONSTANT(xiiGALWaveFeature::BallOut),
  XII_BITFLAGS_CONSTANT(xiiGALWaveFeature::Shuffle),
  XII_BITFLAGS_CONSTANT(xiiGALWaveFeature::ShuffleRelative),
  XII_BITFLAGS_CONSTANT(xiiGALWaveFeature::Clustered),
  XII_BITFLAGS_CONSTANT(xiiGALWaveFeature::Quad),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALRayTracingCapabilityFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALRayTracingCapabilityFlags::None),
  XII_BITFLAGS_CONSTANT(xiiGALRayTracingCapabilityFlags::StandaloneShaders),
  XII_BITFLAGS_CONSTANT(xiiGALRayTracingCapabilityFlags::InlineRayTracing),
  XII_BITFLAGS_CONSTANT(xiiGALRayTracingCapabilityFlags::IndirectRayTracing),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALValidationFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALValidationFlags::None),
  XII_BITFLAGS_CONSTANT(xiiGALValidationFlags::CheckShaderBufferSize),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALCommandQueueType, 1)
  XII_BITFLAGS_CONSTANT(xiiGALCommandQueueType::Unknown),
  XII_BITFLAGS_CONSTANT(xiiGALCommandQueueType::Transfer),
  XII_BITFLAGS_CONSTANT(xiiGALCommandQueueType::Compute),
  XII_BITFLAGS_CONSTANT(xiiGALCommandQueueType::Graphics),
  XII_BITFLAGS_CONSTANT(xiiGALCommandQueueType::SparseBinding),
  XII_BITFLAGS_CONSTANT(xiiGALCommandQueueType::PrimaryType),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALCommandQueuePriority, 1)
  XII_ENUM_CONSTANT(xiiGALCommandQueuePriority::Unknown),
  XII_ENUM_CONSTANT(xiiGALCommandQueuePriority::Low),
  XII_ENUM_CONSTANT(xiiGALCommandQueuePriority::Medium),
  XII_ENUM_CONSTANT(xiiGALCommandQueuePriority::High),
  XII_ENUM_CONSTANT(xiiGALCommandQueuePriority::RealTime),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALShadingRateCombiner, 1)
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCombiner::PassThrough),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCombiner::CombinerOverride),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCombiner::CombinerMin),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCombiner::CombinerMax),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCombiner::CombinerSum),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCombiner::CombinerMul),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALShadingRateFormat, 1)
  XII_ENUM_CONSTANT(xiiGALShadingRateFormat::Unknown),
  XII_ENUM_CONSTANT(xiiGALShadingRateFormat::Palette),
  XII_ENUM_CONSTANT(xiiGALShadingRateFormat::RG8UNormalized),
  XII_ENUM_CONSTANT(xiiGALShadingRateFormat::ColumnRowFloat32),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALShadingRateAxis, 1)
  XII_ENUM_CONSTANT(xiiGALShadingRateAxis::X1),
  XII_ENUM_CONSTANT(xiiGALShadingRateAxis::X2),
  XII_ENUM_CONSTANT(xiiGALShadingRateAxis::X4),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALShadingRate, 1)
  XII_BITFLAGS_CONSTANT(xiiGALShadingRate::_1X1),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRate::_1X2),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRate::_1X4),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRate::_2X1),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRate::_2X2),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRate::_2X4),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRate::_4X1),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRate::_4X2),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRate::_4X4),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALSampleCount, 1)
  XII_BITFLAGS_CONSTANT(xiiGALSampleCount::None),
  XII_BITFLAGS_CONSTANT(xiiGALSampleCount::OneSample),
  XII_BITFLAGS_CONSTANT(xiiGALSampleCount::TwoSamples),
  XII_BITFLAGS_CONSTANT(xiiGALSampleCount::FourSamples),
  XII_BITFLAGS_CONSTANT(xiiGALSampleCount::EightSamples),
  XII_BITFLAGS_CONSTANT(xiiGALSampleCount::SixteenSamples),
  XII_BITFLAGS_CONSTANT(xiiGALSampleCount::ThirtyTwoSamples),
  XII_BITFLAGS_CONSTANT(xiiGALSampleCount::SixtyFourSamples),
  XII_BITFLAGS_CONSTANT(xiiGALSampleCount::AllSamples),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALMSAASampleCount, 1)
  XII_ENUM_CONSTANT(xiiGALMSAASampleCount::OneSample),
  XII_ENUM_CONSTANT(xiiGALMSAASampleCount::TwoSamples),
  XII_ENUM_CONSTANT(xiiGALMSAASampleCount::FourSamples),
  XII_ENUM_CONSTANT(xiiGALMSAASampleCount::EightSamples),
  XII_ENUM_CONSTANT(xiiGALMSAASampleCount::SixteenSamples),
  XII_ENUM_CONSTANT(xiiGALMSAASampleCount::ThirtyTwoSamples),
  XII_ENUM_CONSTANT(xiiGALMSAASampleCount::SixtyFourSamples),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALShadingRateCapabilityFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCapabilityFlags::None),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCapabilityFlags::PerDraw),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCapabilityFlags::PerPrimitive),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCapabilityFlags::TextureBased),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCapabilityFlags::SampleMask),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCapabilityFlags::ShaderSampleMask),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCapabilityFlags::ShaderDepthStencilWrite),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCapabilityFlags::PerPrimitiveWithMultipleViewports),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCapabilityFlags::SameTextureForWholeRenderPass),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCapabilityFlags::TextureArray),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCapabilityFlags::ShadingRateShaderInput),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCapabilityFlags::AdditionalInvocations),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCapabilityFlags::NonSubSampledRenderTarget),
  XII_BITFLAGS_CONSTANT(xiiGALShadingRateCapabilityFlags::SubSampledRenderTarget),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALShadingRateTextureAccess, 1)
  XII_ENUM_CONSTANT(xiiGALShadingRateTextureAccess::Unknown),
  XII_ENUM_CONSTANT(xiiGALShadingRateTextureAccess::OnGPU),
  XII_ENUM_CONSTANT(xiiGALShadingRateTextureAccess::OnSubmit),
  XII_ENUM_CONSTANT(xiiGALShadingRateTextureAccess::OnSetRenderTarget),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALDrawCommandCapabilityFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALDrawCommandCapabilityFlags::None),
  XII_BITFLAGS_CONSTANT(xiiGALDrawCommandCapabilityFlags::BaseVertex),
  XII_BITFLAGS_CONSTANT(xiiGALDrawCommandCapabilityFlags::DrawIndirect),
  XII_BITFLAGS_CONSTANT(xiiGALDrawCommandCapabilityFlags::DrawIndirectFirstInstance),
  XII_BITFLAGS_CONSTANT(xiiGALDrawCommandCapabilityFlags::NativeMultiDrawIndirect),
  XII_BITFLAGS_CONSTANT(xiiGALDrawCommandCapabilityFlags::DrawIndirectCounterBuffer),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALSparseResourceCapabilityFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALSparseResourceCapabilityFlags::None),
  XII_BITFLAGS_CONSTANT(xiiGALSparseResourceCapabilityFlags::ShaderResourceResidency),
  XII_BITFLAGS_CONSTANT(xiiGALSparseResourceCapabilityFlags::Buffer),
  XII_BITFLAGS_CONSTANT(xiiGALSparseResourceCapabilityFlags::Texture2D),
  XII_BITFLAGS_CONSTANT(xiiGALSparseResourceCapabilityFlags::Texture3D),
  XII_BITFLAGS_CONSTANT(xiiGALSparseResourceCapabilityFlags::Texture2Samples),
  XII_BITFLAGS_CONSTANT(xiiGALSparseResourceCapabilityFlags::Texture4Samples),
  XII_BITFLAGS_CONSTANT(xiiGALSparseResourceCapabilityFlags::Texture8Samples),
  XII_BITFLAGS_CONSTANT(xiiGALSparseResourceCapabilityFlags::Texture16Samples),
  XII_BITFLAGS_CONSTANT(xiiGALSparseResourceCapabilityFlags::Aliased),
  XII_BITFLAGS_CONSTANT(xiiGALSparseResourceCapabilityFlags::Standard2DTileShape),
  XII_BITFLAGS_CONSTANT(xiiGALSparseResourceCapabilityFlags::Standard2DMSTileShape),
  XII_BITFLAGS_CONSTANT(xiiGALSparseResourceCapabilityFlags::Standard3DTileShape),
  XII_BITFLAGS_CONSTANT(xiiGALSparseResourceCapabilityFlags::AlignedMipSize),
  XII_BITFLAGS_CONSTANT(xiiGALSparseResourceCapabilityFlags::NonResidentStrict),
  XII_BITFLAGS_CONSTANT(xiiGALSparseResourceCapabilityFlags::Texture2DArrayMipTail),
  XII_BITFLAGS_CONSTANT(xiiGALSparseResourceCapabilityFlags::BufferStandardBlock),
  XII_BITFLAGS_CONSTANT(xiiGALSparseResourceCapabilityFlags::NonResidentSafe),
  XII_BITFLAGS_CONSTANT(xiiGALSparseResourceCapabilityFlags::MixedResourceTypeSupport),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_ENUM(xiiGALTextureFormatComponentType, 1)
  XII_ENUM_CONSTANT(xiiGALTextureFormatComponentType::Undefined),
  XII_ENUM_CONSTANT(xiiGALTextureFormatComponentType::Float),
  XII_ENUM_CONSTANT(xiiGALTextureFormatComponentType::SignedNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormatComponentType::UnsignedNormalized),
  XII_ENUM_CONSTANT(xiiGALTextureFormatComponentType::UnsignedNormalizedSRGB),
  XII_ENUM_CONSTANT(xiiGALTextureFormatComponentType::SignedInteger),
  XII_ENUM_CONSTANT(xiiGALTextureFormatComponentType::UnsignedInteger),
  XII_ENUM_CONSTANT(xiiGALTextureFormatComponentType::Depth),
  XII_ENUM_CONSTANT(xiiGALTextureFormatComponentType::DepthStencil),
  XII_ENUM_CONSTANT(xiiGALTextureFormatComponentType::Compound),
  XII_ENUM_CONSTANT(xiiGALTextureFormatComponentType::Compressed),
XII_END_STATIC_REFLECTED_ENUM;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALResourceDimensionCapabilityFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALResourceDimensionCapabilityFlags::None),
  XII_BITFLAGS_CONSTANT(xiiGALResourceDimensionCapabilityFlags::Buffer),
  XII_BITFLAGS_CONSTANT(xiiGALResourceDimensionCapabilityFlags::Texture1D),
  XII_BITFLAGS_CONSTANT(xiiGALResourceDimensionCapabilityFlags::Texture1DArray),
  XII_BITFLAGS_CONSTANT(xiiGALResourceDimensionCapabilityFlags::Texture2D),
  XII_BITFLAGS_CONSTANT(xiiGALResourceDimensionCapabilityFlags::Texture2DArray),
  XII_BITFLAGS_CONSTANT(xiiGALResourceDimensionCapabilityFlags::Texture3D),
  XII_BITFLAGS_CONSTANT(xiiGALResourceDimensionCapabilityFlags::TextureCube),
  XII_BITFLAGS_CONSTANT(xiiGALResourceDimensionCapabilityFlags::TextureCubeArray),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALSparseTextureFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALSparseTextureFlags::None),
  XII_BITFLAGS_CONSTANT(xiiGALSparseTextureFlags::SingleMipTail),
  XII_BITFLAGS_CONSTANT(xiiGALSparseTextureFlags::AlignedMipSize),
  XII_BITFLAGS_CONSTANT(xiiGALSparseTextureFlags::NonStandardBlockSize),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALPipelineStageFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::Undefined),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::TopOfPipeline),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::DrawIndirect),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::VertexInput),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::VertexShader),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::HullShader),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::DomainShader),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::GeometryShader),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::PixelShader),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::EarlyFragmentTests),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::LateFragmentTests),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::RenderTarget),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::ComputeShader),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::Transfer),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::BottomOfPipeline),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::Host),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::ConditionalRendering),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::ShadingRateTexture),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::RayTracingShader),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::AccelerationStructureBuild),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::TaskShader),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::MeshShader),
  XII_BITFLAGS_CONSTANT(xiiGALPipelineStageFlags::FragmentDensityProcess),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALAccessFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::None),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::IndirectCommandRead),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::IndexRead),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::VertexRead),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::UniformRead),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::InputAttachmentRead),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::ShaderRead),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::ShaderWrite),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::RenderTargetRead),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::RenderTargetWrite),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::DepthStencilRead),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::DepthStencilWrite),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::CopySource),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::CopyDestination),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::HostRead),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::HostWrite),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::MemoryRead),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::MemoryWrite),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::ConditionalRenderingRead),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::ShadingRateTextureRead),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::AccelerationStructureRead),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::AccelerationStructureWrite),
  XII_BITFLAGS_CONSTANT(xiiGALAccessFlags::FragmentDensityMapRead),
XII_END_STATIC_REFLECTED_BITFLAGS;

XII_BEGIN_STATIC_REFLECTED_BITFLAGS(xiiGALResourceStateFlags, 1)
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::Unknown),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::Undefined),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::VertexBuffer),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::ConstantBuffer),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::IndexBuffer),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::RenderTarget),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::UnorderedAccess),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::DepthWrite),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::DepthRead),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::ShaderResource),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::StreamOut),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::IndirectArgument),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::CopyDestination),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::CopySource),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::ResolveDestination),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::ResolveSource),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::InputAttachment),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::Present),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::BuildAsRead),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::BuildAsWrite),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::RayTracing),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::Common),
  XII_BITFLAGS_CONSTANT(xiiGALResourceStateFlags::ShadingRate),
XII_END_STATIC_REFLECTED_BITFLAGS;

// clang-format on
