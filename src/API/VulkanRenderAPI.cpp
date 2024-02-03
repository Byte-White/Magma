#include "Magma/API/VulkanRenderAPI.h"
#include "Magma/Core/VulkanImpl.h"
#include <iostream>



namespace mg
{
    static VkSurfaceKHR surface;
    static VkResult err;
    static ImGui_ImplVulkan_InitInfo init_info;

    static uint32_t s_CurrentFrameIndex = 0;
    static std::vector<std::vector<VkCommandBuffer>> s_AllocatedCommandBuffers; 
    static std::vector<std::vector<std::function<void()>>> s_ResourceFreeQueue;

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
            MAGMA_CORE_ERROR("GLFW: Vulkan Not Supported");
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
        vk::SetupVulkanWindow(&vk::g_MainWindowData, surface, w, h);

        // Initializing vectors based on the number of images in the main window data.
        s_AllocatedCommandBuffers.resize(vk::g_MainWindowData.ImageCount);
        s_ResourceFreeQueue.resize(vk::g_MainWindowData.ImageCount);

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
        init_info.ImageCount = vk::g_MainWindowData.ImageCount;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = vk::g_Allocator;
        init_info.CheckVkResultFn = vk::check_vk_result;
        ImGui_ImplVulkan_Init(&init_info, vk::g_MainWindowData.RenderPass);
        is_initialized = true;
        return true;
	}
	void VulkanRenderAPI::Destroy()
	{
        // Cleanup
        err = vkDeviceWaitIdle(vk::g_Device);
        vk::check_vk_result(err);

        // Free resources in queue
        for (auto& queue : s_ResourceFreeQueue)
        {
            for (auto& func : queue)
                func();
        }
        s_ResourceFreeQueue.clear();


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

                // Clear allocated command buffers from here since entire pool is destroyed
                s_AllocatedCommandBuffers.clear();
                s_AllocatedCommandBuffers.resize(vk::g_MainWindowData.ImageCount);
                
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
        vk::g_MainWindowData.ClearValue.color.float32[0] = clear_color.x * clear_color.w;
        vk::g_MainWindowData.ClearValue.color.float32[1] = clear_color.y * clear_color.w;
        vk::g_MainWindowData.ClearValue.color.float32[2] = clear_color.z * clear_color.w;
        vk::g_MainWindowData.ClearValue.color.float32[3] = clear_color.w;
        if (!main_is_minimized)
            vk::FrameRender(&vk::g_MainWindowData, main_draw_data);

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        // Present Main Platform Window
        if (!main_is_minimized)
            vk::FramePresent(&vk::g_MainWindowData);

        for (auto& func : s_ResourceFreeQueue[s_CurrentFrameIndex])
            func();
        s_ResourceFreeQueue[s_CurrentFrameIndex].clear();
        s_CurrentFrameIndex = (s_CurrentFrameIndex + 1) % vk::g_MainWindowData.ImageCount;
	}

    void VulkanRenderAPI::SubmitResourceFree(std::function<void()>&& func)
    {
        s_ResourceFreeQueue[s_CurrentFrameIndex].emplace_back(func);
    }

    VkCommandBuffer VulkanRenderAPI::GetCommandBuffer(bool begin)
    {
        VkCommandPool command_pool = vk::g_MainWindowData.Frames[vk::g_MainWindowData.FrameIndex].CommandPool;

        VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
        cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmdBufAllocateInfo.commandPool = command_pool;
        cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        cmdBufAllocateInfo.commandBufferCount = 1;

        VkCommandBuffer command_buffer;
        auto err = vkAllocateCommandBuffers(vk::g_Device, &cmdBufAllocateInfo, &command_buffer);
        vk::check_vk_result(err);

        if (begin)
        {
            VkCommandBufferBeginInfo begin_info = {};
            begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            err = vkBeginCommandBuffer(command_buffer, &begin_info);
            vk::check_vk_result(err);
        }

        // Add the allocated command buffer to the s_AllocatedCommandBuffers vector
        s_AllocatedCommandBuffers[vk::g_MainWindowData.FrameIndex].emplace_back(command_buffer);

        return command_buffer;
    }


    void VulkanRenderAPI::FlushCommandBuffer(VkCommandBuffer commandBuffer)
    {
        const uint64_t DEFAULT_FENCE_TIMEOUT = 100000000000;

        VkSubmitInfo end_info = {};
        end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        end_info.commandBufferCount = 1;
        end_info.pCommandBuffers = &commandBuffer;
        auto err = vkEndCommandBuffer(commandBuffer);
        vk::check_vk_result(err);

        // Create fence to ensure that the command buffer has finished executing
        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.flags = 0;
        VkFence fence;
        err = vkCreateFence(vk::g_Device, &fenceCreateInfo, nullptr, &fence);
        vk::check_vk_result(err);

        err = vkQueueSubmit(vk::g_Queue, 1, &end_info, fence);
        vk::check_vk_result(err);

        err = vkWaitForFences(vk::g_Device, 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT);
        vk::check_vk_result(err);

        vkDestroyFence(vk::g_Device, fence, nullptr);
    }
}