#include "Magma/Utility/Input.h"

namespace mg
{
	GLFWwindow* Input::m_window = nullptr;

	bool Input::IsKeyPressed(const KeyCode key)
	{
		auto state = glfwGetKey(*Input::GetWindow(), static_cast<int32_t>(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(const MouseCode button)
	{
		auto state = glfwGetMouseButton(*Input::GetWindow(), static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	ImVec2 Input::GetMousePosition()
	{
		double xpos, ypos;
		glfwGetCursorPos(*Input::GetWindow(), &xpos, &ypos);

		return ImVec2{ (float)xpos, (float)ypos };
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}
}