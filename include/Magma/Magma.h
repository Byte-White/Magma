// Magma


//  graphics api check
#if !(defined(MAGMA_VULKAN_ENABLED) || defined(MAGMA_OPENGL_ENABLED))
#error "No Graphics API selected. After initial setup, it may be necessary to rebuild your project to ensure all configurations are properly applied."
#endif 

#if (defined(MAGMA_VULKAN_ENABLED) && defined(MAGMA_OPENGL_ENABLED))
#error "Cannot use two APIs"
#endif

#if (defined(__EMSCRIPTEN__) && defined(MAGMA_VULKAN_ENABLED))
#error "Cannot use Emscripten with Vulkan"
#endif

#include "Application/Application.h"
#include "Application/Themes.h"
#include "Application/Image.h"
#ifdef MAGMA_ENTRYPOINT
#include "Core/EntryPoint.h"
#endif
#include "Core/PlatformDetection.h"
#include "Utility/Input.h"
#include "Utility/Random.h"