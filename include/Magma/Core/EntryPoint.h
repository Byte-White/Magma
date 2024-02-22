#ifndef MAGMA_ENTRYPOINT_H
#define MAGMA_ENTRYPOINT_H

#include "Magma/Application/Application.h"
#include "Magma/API/RenderAPI.h"
#include "Magma/API/OpenGLRenderAPI.h"
#include "Magma/API/VulkanRenderAPI.h"
#include "Magma/Core/Logging.h"
#include "Magma/Utility/Random.h"

// To be defined by the user as an entrypoint
// arguments: argc and argv
extern mg::Application* CreateApplication(int, char**);
namespace mg { extern mg::RenderAPI* renderer; }

int main(int argc,char** argv)
{
	mg::Log::Init();
	mg::Random::SetSeed();

	mg::app = CreateApplication(argc, argv);

	#if defined(MAGMA_OPENGL_ENABLED)
	mg::app->GetRenderer() = new mg::OpenGLRenderAPI();
	#ifdef __EMSCRIPTEN__
	MAGMA_CORE_INFO("WebGL API selected.");
	#else
	MAGMA_CORE_INFO("OpenGL API selected.");
	#endif
	#elif defined(MAGMA_VULKAN_ENABLED)
	mg::app->GetRenderer() = new mg::VulkanRenderAPI();
	MAGMA_CORE_INFO("Vulkan API selected.");
	#else
	#error "Select an API"
	#endif

	// Init after creating the application to get the title
	if (!mg::app->GetRenderer()->Init())
	{
		MAGMA_CORE_ERROR("Could not initialize renderer.");
		return 1;
	}
	else MAGMA_CORE_INFO("Renderer Initialized.");

	mg::app->Run();

	mg::app->GetRenderer()->Destroy();
	delete mg::app->GetRenderer();
	delete mg::app;
}

#endif // MAGMA_ENTRYPOINT_H
