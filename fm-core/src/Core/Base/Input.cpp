#include "fmpch.h"

#include "Input.h"

namespace fm {

	Input::Input() {

		for (int i = 0; i < GamepadCount; i++)
			m_GamepadSlots[i] = GamepadInvalid;
	}

	void Input::AddGamepad(GamepadID id) {

		FM_ASSERT(id != GamepadInvalid, "Cannot add an Invalid value");

		s_Instance->m_GamepadSlots[s_Instance->m_CurrentEmptySlot] = id;

		for (int i = 0; i < GamepadCount; i++) {

			if (s_Instance->m_GamepadSlots[i] == GamepadInvalid) {

				s_Instance->m_CurrentEmptySlot += i;
				break;
			}
		}
	}

	void Input::RemoveGamepad(GamepadID id) {

		s_Instance->m_GamepadSlots[id] = GamepadInvalid;
		s_Instance->m_CurrentEmptySlot = id;
	}

	GamepadID Input::GetCurrentEmptySlot() {

		return s_Instance->m_CurrentEmptySlot;
	}
}
