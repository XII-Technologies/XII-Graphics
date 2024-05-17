# #####################################
# ## Vulkan support
# #####################################

set(XII_BUILD_VULKAN OFF CACHE BOOL "Build the Vulkan Graphics Device.")

# #####################################
# ## xii_requires_vulkan()
# #####################################

macro(xii_requires_vulkan)
  xii_requires_one_of(XII_CMAKE_PLATFORM_LINUX XII_CMAKE_PLATFORM_WINDOWS)
  xii_requires(XII_BUILD_VULKAN)
  find_package(XIIVulkan REQUIRED)
endmacro()

# #####################################
# ## xii_link_target_vulkan(<target>)
# #####################################
function(xii_link_target_vulkan TARGET_NAME)
  xii_requires_vulkan()

  find_package(XIIVulkan REQUIRED)

  if(XIIVULKAN_FOUND)
    target_link_libraries(${TARGET_NAME} PRIVATE XIIVulkan::Loader)

    # Only on linux is the loader a dll.
    if(XII_CMAKE_PLATFORM_LINUX)
      get_target_property(_dll_location XIIVulkan::Loader IMPORTED_LOCATION)
      if(NOT _dll_location STREQUAL "")
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:XIIVulkan::Loader> $<TARGET_FILE_DIR:${TARGET_NAME}>)
      endif()
      unset(_dll_location)
    endif()
  endif()
endfunction()

# #####################################
# ## xii_link_target_dxc(<target>)
# #####################################
function(xii_link_target_dxc TARGET_NAME)
  xii_requires_vulkan()

  find_package(XIIVulkan REQUIRED)

  if(XIIVULKAN_FOUND)
    target_link_libraries(${TARGET_NAME} PRIVATE XIIVulkan::DXC)
    get_target_property(_dll_location XIIVulkan::DXC IMPORTED_LOCATION)
    if(NOT _dll_location STREQUAL "")
      add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:XIIVulkan::DXC> $<TARGET_FILE_DIR:${TARGET_NAME}>)
    endif()
    unset(_dll_location)
  endif()
endfunction()

# #####################################
# ## xii_sources_target_spirv_reflect(<target>)
# #####################################
function(xii_sources_target_spirv_reflect TARGET_NAME)
  xii_requires_vulkan()

  find_package(XIIVulkan REQUIRED)

  if(XIIVULKAN_FOUND)
    if(XII_CMAKE_PLATFORM_WINDOWS_DESKTOP AND XII_CMAKE_ARCHITECTURE_64BIT)
      target_include_directories(${TARGET_NAME} PRIVATE "${XII_VULKAN_DIR}/Source/SPIRV-Reflect")
      target_sources(${TARGET_NAME} PRIVATE "${XII_VULKAN_DIR}/Source/SPIRV-Reflect/spirv_reflect.h")
      target_sources(${TARGET_NAME} PRIVATE "${XII_VULKAN_DIR}/Source/SPIRV-Reflect/spirv_reflect.c")
      source_group("SPIRV-Reflect" FILES "${XII_VULKAN_DIR}/Source/SPIRV-Reflect/spirv_reflect.h" "${XII_VULKAN_DIR}/x86_64/include/SPIRV-Reflect/spirv_reflect.c")
    elseif(XII_CMAKE_PLATFORM_LINUX AND XII_CMAKE_ARCHITECTURE_64BIT)
      target_include_directories(${TARGET_NAME} PRIVATE "${XII_VULKAN_DIR}/x86_64/include/SPIRV-Reflect")
      target_sources(${TARGET_NAME} PRIVATE "${XII_VULKAN_DIR}/x86_64/include/SPIRV-Reflect/spirv_reflect.h")
      target_sources(${TARGET_NAME} PRIVATE "${XII_VULKAN_DIR}/x86_64/include/SPIRV-Reflect/spirv_reflect.c")
      source_group("SPIRV-Reflect" FILES "${XII_VULKAN_DIR}/x86_64/include/SPIRV-Reflect/spirv_reflect.h" "${XII_VULKAN_DIR}/x86_64/include/SPIRV-Reflect/spirv_reflect.c")
    else()
      message(FATAL_ERROR "TODO: Vulkan is not yet supported on this platform and/or architecture.")
    endif()
  endif()
endfunction()
