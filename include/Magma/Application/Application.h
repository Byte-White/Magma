#ifndef MAGMA_APPLICATION_H
#define MAGMA_APPLICATION_H
#include <imgui.h>
#include <GLFW/glfw3.h>
#include "Magma/API/RenderAPI.h"
#include <string>

namespace mg 
{

	class Application
	{
	public:
		
		Application();
		~Application();
		void Run();
		virtual void Init()	  {}
		virtual void Destroy(){}
		virtual void Render() {}
		inline GLFWwindow** GetWindow()
		{
			return &window;
		}

		RenderAPI* renderer;
		inline const int GetWidth()				{ return window_width; }
		inline const int GetHeight()			{ return window_height; }
		inline const std::string GetTitle()		{ return window_title; }
		inline const ImVec4 GetClearColor()		{ return clear_color; }

		void SetSize(int width,int height);
		void SetTitle(const std::string& title);
		void SetClearColor(ImVec4 color);
	protected:
		bool is_running = true;
		ImVec4 clear_color;
		GLFWwindow* window;
		int window_width = 1280, window_height = 720;
		std::string window_title = "Magma Application";
	};

	extern Application* app;
}

#endif