
# -----------------------------------------------------------------------------
# Find DirectX12 SDK
# Defines:
#   DirectX12_FOUND
#   DirectX12_INCLUDE_DIR
#   DirectX12_LIBRARY
#   DirectX12_ROOT_DIR

xii_pull_platform_vars()

if(XII_CMAKE_PLATFORM_WINDOWS)
  include(FindPkgMacros)
  findpkg_begin(DirectX12)

  # Get path, convert backslashes as ${ENV_DXSDK_DIR}
  getenv_path(DXSDK_DIR)
  getenv_path(DIRECTX_HOME)
  getenv_path(DIRECTX_ROOT)
  getenv_path(DIRECTX_BASE)

  # construct search paths
  set(DirectX12_PREFIX_PATH
    "${DXSDK_DIR}" "${ENV_DXSDK_DIR}"
    "${DIRECTX_HOME}" "${ENV_DIRECTX_HOME}"
    "${DIRECTX_ROOT}" "${ENV_DIRECTX_ROOT}"
    "${DIRECTX_BASE}" "${ENV_DIRECTX_BASE}"
    "C:/apps_x86/Microsoft DirectX SDK*"
    "C:/Program Files (x86)/Microsoft DirectX SDK*"
    "C:/apps/Microsoft DirectX SDK*"
    "C:/Program Files/Microsoft DirectX SDK*"
    "$ENV{ProgramFiles}/Microsoft DirectX SDK*"
  )

  set(DirectX12_INC_SEARCH_PATH
    "C:/Program Files (x86)/Windows Kits/10/Include/${XII_CMAKE_WINDOWS_SDK_VERSION}/shared"
    "C:/Program Files (x86)/Windows Kits/10/Include/${XII_CMAKE_WINDOWS_SDK_VERSION}/um"
  )
  set(DirectX12_LIB_SEARCH_PATH
    "C:/Program Files (x86)/Windows Kits/10/Lib/${XII_CMAKE_WINDOWS_SDK_VERSION}/um"
  )

  # Generate candidate paths from DirectX12_PREFIX_PATH
  create_search_paths(DirectX12)

  # redo search if prefix path changed
  clear_if_changed(DirectX12_PREFIX_PATH
    DirectX12_LIBRARY
    DirectX12_INCLUDE_DIR
  )

  # dlls are in DirectX12_ROOT_DIR/Developer Runtime/x64|x86
  # lib files are in DirectX12_ROOT_DIR/Lib/x64|x86
  if(XII_CMAKE_ARCHITECTURE_ARM)
    if(XII_CMAKE_ARCHITECTURE_64BIT)
      set(DirectX12_LIBPATH_SUFFIX "arm64")
    else()
      set(DirectX12_LIBPATH_SUFFIX "arm")
    endif()
  else()
    if(XII_CMAKE_ARCHITECTURE_64BIT)
      set(DirectX12_LIBPATH_SUFFIX "x64")
    else()
      set(DirectX12_LIBPATH_SUFFIX "x86")
    endif()
  endif()

  # look for D3D12 components
  find_path(DirectX12_INCLUDE_DIR NAMES D3D12Shader.h HINTS ${DirectX12_INC_SEARCH_PATH})
  find_library(DirectX12_DXERR_LIBRARY NAMES DxErr HINTS ${DirectX12_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX12_LIBPATH_SUFFIX})
  find_library(DirectX12_DXGUID_LIBRARY NAMES dxguid HINTS ${DirectX12_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX12_LIBPATH_SUFFIX})
  find_library(DirectX12_DXGI_LIBRARY NAMES dxgi HINTS ${DirectX12_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX12_LIBPATH_SUFFIX})
  find_library(DirectX12_D3DCOMPILER_LIBRARY NAMES d3dcompiler HINTS ${DirectX12_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX12_LIBPATH_SUFFIX})

  find_library(DirectX12_LIBRARY NAMES d3d12 HINTS ${DirectX12_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX12_LIBPATH_SUFFIX})
  find_library(DirectX12_D3DX12_LIBRARY NAMES d3dx12 HINTS ${DirectX12_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX12_LIBPATH_SUFFIX})

  if(DirectX12_INCLUDE_DIR AND DirectX12_LIBRARY)
    set(DirectX12_D3D12_FOUND TRUE)
    set(DirectX12_INCLUDE_DIR ${DirectX12_INCLUDE_DIR})

    set(DirectX12_D3D12_LIBRARIES ${DirectX12_D3D12_LIBRARIES}
      ${DirectX12_LIBRARY}
      ${DirectX12_DXGI_LIBRARY}
      ${DirectX12_DXGUID_LIBRARY}
      ${DirectX12_D3DCOMPILER_LIBRARY}
    )
  endif()

  if(DirectX12_D3DX12_LIBRARY)
    set(DirectX12_D3D12_LIBRARIES ${DirectX12_D3D12_LIBRARIES} ${DirectX12_D3DX12_LIBRARY})
  endif()

  if(DirectX12_DXERR_LIBRARY)
    set(DirectX12_D3D12_LIBRARIES ${DirectX12_D3D12_LIBRARIES} ${DirectX12_DXERR_LIBRARY})
  endif()

  findpkg_finish(DirectX12)

  set(DirectX12_LIBRARIES
    ${DirectX12_D3D12_LIBRARIES}
  )

  mark_as_advanced(DirectX12_INCLUDE_DIR
    DirectX12_D3D12_LIBRARIES
    DirectX12_D3DX12_LIBRARY
    DirectX12_DXERR_LIBRARY
    DirectX12_DXGUID_LIBRARY
    DirectX12_DXGI_LIBRARY
    DirectX12_D3DCOMPILER_LIBRARY)
endif(XII_CMAKE_PLATFORM_WINDOWS)
