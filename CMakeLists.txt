cmake_minimum_required(VERSION 3.3)

project(Magma)

option(MAGMA_VULKAN OFF "enable to use Vulkan")
option(MAGMA_OPENGL OFF "enable to use OpenGL")


set(IMGUI_SOURCE
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_demo.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
)

if(MAGMA_VULKAN)
    list(APPEND IMGUI_SOURCE "vendor/imgui/backends/imgui_impl_vulkan.cpp")
elseif(MAGMA_OPENGL)
    list(APPEND IMGUI_SOURCE "vendor/imgui/backends/imgui_impl_opengl3.cpp")
endif()


set(MAGMA_SOURCE
    src/Core/VulkanImpl.cpp
    src/Core/Logging.cpp
    src/API/RenderAPI.cpp
    src/API/VulkanRenderAPI.cpp
    src/API/OpenGLRenderAPI.cpp
    src/Application/Application.cpp
    src/Application/ImageImplVulkan.cpp
    src/Application/ImageImplOpenGL.cpp
)

#vendor

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/stb_image)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/spdlog)
#/vendor


add_library(
    ${PROJECT_NAME}
    STATIC
    ${IMGUI_SOURCE}
    ${MAGMA_SOURCE}
)


if(MAGMA_VULKAN)
    target_compile_definitions(${PROJECT_NAME} PUBLIC MAGMA_VULKAN_ENABLED)
endif()
if(MAGMA_OPENGL)
    target_compile_definitions(${PROJECT_NAME} PUBLIC 
    MAGMA_OPENGL_ENABLED 
    IMGUI_IMPL_OPENGL_LOADER_NONE)
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

if(MAGMA_VULKAN)
        find_package(Vulkan REQUIRED)
        if(TARGET Vulkan::Vulkan)
            message(STATUS "Vulkan found")
        else()
            message(FATAL_ERROR "Vulkan not found")
        endif()
    if(NOT TARGET Vulkan::Vulkan)
            # Create an INTERFACE IMPORTED GLOBAL target for Vulkan.
            add_library(Vulkan::Vulkan INTERFACE IMPORTED GLOBAL)

            # Set the include directories and link libraries for Vulkan.
            set_property(TARGET Vulkan::Vulkan PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Vulkan_INCLUDE_DIRS})
            set_property(TARGET Vulkan::Vulkan PROPERTY INTERFACE_LINK_LIBRARIES ${Vulkan_LIBRARIES})
    endif()
endif()


target_include_directories(
    ${PROJECT_NAME}
    PUBLIC 
    ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/vendor/imgui/
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    glfw
    stb_image
    spdlog
)

if(MAGMA_VULKAN)
target_link_libraries( 
    ${PROJECT_NAME} PUBLIC
    Vulkan::Vulkan
)
endif()

if(MAGMA_OPENGL)
target_link_libraries( 
    ${PROJECT_NAME} PUBLIC
    glad::glad
)
endif()

add_library(magma ALIAS ${PROJECT_NAME})
