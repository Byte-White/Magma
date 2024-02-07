#include "Magma/Application/Application.h"
#include "Magma/Core/VulkanImpl.h"
#include "Magma/API/RenderAPI.h"

namespace mg
{
        Application* app;
        
        void glfw_error_callback(int error, const char* description)
        {
            MAGMA_CORE_ERROR("GLFW Error {0}: {1}", error, description);
        }

		Application::Application()
		{
            
		}
		Application::~Application()
		{
            
		}
		void Application::Run()
		{
            Init();
            #ifdef __EMSCRIPTEN__
            ImGui::GetIO().IniFilename = nullptr;
            EMSCRIPTEN_MAINLOOP_BEGIN
            #else
            while (!glfwWindowShouldClose(window) && is_running)
            #endif
            {
                renderer->NewFrame();

                // User Application Render
                Render();

                // Rendering
                renderer->Render();
            }
            #ifdef __EMSCRIPTEN__
            EMSCRIPTEN_MAINLOOP_END
            #endif
            Destroy();
		}

        void Application::Close()
        {
            is_running = false;
        }

        void Application::SetSize(int width, int height)
        {
            window_width = width;
            window_height = height;
            if (renderer->is_initialized)
            {
                glfwSetWindowSize(window, width, height);
            }
        }
        void Application::SetTitle(const std::string& title)
        {
            window_title = title;
            if (renderer->is_initialized)
            {
                glfwSetWindowTitle(window, title.c_str());
            }
        }
}