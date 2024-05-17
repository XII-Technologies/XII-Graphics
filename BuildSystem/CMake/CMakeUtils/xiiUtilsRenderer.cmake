# #####################################
# ## xii_requires_renderer()
# #####################################

macro(xii_requires_renderer)
  xii_requires_one_of(XII_BUILD_D3D11 XII_BUILD_D3D12 XII_BUILD_VULKAN)
endmacro()

# #####################################
# ## xii_add_renderers(<target>)
# ## Add all required libraries and dependencies to the given target so it has access to all available renderers.
# #####################################

function(xii_add_renderers TARGET_NAME)
  add_dependencies(${TARGET_NAME}
    GraphicsNull
    ShaderCompiler
  )

  if (XII_BUILD_D3D11)
    if(TARGET GraphicsD3D11)
      add_dependencies(${TARGET_NAME}
        GraphicsD3D11
      )
    endif()
  endif()

  if (XII_BUILD_D3D12)
    if(TARGET GraphicsD3D12)
      add_dependencies(${TARGET_NAME}
        GraphicsD3D12
      )
    endif()
  endif()

  if (XII_BUILD_VULKAN)
    if(TARGET GraphicsVulkan)
      add_dependencies(${TARGET_NAME}
        GraphicsVulkan
      )
    endif()
  endif()
endfunction()
