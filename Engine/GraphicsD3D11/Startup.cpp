#include <GraphicsD3D11/GraphicsD3D11PCH.h>

#include <Foundation/Configuration/Startup.h>

// clang-format off
XII_BEGIN_SUBSYSTEM_DECLARATION(xiiGAL, GraphicsD3D11)

  BEGIN_SUBSYSTEM_DEPENDENCIES
    "Foundation",
    "Core"
  END_SUBSYSTEM_DEPENDENCIES

  ON_CORESYSTEMS_STARTUP
  {
  }

  ON_CORESYSTEMS_SHUTDOWN
  {
  }

XII_END_SUBSYSTEM_DECLARATION;
// clang-format on

XII_STATICLINK_FILE(GraphicsD3D11, GraphicsD3D11_Startup);
