#pragma once

#include "Core/Base/Input.h"

namespace fm {

	class FM_API HeadlessInput : public Input {

	protected:

		virtual bool IsKeyPressedImpl(KeyCode keycode) override;
		virtual bool IsMouseButtonPressedImpl(MouseCode button) override;
		virtual Vector2i GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
		virtual Vector2f GetGamepadAxisOffsetImpl(GamepadID id, GamepadAxisCode axis) override;
		virtual float GetGamepadAxisXOffsetImpl(GamepadID id, GamepadAxisCode axis) override;
		virtual float GetGamepadAxisYOffsetImpl(GamepadID id, GamepadAxisCode axis) override;
		virtual bool IsGamepadButtonPressedImpl(GamepadID id, GamepadButtonCode button) override;
	};
}
