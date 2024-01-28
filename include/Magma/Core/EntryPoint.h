#ifndef MAGMA_ENTRYPOINT_H
#define MAGMA_ENTRYPOINT_H

#include "Magma/Application/Application.h"
#include "Magma/API/RenderAPI.h"
#include "Magma/API/OpenGLRenderAPI.h"
#include "Magma/API/VulkanRenderAPI.h"

// To be defined by the user as an entrypoint
// arguments: argc and argv
extern mg::Application* CreateApplication(int, char**);
namespace mg { extern mg::RenderAPI* renderer; }

int main(int argc,char** argv)
{
	mg::app = CreateApplication(argc, argv);

	#if defined(MAGMA_OPENGL_ENABLED)
	mg::app->renderer = new mg::OpenGLRenderAPI();
	#elif defined(MAGMA_VULKAN_ENABLED)
	mg::app->renderer = new mg::VulkanRenderAPI();
	#else
	#error "Select an API"
	#endif

	// Init after creating the application to get the title
	if (!mg::app->renderer->Init())
		return 1;
	mg::app->Run();

	mg::app->renderer->Destroy();
	delete mg::app;
}

#endif // MAGMA_ENTRYPOINT_H
