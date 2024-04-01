#include "Magma/Application/Application.h"
#include "Magma/Utility/Input.h"
#include "Magma/Resources/FiraCode.embed"

namespace mg
{
        Application* app;
        
        void glfw_error_callback(int error, const char* description)
        {
            MAGMA_CORE_ERROR("GLFW Error {0}: {1}", error, description);
        }

		Application::Application()
		{
            if (APISetup() == false)
            {
                MAGMA_CORE_ERROR("Setup failed.");
                exit(1);
            }
            else MAGMA_CORE_INFO("Setup complete.");
            
            m_initialized = true;
        }
		Application::~Application()
		{
            mg::app->APICleanup();
		}

		void Application::Run()
		{
            Input::SetWindow(m_window);
            Init();
            #ifdef __EMSCRIPTEN__
            ImGui::GetIO().IniFilename = nullptr;
            EMSCRIPTEN_MAINLOOP_BEGIN
            #else
            while (!glfwWindowShouldClose(m_window) && m_running)
            #endif
            {
                APINewFrame();

                // User Application Render
                Render();

                APIRenderFrame();
            }
            #ifdef __EMSCRIPTEN__
            EMSCRIPTEN_MAINLOOP_END
            #endif
            Destroy();
		}

        void Application::Close()
        {
            m_running = false;
        }

        void Application::SetSize(int width, int height)
        {
            m_windowWidth = width;
            m_windowHeight = height;
            if (m_initialized)
                glfwSetWindowSize(m_window, width, height);
        }
        void Application::SetTitle(const std::string& title)
        {
            m_windowTitle = title;
            if(m_initialized)
                glfwSetWindowTitle(m_window, title.c_str());
        }

        void Application::SetupTheme()
        {
            // Color Theme
            CrimsonShadowTheme();
            // DarkForestTheme();
            // DeepOceanTheme();
            // Loading Font

            ImGuiIO& io = ImGui::GetIO();
            ImFontConfig fontConfig;
            fontConfig.FontDataOwnedByAtlas = false;
            ImFont* firacodeFont = io.Fonts->AddFontFromMemoryTTF((void*)g_FiraCode, sizeof(g_FiraCode) / sizeof(unsigned char), 20.0f, &fontConfig);
            io.FontDefault = firacodeFont;
        }
}