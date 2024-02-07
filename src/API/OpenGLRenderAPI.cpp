#ifdef MAGMA_OPENGL_ENABLED
#include "Magma/API/OpenGLRenderAPI.h"
#include "Magma/Application/Application.h"


#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

namespace mg
{
    void GLAPIENTRY errorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
        MAGMA_CORE_ERROR("OpenGL Error: {0}",message);
        MAGMA_CORE_WARN("Source: {0} , Type: {1} , ID: {2} , Severity: {3}",source,type,id,severity);
    }


    // Defined in VulkanImpl
    void glfw_error_callback(int error, const char* description);

    OpenGLRenderAPI::OpenGLRenderAPI()
    {

    }
    OpenGLRenderAPI::~OpenGLRenderAPI()
    {

    }
    bool OpenGLRenderAPI::Init()
    {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            return false;

        

        // GL 4.3 + GLSL 430
        const char* glsl_version = "#version 430";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);             
        
        

        // Create window with graphics context
        *app->GetWindow() = glfwCreateWindow(app->GetWidth(), app->GetHeight(), app->GetTitle().c_str(), nullptr, nullptr);
        if (app->GetWindow() == nullptr)
            return false;
        glfwMakeContextCurrent(*app->GetWindow());
        glfwSwapInterval(1); // Enable vsync

        // Load GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            MAGMA_CORE_ERROR("Failed to initialize GLAD!");
            return false;
        }

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(errorCallback, nullptr);

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
        ImGui_ImplGlfw_InitForOpenGL(*app->GetWindow(), true);
        #ifdef __EMSCRIPTEN__
        ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
        #endif
        ImGui_ImplOpenGL3_Init(glsl_version);

       
        is_initialized = true;
        return true;
    }
    void OpenGLRenderAPI::Destroy()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(*app->GetWindow());
        glfwTerminate();
    }
    void OpenGLRenderAPI::NewFrame()
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);
    }
    void OpenGLRenderAPI::Render()
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(*app->GetWindow(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(*app->GetWindow());
    }
}
#endif