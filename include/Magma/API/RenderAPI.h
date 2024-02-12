#include "imgui.h"
#include "Magma/Core/Logging.h"

#ifndef MAGMA_RENDERAPI_H
#define MAGMA_RENDERAPI_H

namespace mg
{
	class RenderAPI
	{
	public:
		enum API{
			Vulkan = 0,
			OpenGL
		};
		virtual bool Init() { return false; }
		virtual void Destroy(){}
		virtual void NewFrame(){}
		virtual void Render(){}

		ImVec4 clear_color;

		static inline API GetAPI()
		{
			#ifdef MAGMA_VULKAN_ENABLED
			return Vulkan;
			#elif MAGMA_OPENGL_ENABLED
			return OpenGL;
			#endif
		}
		bool is_initialized = false;
	protected:
		void SetupTheme();
	};
}
#endif