#pragma once

/// \brief The maximum number of bound constant buffers.
#define XII_GAL_MAX_CONSTANT_BUFFER_COUNT 16

/// \brief The maximum number of bound vertex buffers.
#define XII_GAL_MAX_VERTEX_BUFFER_COUNT 16

/// \brief The maximum number of bound samplers.
#define XII_GAL_MAX_SAMPLER_COUNT 16

/// \brief The maximum number of bound render targets.
#define XII_GAL_MAX_RENDERTARGET_COUNT 8

/// \brief The maximum number of bound viewports.
#define XII_GAL_MAX_VIEWPORT_COUNT 16

/// \brief The maximum number of queues in graphics adapter description.
#define XII_GAL_MAX_ADAPTER_QUEUE_COUNT 16

/// \brief Special constant for the default adapter index.
#define XII_GAL_DEFAULT_ADAPTER_ID 0xFFFFFFFFU

/// \brief Special constant for the default queue index.
#define XII_GAL_DEFAULT_QUEUE_ID 0xFFU

/// \brief The maximum number of shading rate modes.
#define XII_GAL_MAX_SHADING_RATE 9

/// \brief The bit shift for the shading X-Axis rate.
#define XII_GAL_SHADING_RATE_X_SHIFT 2

/// \brief The maximum number of resource signatures that one pipeline can use.
#define XII_GAL_MAX_RESOURCE_SIGNATURES_COUNT 8
