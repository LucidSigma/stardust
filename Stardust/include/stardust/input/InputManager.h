#pragma once
#ifndef STARDUST_INPUT_MANAGER_H
#define STARDUST_INPUT_MANAGER_H

#include "stardust/data/Containers.h"
#include "stardust/data/Pointers.h"
#include "stardust/input/controller/GameControllerCodes.h"
#include "stardust/input/controller/GameController.h"
#include "stardust/input/keyboard/KeyCodes.h"
#include "stardust/input/mouse/MouseButtonCodes.h"

namespace stardust
{
	class InputManager
	{
	public:
		enum class AxisType
		{
			MouseX,
			MouseY,
			MouseScroll,
			ControllerLeftX,
			ControllerLeftY,
			ControllerRightX,
			ControllerRightY,
			ControllerLeftTrigger,
			ControllerRightTrigger,
		};

	private:
		struct ButtonInput
		{
			HashSet<KeyCode> keys{ };
			HashSet<MouseButton> mouseButtons{ };
			HashSet<GameControllerButton> controllerButtons{ };
		};

		struct AxisInput
		{
			HashMap<AxisType, bool> axes{ };

			HashSet<KeyCode> positiveKeys{ };
			HashSet<KeyCode> negativeKeys{ };

			HashSet<MouseButton> positiveMouseButtons{ };
			HashSet<MouseButton> negativeMouseButtons{ };

			HashSet<GameControllerButton> positiveControllerButtons{ };
			HashSet<GameControllerButton> negativeControllerButtons{ };
		};

		HashMap<String, ButtonInput> m_buttons{ };
		HashMap<String, AxisInput> m_axes{ };

	public:
		InputManager() = default;
		~InputManager() noexcept = default;

		void AddToButton(const String& buttonName, const KeyCode key);
		void AddToButton(const String& buttonName, const Vector<KeyCode>& keys);
		void AddToButton(const String& buttonName, const MouseButton button);
		void AddToButton(const String& buttonName, const Vector<MouseButton>& buttons);
		void AddToButton(const String& buttonName, const GameControllerButton button);
		void AddToButton(const String& buttonName, const Vector<GameControllerButton>& buttons);

		void RemoveButton(const String& buttonName);
		void RemoveFromButton(const String& buttonName, const KeyCode key);
		void RemoveFromButton(const String& buttonName, const MouseButton button);
		void RemoveFromButton(const String& buttonName, const GameControllerButton button);

		[[nodiscard]] bool IsButtonDown(const String& buttonName, const Vector<ObserverPtr<const GameController>>& gameControllers = { }) const;
		[[nodiscard]] bool IsButtonPressed(const String& buttonName, const Vector<ObserverPtr<const GameController>>& gameControllers = { }) const;
		[[nodiscard]] bool IsButtonUp(const String& buttonName, const Vector<ObserverPtr<const GameController>>& gameControllers = { }) const;

		void AddToAxis(const String& axisName, const AxisType axisType, const bool inverted = false);

		void AddToPositiveAxis(const String& axisName, const KeyCode key);
		void AddToPositiveAxis(const String& axisName, const Vector<KeyCode>& keys);
		void AddToPositiveAxis(const String& axisName, const MouseButton button);
		void AddToPositiveAxis(const String& axisName, const Vector<MouseButton>& buttons);
		void AddToPositiveAxis(const String& axisName, const GameControllerButton button);
		void AddToPositiveAxis(const String& axisName, const Vector<GameControllerButton>& buttons);

		void AddToNegativeAxis(const String& axisName, const KeyCode key);
		void AddToNegativeAxis(const String& axisName, const Vector<KeyCode>& keys);
		void AddToNegativeAxis(const String& axisName, const MouseButton button);
		void AddToNegativeAxis(const String& axisName, const Vector<MouseButton>& buttons);
		void AddToNegativeAxis(const String& axisName, const GameControllerButton button);
		void AddToNegativeAxis(const String& axisName, const Vector<GameControllerButton>& buttons);

		void RemoveAxis(const String& axisName);
		void RemoveFromAxis(const String& axisName, const AxisType axisType);
		void RemoveFromPositiveAxis(const String& axisName, const KeyCode key);
		void RemoveFromPositiveAxis(const String& axisName, const MouseButton button);
		void RemoveFromPositiveAxis(const String& axisName, const GameControllerButton button);
		void RemoveFromNegativeAxis(const String& axisName, const KeyCode key);
		void RemoveFromNegativeAxis(const String& axisName, const MouseButton button);
		void RemoveFromNegativeAxis(const String& axisName, const GameControllerButton button);
	};
}

#endif