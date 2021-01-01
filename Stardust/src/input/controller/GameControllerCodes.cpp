#include "stardust/input/controller/GameControllerCodes.h"

namespace stardust
{
	[[nodiscard]] String GameControllerButtonToString(const GameControllerButton controllerButton, const bool usePSnames)
	{
		switch (controllerButton)
		{
		case GameControllerButton::DPadUp:
			return "DPadUp";

		case GameControllerButton::DPadDown:
			return "DPadDown";

		case GameControllerButton::DPadLeft:
			return "DPadLeft";

		case GameControllerButton::DPadRight:
			return "DPadRight";

		case GameControllerButton::A:
			return usePSnames ? "X" : "A";

		case GameControllerButton::B:
			return usePSnames ? "Circle" : "B";

		case GameControllerButton::X:
			return usePSnames ? "Square" : "X";

		case GameControllerButton::Y:
			return usePSnames ? "Triangle" : "Y";

		case GameControllerButton::Back:
			return usePSnames ? "Share" : "Back";

		case GameControllerButton::Guide:
			return "Guide";

		case GameControllerButton::Start:
			return usePSnames ? "Options" : "Start";

		case GameControllerButton::LeftStick:
			return "LeftStick";

		case GameControllerButton::RightStick:
			return "RightStick";

		case GameControllerButton::LeftShoulder:
			return "LeftShoulder";

		case GameControllerButton::RightShoulder:
			return "RightShoulder";

		case GameControllerButton::Paddle1:
			return "Paddle1";

		case GameControllerButton::Paddle2:
			return "Paddle2";

		case GameControllerButton::Paddle3:
			return "Paddle3";

		case GameControllerButton::Paddle4:
			return "Paddle4";

		case GameControllerButton::Misc:
			return "Misc";

		case GameControllerButton::Touchpad:
			return "Touchpad";

		default:
			return "";
		}
	}
}