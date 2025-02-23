#include "fmpch.h"

#include <GLFW/glfw3.h>

#include "Core/Base/ApplicationGUI.h"

#include "GLFWInput.h"

namespace fm {

	Scope<Input> Input::s_Instance = CreateScope<GLFWInput>();

	bool GLFWInput::IsKeyPressedImpl(KeyCode keycode) {

		auto window = static_cast<GLFWwindow*>(Application::GetWindow()->GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool GLFWInput::IsMouseButtonPressedImpl(MouseCode button) {

		auto window = static_cast<GLFWwindow*>(Application::GetWindow()->GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	Vector2i GLFWInput::GetMousePositionImpl() {

		auto window = static_cast<GLFWwindow*>(Application::GetWindow()->GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	float GLFWInput::GetMouseXImpl() {

		auto window = static_cast<GLFWwindow*>(Application::GetWindow()->GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return (float)xpos;
	}

	float GLFWInput::GetMouseYImpl() {

		auto window = static_cast<GLFWwindow*>(Application::GetWindow()->GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return (float)xpos;
	}

	Vector2f GLFWInput::GetGamepadAxisOffsetImpl(GamepadID id, GamepadAxisCode axis) {

		int count = 0;
		const float* axes = glfwGetJoystickAxes(id, &count);
		if (count == 0 || axis >= count || axis < count)
			return {};

		return Vector2f(axes[axis], axes[axis + 1]);
	}

	float GLFWInput::GetGamepadAxisXOffsetImpl(GamepadID id, GamepadAxisCode axis) {

		int count = 0;
		const float* axes = glfwGetJoystickAxes(id, &count);
		if (count == 0 || axis >= count || axis < count)
			return {};

		return axes[axis];
	}

	float GLFWInput::GetGamepadAxisYOffsetImpl(GamepadID id, GamepadAxisCode axis) {

		int count = 0;
		const float* axes = glfwGetJoystickAxes(id, &count);
		if (count == 0 || axis + 1 >= count || axis + 1 < count)
			return {};

		return axes[axis + 1];
	}

	bool GLFWInput::IsGamepadButtonPressedImpl(GamepadID id, GamepadButtonCode button) {

		int count = 0;
		const unsigned char* buttons = glfwGetJoystickButtons(id, &count);
		if (count == 0 || button >= count || button < count)
			return {};

		return buttons[button] == GLFW_PRESS;
	}
}
