#ifndef MAGMA_APPLICATION_H
#define MAGMA_APPLICATION_H

#define GLFW_INCLUDE_NONE
#ifdef MAGMA_VULKAN_ENABLED
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

#include "backends/imgui_impl_glfw.h"
#ifdef MAGMA_VULKAN_ENABLED
#include "vulkan/vulkan.h"
#include "backends/imgui_impl_vulkan.h"
#elif MAGMA_OPENGL_ENABLED
#include "backends/imgui_impl_opengl3.h"
#include "glad/glad.h"
#endif
#include <imgui.h> 

#include <string>
#include <vector>

#include "Magma/Application/Themes.h"
#include "Magma/Core/Logging.h"


namespace mg 
{
	enum class GraphicsAPI
	{
		Vulkan = 0,
		OpenGL
	};

	void glfw_error_callback(int error, const char* description);
	
	#ifdef MAGMA_VULKAN_ENABLED

	#ifdef IMGUI_VULKAN_DEBUG_REPORT
	VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData);
	#endif // IMGUI_VULKAN_DEBUG_REPORT

	void check_vk_result(VkResult err);
	#endif

	class Application
	{
	public:

		static inline GraphicsAPI GetAPI()
		{
			#ifdef MAGMA_VULKAN_ENABLED
			return GraphicsAPI::Vulkan;
			#elif MAGMA_OPENGL_ENABLED
			return GraphicsAPI::OpenGL;
			#endif
		}

		Application();
		~Application();
		void Run();
		void Close();
		virtual void Init()	  {}
		virtual void Destroy(){}
		virtual void Render() {}
		inline GLFWwindow** GetWindow()
		{
			return &m_window;
		}

		inline const int GetWidth()				{ return m_windowWidth; }
		inline const int GetHeight()			{ return m_windowHeight; }
		inline const std::string GetTitle()		{ return m_windowTitle; }
		inline const ImVec4 GetClearColor()		{ return m_clearColor; }
		static inline float GetTime()			{ return (float)glfwGetTime(); }


		void SetSize(int width,int height);
		void SetTitle(const std::string& title);
		void SetClearColor(ImVec4 color);

		void SetupTheme();

		// API specific
		bool APISetup();
		void APICleanup();
		void APINewFrame();
		void APIRenderFrame();

	#ifdef MAGMA_VULKAN_ENABLED
	public:
		inline VkAllocationCallbacks* GetAllocator() const { return m_Allocator; }
		inline VkInstance GetInstance() const { return m_Instance; }
		inline VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
		inline VkDevice GetDevice() const { return m_Device; }
		inline uint32_t GetQueueFamily() const { return m_QueueFamily; }
		inline VkQueue GetQueue() const { return m_Queue; }
		inline VkDebugReportCallbackEXT GetDebugReportCallback() const { return m_DebugReport; }
		inline VkPipelineCache GetPipelineCache() const { return m_PipelineCache; }
		inline VkDescriptorPool GetDescriptorPool() const { return m_DescriptorPool; }
		inline ImGui_ImplVulkanH_Window GetMainWindowData() const { return m_MainWindowData; }
		inline int GetMinImageCount() const { return m_MinImageCount; }
		inline bool isSwapChainRebuild() const { return m_SwapChainRebuild; }

		void SubmitResourceFree(std::function<void()>&& func);
		VkCommandBuffer GetCommandBuffer(bool begin);
		void FlushCommandBuffer(VkCommandBuffer commandBuffer);

		// Vulkan helper function
	protected:
		bool IsExtensionAvailable(const std::vector<VkExtensionProperties>& properties, const char* extension);
		VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();
		void SetupVulkan(std::vector<const char*> instance_extensions);
		void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);
		void CleanupVulkan();
		void CleanupVulkanWindow();
		void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
		void FramePresent(ImGui_ImplVulkanH_Window* wd);
	#endif
	protected: // api specific
		#ifdef MAGMA_VULKAN_ENABLED

		VkAllocationCallbacks* m_Allocator = nullptr;
		VkInstance m_Instance = VK_NULL_HANDLE;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_Device = VK_NULL_HANDLE;
		uint32_t m_QueueFamily = (uint32_t)-1;
		VkQueue m_Queue = VK_NULL_HANDLE;
		VkDebugReportCallbackEXT m_DebugReport = VK_NULL_HANDLE;
		VkPipelineCache m_PipelineCache = VK_NULL_HANDLE;
		VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
		ImGui_ImplVulkanH_Window m_MainWindowData;
		int m_MinImageCount = 2;
		bool m_SwapChainRebuild = false;

		ImGui_ImplVulkan_InitInfo m_InitInfo;
		VkSurfaceKHR m_Surface;
		uint32_t m_CurrentFrameIndex = 0;
		std::vector<std::vector<VkCommandBuffer>> m_AllocatedCommandBuffers;
		std::vector<std::vector<std::function<void()>>> m_ResourceFreeQueue;

		#endif


	protected: // cross api
		bool m_running = true, m_initialized = false;
		ImVec4 m_clearColor;
		GLFWwindow* m_window;
		int m_windowWidth = 1280, m_windowHeight = 720;
		std::string m_windowTitle = "Magma Application";
	};

	extern Application* app;
}

#endif