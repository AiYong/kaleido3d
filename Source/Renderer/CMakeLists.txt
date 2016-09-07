include_directories(
../.. ../../../Include
${Kaleido3D_ROOT_DIR}/Source/RHI/D3D12
${DXSDK_INCLUDE_DIR}
${V8_INCLUDE_DIR}
)

if(BUILD_SHARED)
	add_definitions(-DBUILD_SHARED_LIB)
endif()

link_directories(${V8_LIBRARY_DIR})

_add_package_VulkanSDK()

set(RENDER_SRCS
Render.h
Render.cpp
MeshRender.cpp
RenderContext.h
RenderContext.cpp
DynamicRHILoader.h
DynamicRHILoader.cpp
V8Engine.cpp
)

add_library(Render ${LIB_TYPE}
${RENDER_SRCS}
)

target_link_libraries(
Render
Core
RHI_Vulkan
RHI_D3D12
${V8_LIBRARIES}
)

set_target_properties(Render PROPERTIES FOLDER "Runtime")

install(TARGETS Render ARCHIVE DESTINATION lib)