#pragma once

#include <GraphicsFoundation/GraphicsFoundationDLL.h>

#include <Foundation/Profiling/Profiling.h>
#include <GraphicsFoundation/Declarations/GraphicsTypes.h>

#if 0
struct GPUTimingScope;

/// Sets profiling marker and GPU timings for the current scope.
class XII_GRAPHICSFOUNDATION_DLL xiiProfilingScopeAndMarker : public xiiProfilingScope
{
public:
  xiiProfilingScopeAndMarker(xiiGALCommandList* pCommandList, xiiStringView sName);

  ~xiiProfilingScopeAndMarker();

public:
  static GPUTimingScope* Start(xiiGALCommandList* pCommandList, xiiStringView sName);
  static void            Stop(xiiGALCommandList* pCommandList, GPUTimingScope*& ref_pTimingScope);

protected:
  xiiGALCommandList* m_pCommandList = nullptr;
  GPUTimingScope*    m_pTimingScope = nullptr;
};

#  if XII_ENABLED(XII_USE_PROFILING) || defined(XII_DOCS)

/// \brief Profiles the current scope using the given name and also inserts a marker with the given GALContext.
#    define XII_PROFILE_AND_MARKER(GALContext, szName) xiiProfilingScopeAndMarker XII_CONCAT(_xiiProfilingScope, XII_SOURCE_LINE)(GALContext, szName)

#  else

#    define XII_PROFILE_AND_MARKER(GALContext, szName) /*empty*/

#  endif
#else
#  define XII_PROFILE_AND_MARKER(GALContext, szName)
#endif

#include <GraphicsFoundation/Profiling/Implementation/Profiling_inl.h>
