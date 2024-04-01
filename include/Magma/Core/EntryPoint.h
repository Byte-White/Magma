#ifndef MAGMA_ENTRYPOINT_H
#define MAGMA_ENTRYPOINT_H

#include "Magma/Application/Application.h"

#include "Magma/Core/Logging.h"
#include "Magma/Utility/Random.h"

// To be defined by the user as an entrypoint
// arguments: argc and argv
extern mg::Application* CreateApplication(int, char**);

int main(int argc,char** argv)
{
	mg::Log::Init();
	mg::Random::SetSeed();

	mg::app = CreateApplication(argc, argv);

	#if defined(MAGMA_OPENGL_ENABLED)
		#ifdef __EMSCRIPTEN__
			MAGMA_CORE_INFO("WebGL API selected.");
		#else
			MAGMA_CORE_INFO("OpenGL API selected.");
		#endif
	#elif defined(MAGMA_VULKAN_ENABLED)
		MAGMA_CORE_INFO("Vulkan API selected.");
	#else
		#error "Select an API"
	#endif

	mg::app->Run();
	
	delete mg::app;
}

#endif // MAGMA_ENTRYPOINT_H
