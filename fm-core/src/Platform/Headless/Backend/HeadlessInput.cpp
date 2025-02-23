#include "fmpch.h"

#include "HeadlessInput.h"

namespace fm {

	Scope<Input> Input::s_Instance = CreateScope<HeadlessInput>();

	bool HeadlessInput::IsKeyPressedImpl(KeyCode keycode) {

		return false;
	}

	bool HeadlessInput::IsMouseButtonPressedImpl(MouseCode button) {

		return false;
	}

	Vector2i HeadlessInput::GetMousePositionImpl() {

		return { 0.0f, 0.0f };
	}

	float HeadlessInput::GetMouseXImpl() {

		return 0.0f;
	}

	float HeadlessInput::GetMouseYImpl() {

		return 0.0f;
	}

	Vector2f HeadlessInput::GetGamepadAxisOffsetImpl(GamepadID id, GamepadAxisCode axis) {

		return { 0.0f, 0.0f };
	}

	float HeadlessInput::GetGamepadAxisXOffsetImpl(GamepadID id, GamepadAxisCode axis) {

		return 0.0f;
	}

	float HeadlessInput::GetGamepadAxisYOffsetImpl(GamepadID id, GamepadAxisCode axis) {

		return 0.0f;
	}

	bool HeadlessInput::IsGamepadButtonPressedImpl(GamepadID id, GamepadButtonCode button) {

		return false;
	}
}
