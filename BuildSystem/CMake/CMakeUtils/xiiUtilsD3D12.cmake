# #####################################
# ## DirectX 12 support
# #####################################

set(XII_BUILD_D3D12 OFF CACHE BOOL "Build the DirectX 12 Graphics Device")

# #####################################
# ## xii_requires_d3d12()
# #####################################

macro(xii_requires_d3d12)
  xii_requires_windows()
  xii_requires(XII_BUILD_D3D12)
endmacro()

# #####################################
# ## xii_link_target_dx11(<target>)
# #####################################

function(xii_link_target_d3d12 TARGET_NAME)
  xii_requires_d3d12()

  get_property(XII_D3D12_LIBRARY GLOBAL PROPERTY XII_D3D12_LIBRARY)
  # Execute find_package once
  if(NOT XII_D3D12_LIBRARY)
    find_package(DirectX12 REQUIRED)
    if(DirectX12_FOUND)
      set_property(GLOBAL PROPERTY XII_D3D12_LIBRARY ${DirectX12_LIBRARY})
      set_property(GLOBAL PROPERTY XII_D3D12_LIBRARIES ${DirectX12_D3D12_LIBRARIES})
    endif()
  endif()

  get_property(XII_D3D12_LIBRARY GLOBAL PROPERTY XII_D3D12_LIBRARY)
  get_property(XII_D3D12_LIBRARIES GLOBAL PROPERTY XII_D3D12_LIBRARIES)

  target_link_libraries(${TARGET_NAME}
    PRIVATE
    ${XII_D3D12_LIBRARIES}
  )

  if(XII_CMAKE_ARCHITECTURE_ARM)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(D3D12_COPY_DLLS_BIT "arm64")
    else()
      set(D3D12_COPY_DLLS_BIT "arm")
      endif()
    else()
      if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(D3D12_COPY_DLLS_BIT "x64")
    else()
        set(D3D12_COPY_DLLS_BIT "x86")
    endif()
  endif()

  # ARM dll is not provide in the windows SDK.
  if(NOT XII_CMAKE_ARCHITECTURE_ARM)
    if(${XII_D3D12_LIBRARY} MATCHES "/10/")
      set(D3D12_COPY_DLLS_WINSDKVERSION "10")
      set(D3D12_COPY_DLLS_DLL_VERSION "47")
    endif()
  endif()

  # Copy the DirectX Shader Compiler DLL.
  if(${D3D12_COPY_DLLS_WINSDKVERSION})
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "%ProgramFiles(x86)%/Windows Kits/${D3D12_COPY_DLLS_WINSDKVERSION}/Redist/D3D/${D3D12_COPY_DLLS_BIT}/d3dcompiler_${D3D12_COPY_DLLS_DLL_VERSION}.dll"
      $<TARGET_FILE_DIR:${TARGET_NAME}>
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
  endif()

  # Copy the DXIL (signer) dll.
  if(${D3D12_COPY_DLLS_WINSDKVERSION})
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "%ProgramFiles(x86)%/Windows Kits/${D3D12_COPY_DLLS_WINSDKVERSION}/Redist/D3D/${D3D12_COPY_DLLS_BIT}/dxil.dll"
      $<TARGET_FILE_DIR:${TARGET_NAME}>
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
  endif()

endfunction()
