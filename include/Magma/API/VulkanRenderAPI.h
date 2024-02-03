#include "RenderAPI.h"
#include "functional"
#include "vulkan/vulkan.h"
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

		void SubmitResourceFree(std::function<void()>&& func);
		VkCommandBuffer GetCommandBuffer(bool begin);
		void FlushCommandBuffer(VkCommandBuffer commandBuffer);
	private:
	};
}
#endif