#include "Magma/Utility/Input.h"

namespace mg
{
	namespace Utils
	{
		static ImGuiMouseCursor GetImGuiFromCursorShape(CursorShape shape)
		{
			switch (shape)
			{
			case mg::CursorShape::Arrow:       return ImGuiMouseCursor_Arrow;
			// case mg::CursorShape::IBeam:       return ImGuiMouseCursor_Arrow; // not supported in ImGui
			// case mg::CursorShape::Crosshair:   return ImGuiMouseCursor_Arrow; // not supported in ImGui
			case mg::CursorShape::PointingHand:return ImGuiMouseCursor_Hand;
			case mg::CursorShape::ResizeEW:    return ImGuiMouseCursor_ResizeEW;
			case mg::CursorShape::ResizeNS:    return ImGuiMouseCursor_ResizeNS;
			case mg::CursorShape::ResizeNWSE:  return ImGuiMouseCursor_ResizeNWSE;
			case mg::CursorShape::ResizeNESW:  return ImGuiMouseCursor_ResizeNESW;
			case mg::CursorShape::ResizeAll:   return ImGuiMouseCursor_ResizeAll;
			case mg::CursorShape::NotAllowed:  return ImGuiMouseCursor_NotAllowed;
			case mg::CursorShape::HResize:     return ImGuiMouseCursor_ResizeEW;
			case mg::CursorShape::VResize:     return ImGuiMouseCursor_ResizeNS;
			default:                            return ImGuiMouseCursor_Arrow;
			}

		}

	}


	GLFWwindow* Input::m_window = nullptr;
	// GLFWcursor* Input::m_currentCursor = nullptr; // not needed

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

	void Input::SetCursorMode(CursorMode mode)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int)mode);
	}

	void Input::SetCursorShape(CursorShape shape)
	{
		ImGui::SetMouseCursor(Utils::GetImGuiFromCursorShape(shape));

		/* doesn't work with glfw because imgui changes the cursor
		cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR + (int)shape);
		glfwSetCursor(m_window, cursor); // renders both the selected cursor and the normal one
		*/
	}
}