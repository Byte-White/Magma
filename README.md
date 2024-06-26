# Magma

![magmalogo](https://github.com/Byte-White/Magma/assets/51212450/23f7754f-b537-4ba1-b557-f4012e79ce42)

Magma is a simple application framework built with Dear ImGui, providing an easy-to-use platform for developing both desktop and web applications. It offers support for both Vulkan and OpenGL rendering backends.

## Features

- Simple and intuitive interface powered by Dear ImGui
- Cross-platform support for desktop and web applications
- Rendering backends for Vulkan and OpenGL
- Easy to use

## Requirements
* [CMake](https://cmake.org/download/) **(Required)**
* [VulkanSDK](https://vulkan.lunarg.com/sdk/home) **(Optional)**
* [Emscripten](https://emscripten.org/docs/getting_started/downloads.html) **(Optional)** 

## Getting Started

To get started with Magma, follow these steps:

1. Clone the repository recursively
2. Create a `CMakeLists.txt` file
```CMake
cmake_minimum_required(VERSION 3.3)
project(MagmaExample)


set(MAGMA_VULKAN ON)
set(MAGMA_OPENGL OFF)
set(MAGMA_EMSCRIPTEN OFF)

#add_compile_definitions(MAGMA_NO_LOGGING) #uncomment to stop all logging
#add_compile_definitions(MAGMA_NO_CORE_LOGGING) #uncomment to stop core logging
#add_compile_definitions(MAGMA_NO_CLIENT_LOGGING) #uncomment to stop client logging

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
![image](https://github.com/Byte-White/Magma/assets/51212450/d3fc7b5d-dbf9-4330-aed0-8be3133a15b2)

> **_NOTE:_**  `#define MAGMA_ENTRYPOINT` should be before including magma and should be defined where `mg::Application* CreateApplication(int argc, char** argv)` is implemented.

> **After initial setup, it may be necessary to rebuild your project to ensure all configurations are properly applied.**

**You can also use the [app template](https://github.com/Byte-White/Magma-Template) and the [app with layers template](https://github.com/Byte-White/Magma-Layers-Template)**

## Created With This Framework

### FireBridge - A Serial Monitor

![image](https://github.com/Byte-White/Magma/assets/51212450/58738d9f-afb1-4dde-b7f9-382b924e5973)

[FireBridge repository](https://github.com/Byte-White/FireBridge)

### Lightning Engine - A CPU Ray Tracing Engine

![image](https://github.com/Byte-White/Lightning-Engine/assets/51212450/2691336d-6bd5-477f-ad7f-63c51a1775b4)

[Lightning-Engine repository](https://github.com/Byte-White/Lightning-Engine)

### Embed - a server code generator for Arduino

![draganddrop](https://github.com/Byte-White/Embed/assets/51212450/22e453ed-a5a3-4da1-8168-1f0e426f2645)

![export](https://github.com/Byte-White/Embed/assets/51212450/99567722-64e1-4516-85b3-978746726085)

[Embed repository](https://github.com/Byte-White/Embed)


## Contributing

We welcome contributions from the community. If you encounter any issues or have suggestions for improvement, please open an issue or submit a pull request on the [GitHub repository](https://github.com/Byte-White/Magma).

## License

- Magma is licensed under the MIT License. See the [LICENSE](https://github.com/Byte-White/Magma/LICENSE) file for more details.
- Magma uses the [FiraCode](https://fonts.google.com/specimen/Fira+Code) font [Open Font License (OFL)](https://openfontlicense.org/)
