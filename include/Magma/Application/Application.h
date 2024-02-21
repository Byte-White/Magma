#ifndef MAGMA_APPLICATION_H
#define MAGMA_APPLICATION_H
#include <imgui.h>
#include "Magma/API/RenderAPI.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>

namespace mg 
{
	void glfw_error_callback(int error, const char* description);
	
	class Application
	{
	public:
		
		Application();
		~Application();
		void Run();
		void Close();
		virtual void Init()	  {}
		virtual void Destroy(){}
		virtual void Render() {}
		inline GLFWwindow** GetWindow()
		{
			return &window;
		}
		inline RenderAPI*& GetRenderer()		{ return m_renderer; }
		inline const int GetWidth()				{ return window_width; }
		inline const int GetHeight()			{ return window_height; }
		inline const std::string GetTitle()		{ return window_title; }
		inline const ImVec4 GetClearColor()		{ return clear_color; }

		void SetSize(int width,int height);
		void SetTitle(const std::string& title);
		void SetClearColor(ImVec4 color);
	protected:
		RenderAPI* m_renderer;
		bool m_running = true;
		ImVec4 clear_color;
		GLFWwindow* window;
		int window_width = 1280, window_height = 720;
		std::string window_title = "Magma Application";
	};

	extern Application* app;
}

#endif