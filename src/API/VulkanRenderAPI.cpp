#include "Magma/API/VulkanRenderAPI.h"
#include "Magma/Core/VulkanImpl.h"
#include <iostream>



namespace mg
{
    static VkSurfaceKHR surface;
    static VkResult err;
    static ImGui_ImplVulkan_InitInfo init_info;

	VulkanRenderAPI::VulkanRenderAPI()
	{

	}
	VulkanRenderAPI::~VulkanRenderAPI()
	{

	}
	bool VulkanRenderAPI::Init()
	{
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            return false;

        // Create window with Vulkan context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        *app->GetWindow() = glfwCreateWindow(app->GetWidth(), app->GetHeight(), app->GetTitle().c_str(), nullptr, nullptr);
        if (!glfwVulkanSupported())
        {
            printf("GLFW: Vulkan Not Supported\n");
            return false;
        }

        ImVector<const char*> extensions;
        uint32_t extensions_count = 0;
        const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
        for (uint32_t i = 0; i < extensions_count; i++)
            extensions.push_back(glfw_extensions[i]);
        vk::SetupVulkan(extensions);


        err = glfwCreateWindowSurface(vk::g_Instance, *app->GetWindow(), vk::g_Allocator, &surface);
        vk::check_vk_result(err);

        // Create Framebuffers
        int w, h;
        glfwGetFramebufferSize(*app->GetWindow(), &w, &h);
        entrypoint::wd = &vk::g_MainWindowData;
        vk::SetupVulkanWindow(entrypoint::wd, surface, w, h);

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForVulkan(*app->GetWindow(), true);
        init_info = {};
        init_info.Instance = vk::g_Instance;
        init_info.PhysicalDevice = vk::g_PhysicalDevice;
        init_info.Device = vk::g_Device;
        init_info.QueueFamily = vk::g_QueueFamily;
        init_info.Queue = vk::g_Queue;
        init_info.PipelineCache = vk::g_PipelineCache;
        init_info.DescriptorPool = vk::g_DescriptorPool;
        init_info.Subpass = 0;
        init_info.MinImageCount = vk::g_MinImageCount;
        init_info.ImageCount = entrypoint::wd->ImageCount;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = vk::g_Allocator;
        init_info.CheckVkResultFn = vk::check_vk_result;
        ImGui_ImplVulkan_Init(&init_info, entrypoint::wd->RenderPass);
        is_initialized = true;
        return true;
	}
	void VulkanRenderAPI::Destroy()
	{
        // Cleanup
        err = vkDeviceWaitIdle(vk::g_Device);
        vk::check_vk_result(err);
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        vk::CleanupVulkanWindow();
        vk::CleanupVulkan();

        glfwDestroyWindow(*app->GetWindow());
        glfwTerminate();
	}
	void VulkanRenderAPI::NewFrame()
	{
        glfwPollEvents();

        // Resize swap chain?
        if (mg::vk::g_SwapChainRebuild)
        {
            int width, height;
            glfwGetFramebufferSize(*app->GetWindow(), &width, &height);
            if (width > 0 && height > 0)
            {
                ImGui_ImplVulkan_SetMinImageCount(vk::g_MinImageCount);
                ImGui_ImplVulkanH_CreateOrResizeWindow(vk::g_Instance, vk::g_PhysicalDevice, vk::g_Device, &vk::g_MainWindowData, vk::g_QueueFamily, vk::g_Allocator, width, height, vk::g_MinImageCount);
                vk::g_MainWindowData.FrameIndex = 0;
                vk::g_SwapChainRebuild = false;
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();



        ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);
	}
	void VulkanRenderAPI::Render()
	{
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Render();
        ImDrawData* main_draw_data = ImGui::GetDrawData();
        const bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);
        entrypoint::wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
        entrypoint::wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
        entrypoint::wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
        entrypoint::wd->ClearValue.color.float32[3] = clear_color.w;
        if (!main_is_minimized)
            vk::FrameRender(entrypoint::wd, main_draw_data);

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        // Present Main Platform Window
        if (!main_is_minimized)
            vk::FramePresent(entrypoint::wd);
	}
}