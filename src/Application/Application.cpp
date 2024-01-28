#include "Magma/Application/Application.h"
#include "Magma/Core/VulkanImpl.h"
#include "Magma/API/RenderAPI.h"

namespace mg
{
        Application* app;
        

		Application::Application()
		{
            
		}
		Application::~Application()
		{
            
		}
		void Application::Run()
		{
            
            #ifdef __EMSCRIPTEN__
            ImGui::GetIO().IniFilename = nullptr;
            EMSCRIPTEN_MAINLOOP_BEGIN
                #else
            while (!glfwWindowShouldClose(window))
                #endif
            {
                renderer->NewFrame();

                // User Application Render
                Render();

                // Rendering
                renderer->Render();
            }
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