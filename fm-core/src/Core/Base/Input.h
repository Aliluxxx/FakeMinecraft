#pragma once

#include "Core/Base/Base.h"
#include "Core/Base/Vector2.h"
#include "Core/Base/KeyCodes.h"
#include "Core/Base/MouseCodes.h"
#include "Core/Base/GamepadCodes.h"

namespace fm {

	typedef int GamepadID;

	class FM_API Input {

	public:

		static const Uint32 GamepadCount = 16;
		static const GamepadID GamepadInvalid = -1;

		static bool IsKeyPressed(KeyCode keycode) {

			return s_Instance->IsKeyPressedImpl(keycode);
		}

		static bool IsMouseButtonPressed(MouseCode button) {

			return s_Instance->IsMouseButtonPressedImpl(button);
		}

		static Vector2i GetMousePosition() {

			return s_Instance->GetMousePositionImpl();
		}

		static float GetMouseX() {

			return s_Instance->GetMouseXImpl();
		}

		static float GetMouseY() {

			return s_Instance->GetMouseYImpl();
		}

		static std::vector<GamepadID> GetAvailableGamepads() {

			return std::vector<GamepadID>(s_Instance->m_GamepadSlots.begin(), s_Instance->m_GamepadSlots.end());
		}

		static Vector2f GetGamepadAxisOffset(GamepadID id, GamepadAxisCode axis) {

			return s_Instance->GetGamepadAxisOffsetImpl(id, axis);
		}

		static float GetGamepadAxisXOffset(GamepadID id, GamepadAxisCode axis) {

			return s_Instance->GetGamepadAxisXOffsetImpl(id, axis);
		}

		static float GetGamepadAxisYOffset(GamepadID id, GamepadAxisCode axis) {

			return s_Instance->GetGamepadAxisYOffsetImpl(id, axis);
		}

		static bool IsGamepadButtonPressed(GamepadID id, GamepadButtonCode button) {

			return s_Instance->IsGamepadButtonPressedImpl(id, button);
		}

	protected:

		Input();

		virtual bool IsKeyPressedImpl(KeyCode keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
		virtual Vector2i GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

		static void AddGamepad(GamepadID id);
		static void RemoveGamepad(GamepadID id);
		static GamepadID GetCurrentEmptySlot();

		virtual Vector2f GetGamepadAxisOffsetImpl(GamepadID id, GamepadAxisCode axis) = 0;
		virtual float GetGamepadAxisXOffsetImpl(GamepadID id, GamepadAxisCode axis) = 0;
		virtual float GetGamepadAxisYOffsetImpl(GamepadID id, GamepadAxisCode axis) = 0;
		virtual bool IsGamepadButtonPressedImpl(GamepadID id, GamepadButtonCode button) = 0;

		static Scope<Input> s_Instance;

		std::array<GamepadID, GamepadCount> m_GamepadSlots;
		GamepadID m_CurrentEmptySlot = 0;
	};
}
