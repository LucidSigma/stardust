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
	private:
		HashMap<String, HashSet<KeyCode>> m_keys{ };
		HashMap<String, HashSet<MouseButton>> m_mouseButtons{ };
		HashMap<String, HashSet<GameControllerButton>> m_controllerButtons{ };

	public:
		InputManager() = default;
		~InputManager() noexcept = default;

		void SetButton(const String& buttonName, const KeyCode key);
		void SetButton(const String& buttonName, const Vector<KeyCode>& keys);
		void SetButton(const String& buttonName, const MouseButton button);
		void SetButton(const String& buttonName, const Vector<MouseButton>& buttons);
		void SetButton(const String& buttonName, const GameControllerButton button);
		void SetButton(const String& buttonName, const Vector<GameControllerButton>& buttons);

		[[nodiscard]] bool IsButtonDown(const String& buttonName, const Vector<ObserverPtr<const GameController>>& gameControllers = { }) const;
		[[nodiscard]] bool IsButtonPressed(const String& buttonName, const Vector<ObserverPtr<const GameController>>& gameControllers = { }) const;
		[[nodiscard]] bool IsButtonUp(const String& buttonName, const Vector<ObserverPtr<const GameController>>& gameControllers = { }) const;
	};
}

#endif