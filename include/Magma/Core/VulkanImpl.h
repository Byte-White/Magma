#ifndef MAGMA_VULKAN_IMPL_H
#define MAGMA_VULKAN_IMPL_H

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include <imgui.h> 

#include "Magma/Application/Application.h"

// #include <emscripten/html5.h>

namespace mg
{
	extern Application* app;
	namespace entrypoint
	{
		extern ImGui_ImplVulkanH_Window* wd;
	}
	void glfw_error_callback(int error, const char* description);

	namespace vk 
	{
		

		//#define IMGUI_UNLIMITED_FRAME_RATE
		#ifdef _DEBUG
		#define IMGUI_VULKAN_DEBUG_REPORT
		#endif

		// Data
		extern VkAllocationCallbacks* g_Allocator;
		extern VkInstance g_Instance;
		extern VkPhysicalDevice g_PhysicalDevice;
		extern VkDevice g_Device;
		extern uint32_t g_QueueFamily;
		extern VkQueue g_Queue;
		extern VkDebugReportCallbackEXT g_DebugReport;
		extern VkPipelineCache g_PipelineCache;
		extern VkDescriptorPool g_DescriptorPool;

		extern ImGui_ImplVulkanH_Window g_MainWindowData;
		extern int g_MinImageCount;
		extern bool g_SwapChainRebuild;


		void check_vk_result(VkResult err);

		#ifdef IMGUI_VULKAN_DEBUG_REPORT
		VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData);
		#endif // IMGUI_VULKAN_DEBUG_REPORT

		bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension);
		VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();
		void SetupVulkan(ImVector<const char*> instance_extensions);
		void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);
		void CleanupVulkan();
		void CleanupVulkanWindow();
		void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
		void FramePresent(ImGui_ImplVulkanH_Window* wd);
	}
}

#endif

