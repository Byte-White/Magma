#include "glad/glad.h"
#include "RenderAPI.h"
#ifndef MAGMA_OPENGLRENDERAPI_H
#define MAGMA_OPENGLRENDERAPI_H


namespace mg
{
	class OpenGLRenderAPI: public RenderAPI
	{
	public:
		OpenGLRenderAPI();
		~OpenGLRenderAPI();
		bool Init();
		void Destroy();
		void NewFrame();
		void Render();
	private:
	};
}
#endif