# Find the folder into which the Vulkan SDK has been installed

# Early out, if this target has been created before
if((TARGET XIIVulkan::Loader) AND(TARGET XIIVulkan::DXC))
  return()
endif()

set(XII_VULKAN_DIR $ENV{VULKAN_SDK} CACHE PATH "Directory of the Vulkan SDK")

xii_pull_compiler_and_architecture_vars()
xii_pull_config_vars()

get_property(XII_SUBMODULE_PREFIX_PATH GLOBAL PROPERTY XII_SUBMODULE_PREFIX_PATH)

if(XII_CMAKE_PLATFORM_WINDOWS_DESKTOP AND XII_CMAKE_ARCHITECTURE_64BIT)
  if((XII_VULKAN_DIR STREQUAL "XII_VULKAN_DIR-NOTFOUND") OR (XII_VULKAN_DIR STREQUAL ""))
    # set(CMAKE_FIND_DEBUG_MODE TRUE)
    unset(XII_VULKAN_DIR CACHE)
    unset(XIIVulkan_DIR CACHE)
    find_path(XII_VULKAN_DIR Config/vk_layer_settings.txt
      PATHS
      ${XII_VULKAN_DIR}
      $ENV{VULKAN_SDK}
      REQUIRED
    )

    # set(CMAKE_FIND_DEBUG_MODE FALSE)
  endif()
elseif(XII_CMAKE_PLATFORM_LINUX AND XII_CMAKE_ARCHITECTURE_64BIT)
  if((XII_VULKAN_DIR STREQUAL "XII_VULKAN_DIR-NOTFOUND") OR (XII_VULKAN_DIR STREQUAL ""))
    # set(CMAKE_FIND_DEBUG_MODE TRUE)
    unset(XII_VULKAN_DIR CACHE)
    unset(XIIVulkan_DIR CACHE)
    find_path(XII_VULKAN_DIR config/vk_layer_settings.txt
      PATHS
      ${XII_VULKAN_DIR}
      $ENV{VULKAN_SDK}
    )

    if(XII_CMAKE_ARCHITECTURE_X86)
      if((XII_VULKAN_DIR STREQUAL "XII_VULKAN_DIR-NOTFOUND") OR (XII_VULKAN_DIR STREQUAL ""))
        xii_download_and_extract("${XII_CONFIG_VULKAN_SDK_LINUXX64_URL}" "${CMAKE_BINARY_DIR}/vulkan-sdk" "vulkan-sdk-${XII_CONFIG_VULKAN_SDK_LINUXX64_VERSION}")
        set(XII_VULKAN_DIR "${CMAKE_BINARY_DIR}/vulkan-sdk/${XII_CONFIG_VULKAN_SDK_LINUXX64_VERSION}" CACHE PATH "Directory of the Vulkan SDK" FORCE)

        find_path(XII_VULKAN_DIR config/vk_layer_settings.txt
          PATHS
          ${XII_VULKAN_DIR}
          $ENV{VULKAN_SDK}
        )
      endif()
    endif()

    if((XII_VULKAN_DIR STREQUAL "XII_VULKAN_DIR-NOTFOUND") OR (XII_VULKAN_DIR STREQUAL ""))
      message(FATAL_ERROR "Failed to find vulkan SDK. XII requires the vulkan sdk ${XII_CONFIG_VULKAN_SDK_LINUXX64_VERSION}. Please set the environment variable VULKAN_SDK to the vulkan sdk location.")
    endif()

    # set(CMAKE_FIND_DEBUG_MODE FALSE)
  endif()
elseif(XII_CMAKE_PLATFORM_ANDROID AND XII_CMAKE_ARCHITECTURE_64BIT)
  if((XII_VULKAN_DIR STREQUAL "XII_VULKAN_DIR-NOTFOUND") OR (XII_VULKAN_DIR STREQUAL ""))
    xii_android_verify_ndk()

    unset(XII_VULKAN_DIR CACHE)
    unset(XIIVulkan_DIR CACHE)
    set(XII_VULKAN_DIR "${ANDROID_NDK}/sources/third_party/vulkan/src/include")
  endif()
else()
  message(FATAL_ERROR "TODO: Vulkan is not yet supported on this platform and/or architecture.")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(XIIVulkan DEFAULT_MSG XII_VULKAN_DIR)

if(XIIVULKAN_FOUND)
  if(XII_CMAKE_PLATFORM_WINDOWS_DESKTOP AND XII_CMAKE_ARCHITECTURE_64BIT)
    add_library(XIIVulkan::Loader STATIC IMPORTED)
    set_target_properties(XIIVulkan::Loader PROPERTIES IMPORTED_LOCATION "${XII_VULKAN_DIR}/Lib/vulkan-1.lib")
    set_target_properties(XIIVulkan::Loader PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${XII_VULKAN_DIR}/Include")

    add_library(XIIVulkan::DXC SHARED IMPORTED)
    set_target_properties(XIIVulkan::DXC PROPERTIES IMPORTED_LOCATION "${XII_VULKAN_DIR}/Bin/dxcompiler.dll")
    set_target_properties(XIIVulkan::DXC PROPERTIES IMPORTED_IMPLIB "${XII_VULKAN_DIR}/Lib/dxcompiler.lib")
    set_target_properties(XIIVulkan::DXC PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${XII_VULKAN_DIR}/Include")

  elseif(XII_CMAKE_PLATFORM_LINUX AND XII_CMAKE_ARCHITECTURE_64BIT)
    add_library(XIIVulkan::Loader SHARED IMPORTED)
    set_target_properties(XIIVulkan::Loader PROPERTIES IMPORTED_LOCATION "${XII_VULKAN_DIR}/x86_64/lib/libvulkan.so")
    set_target_properties(XIIVulkan::Loader PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${XII_VULKAN_DIR}/x86_64/include")

    add_library(XIIVulkan::DXC SHARED IMPORTED)
    set_target_properties(XIIVulkan::DXC PROPERTIES IMPORTED_LOCATION "${XII_VULKAN_DIR}/x86_64/lib/libdxcompiler.so")
    set_target_properties(XIIVulkan::DXC PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${XII_VULKAN_DIR}/x86_64/include")
  else()
    message(FATAL_ERROR "TODO: Vulkan is not yet supported on this platform and/or architecture.")
  endif()
endif()
