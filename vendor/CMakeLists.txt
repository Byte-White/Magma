
project(vendor)

add_subdirectory(glfw)
add_subdirectory(stb_image)
# add_subdirectory(glm) # might add it in the future
add_subdirectory(glad)


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