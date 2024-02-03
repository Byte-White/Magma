// Magma


//  graphics api check
#if !(defined(MAGMA_VULKAN_ENABLED) || defined(MAGMA_OPENGL_ENABLED))
#error "No Graphics API selected"
#endif 

#if (defined(MAGMA_VULKAN_ENABLED) && defined(MAGMA_OPENGL_ENABLED))
#error "Cannot use two APIs"
#endif

#if (defined(__EMSCRIPTEN__) && defined(MAGMA_VULKAN_ENABLED))
#error "Cannot use Emscripten with Vulkan"
#endif

#include "Core/VulkanImpl.h"
#include "Application/Application.h"
#include "Core/EntryPoint.h"
#include "Application/Image.h"
