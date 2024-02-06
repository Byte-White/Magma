# Magma

![magmalogo](https://github.com/Byte-White/Magma/assets/51212450/23f7754f-b537-4ba1-b557-f4012e79ce42)

Magma is a simple application framework built with Dear ImGui, providing an easy-to-use platform for developing both desktop and web applications. It offers support for both Vulkan and OpenGL rendering backends.

## Features

- Simple and intuitive interface powered by Dear ImGui
- Cross-platform support for desktop and web applications
- Rendering backends for Vulkan and OpenGL
- Easy to use

## Requirements
* [VulkanSDK](https://vulkan.lunarg.com/sdk/home) **(Required)**
* [Emscripten](https://emscripten.org/docs/getting_started/downloads.html) **(Optional)** 

## Getting Started

To get started with Magma, follow these steps:

1. Clone the repository recursively
2. Create a `CMakeLists.txt` file
```CMake
cmake_minimum_required(VERSION 3.3)
project(MagmaExample)


add_compile_definitions(MAGMA_VULKAN_ENABLED)
#add_compile_definitions(MAGMA_OPENGL_ENABLED)
#set(MAGMA_EMSCRIPTEN ON)


add_subdirectory(Magma)

add_executable(MagmaExample 
Sandbox/src/main.cpp
)


target_link_libraries(MagmaExample
PRIVATE
magma
)
```
3. Create a main file (in this case `Sandbox/src/main.cpp`)

```c++
#define MAGMA_ENTRYPOINT
#include "Magma/Magma.h"

class MyApplication:public mg::Application
{
public:
	MyApplication()
	{
	}

	void Init()
	{
 		SetSize(600,600);
		SetTitle("Magma Example Application");
	}

	void Destroy() override
	{
	}
	
	~MyApplication()
	{

	}
	void Render() override
	{
		ImGui::ShowDemoWindow();

		ImGui::Begin("Second Window");
			if(ImGui::Button("Click me"))
				SetTitle("You Clicked Me");
			if (ImGui::Button("Resize"))
				SetSize(450,450);
		ImGui::End();
	}
private:
};

mg::Application* CreateApplication(int argc, char** argv)
{
	return new MyApplication();
}
```
![image](https://github.com/Byte-White/Magma/assets/51212450/e3990173-b87e-469e-b29e-bf1628c02207)

> **_NOTE:_**  `#define MAGMA_ENTRYPOINT` should be before including magma and should be defined where `mg::Application* CreateApplication(int argc, char** argv)` is implemented.

**You can also use the [template](https://github.com/Byte-White/Magma-Template)**

## Created With This Framework

### FireBridge - A Serial Monitor

![image](https://github.com/Byte-White/Magma/assets/51212450/58738d9f-afb1-4dde-b7f9-382b924e5973)

## Contributing

We welcome contributions from the community. If you encounter any issues or have suggestions for improvement, please open an issue or submit a pull request on the [GitHub repository](https://github.com/Byte-White/Magma).

## License

Magma is licensed under the MIT License. See the [LICENSE](https://github.com/Byte-White/Magma/LICENSE) file for more details.
