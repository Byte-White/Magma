#include "RenderAPI.h"
#ifndef MAGMA_VULKANRENDERAPI_H
#define MAGMA_VULKANRENDERAPI_H


namespace mg
{
	class VulkanRenderAPI:public RenderAPI
	{
	public:
		VulkanRenderAPI();
		~VulkanRenderAPI();
		bool Init();
		void Destroy();
		void NewFrame();
		void Render();
	private:
	};
}
#endif