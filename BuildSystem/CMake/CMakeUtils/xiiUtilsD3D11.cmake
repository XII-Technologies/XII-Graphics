# #####################################
# ## DirectX11 support
# #####################################

set(XII_BUILD_D3D11 OFF CACHE BOOL "Build the DirectX 11 Graphics Device")

# #####################################
# ## xii_requires_d3d11()
# #####################################

macro(xii_requires_d3d11)
  xii_requires_windows()
  xii_requires(XII_BUILD_D3D11)
endmacro()

# #####################################
# ## xii_link_target_d3d11(<target>)
# #####################################

function(xii_link_target_d3d11 TARGET_NAME)
  xii_requires_d3d11()

  get_property(XII_DX11_LIBRARY GLOBAL PROPERTY XII_DX11_LIBRARY)

  # Execute find_package once
  if(NOT XII_DX11_LIBRARY)
    find_package(DirectX11 REQUIRED)

    if(DirectX11_FOUND)
      set_property(GLOBAL PROPERTY XII_DX11_LIBRARY ${DirectX11_LIBRARY})
      set_property(GLOBAL PROPERTY XII_DX11_LIBRARIES ${DirectX11_D3D11_LIBRARIES})
    endif()
  endif()

  get_property(XII_DX11_LIBRARY GLOBAL PROPERTY XII_DX11_LIBRARY)
  get_property(XII_DX11_LIBRARIES GLOBAL PROPERTY XII_DX11_LIBRARIES)

  target_link_libraries(${TARGET_NAME}
    PRIVATE
    ${XII_DX11_LIBRARIES}
  )

  if(XII_CMAKE_ARCHITECTURE_ARM)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(DX11_COPY_DLLS_BIT "arm64")
    else()
      set(DX11_COPY_DLLS_BIT "arm")
    endif()
  else()
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(DX11_COPY_DLLS_BIT "x64")
    else()
      set(DX11_COPY_DLLS_BIT "x86")
    endif()
  endif()

  # arm dll is not provide in the windows SDK.
  if(NOT XII_CMAKE_ARCHITECTURE_ARM)
    if(${XII_DX11_LIBRARY} MATCHES "/8\\.0/")
      set(DX11_COPY_DLLS_WINSDKVERSION "8.0")
      set(DX11_COPY_DLLS_DLL_VERSION "46")
    elseif(${XII_DX11_LIBRARY} MATCHES "/8\\.1/")
      set(DX11_COPY_DLLS_WINSDKVERSION "8.1")
      set(DX11_COPY_DLLS_DLL_VERSION "47")
    elseif(${XII_DX11_LIBRARY} MATCHES "/10/")
      set(DX11_COPY_DLLS_WINSDKVERSION "10")
      set(DX11_COPY_DLLS_DLL_VERSION "47")
    endif()
  endif()

  if(${DX11_COPY_DLLS_WINSDKVERSION})
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "%ProgramFiles(x86)%/Windows Kits/${DX11_COPY_DLLS_WINSDKVERSION}/Redist/D3D/${DX11_COPY_DLLS_BIT}/d3dcompiler_${DX11_COPY_DLLS_DLL_VERSION}.dll"
      $<TARGET_FILE_DIR:${TARGET_NAME}>
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
  endif()
endfunction()
