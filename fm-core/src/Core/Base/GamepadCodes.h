#pragma once

#include "Core/Base/Base.h"

namespace fm {

	using GamepadButtonCode = Uint16;
	using GamepadAxisCode = Uint16;

	namespace Gamepad {

		enum : GamepadButtonCode {

			// From glfw3.h
			ButtonA = 0,
			ButtonB = 1,
			ButtonX = 2,
			ButtonY = 3,
			ButtonLeftBumper = 4,
			ButtonRightBumper = 5,
			ButtonBack = 6,
			ButtonStart = 7,
			ButtonGuide = 8,
			ButtonLeftThumb = 9,
			ButtonRightThumb = 10,
			ButtonDPadUp = 11,
			ButtonDPadRight = 12,
			ButtonDPadDown = 13,
			ButtonDPadLeft = 14,
			ButtonLast = ButtonDPadLeft,
			ButtonCross = ButtonA,
			ButtonCircle = ButtonB,
			ButtonSquare = ButtonX,
			ButtonTriangle = ButtonY
		};

		enum : GamepadAxisCode {

			// From glfw3.h
			AxisLeft = 0,
			AxisRight = 2,
			AxisLeftTrigger = 4,
			AxisRightTrigger = 5
		};
	}
}
