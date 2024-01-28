cmake_minimum_required(VERSION 3.3)

project(Magma)

set(IMGUI_SOURCE
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_demo.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_vulkan.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
)

set(MAGMA_SOURCE
    src/Core/VulkanImpl.cpp
    src/API/VulkanRenderAPI.cpp
    src/API/OpenGLRenderAPI.cpp
    src/Application/Application.cpp
)

add_subdirectory(vendor)

add_library(
    ${PROJECT_NAME}
    STATIC
    ${IMGUI_SOURCE}
    ${MAGMA_SOURCE}
)

# used for actions
if(MAGMA_VULKAN)
add_compile_definitions(MAGMA_VULKAN_ENABLED)
endif()
if(MAGMA_OPENGL)
add_compile_definitions(MAGMA_OPENGL_ENABLED)
endif()


if(MAGMA_EMSCRIPTEN)
    message(STATUS "Building Magma with Emscripten.")
    if(DEFINED ENV{EMSDK})
       message(STATUS "Emscripten SDK found.")
    else()
       message(FATAL_ERROR "Emscripten SDK was not found.")
    endif()
    # Set Emscripten-specific paths and flags
    set(CMAKE_TOOLCHAIN_FILE "${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" CACHE PATH "")
    
    # Add emscripten compiler to the path
    set(CMAKE_C_COMPILER emcc)
    set(CMAKE_CXX_COMPILER em++)
    
    # Add Emscripten include directory
    target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${EMSDK}/upstream/emscripten/system/include
    )
endif()


find_package(Vulkan REQUIRED)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC 
    ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/vendor/imgui/
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    glfw
    stb_image
    Vulkan::Vulkan
    glad::glad
)

add_library(magma ALIAS ${PROJECT_NAME})
