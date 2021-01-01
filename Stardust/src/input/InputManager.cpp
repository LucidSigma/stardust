#include "stardust/input/InputManager.h"

#include "stardust/input/Input.h"

namespace stardust
{
	void InputManager::SetButton(const String& buttonName, const KeyCode key)
	{
		if (!m_buttons.contains(buttonName))
		{
			m_buttons[buttonName].m_keys= { };
		}

		m_buttons[buttonName].m_keys.insert(key);
	}

	void InputManager::SetButton(const String& buttonName, const Vector<KeyCode>& keys)
	{
		if (!m_buttons.contains(buttonName))
		{
			m_buttons[buttonName] = { };
		}

		for (const auto key : keys)
		{
			m_buttons[buttonName].m_keys.insert(key);
		}
	}

	void InputManager::SetButton(const String& buttonName, const MouseButton button)
	{
		if (!m_buttons.contains(buttonName))
		{
			m_buttons[buttonName] = { };
		}

		m_buttons[buttonName].m_mouseButtons.insert(button);
	}

	void InputManager::SetButton(const String& buttonName, const Vector<MouseButton>& buttons)
	{
		if (!m_buttons.contains(buttonName))
		{
			m_buttons[buttonName] = { };
		}

		for (const auto button : buttons)
		{
			m_buttons[buttonName].m_mouseButtons.insert(button);
		}
	}

	void InputManager::SetButton(const String& buttonName, const GameControllerButton button)
	{
		if (!m_buttons.contains(buttonName))
		{
			m_buttons[buttonName] = { };
		}

		m_buttons[buttonName].m_controllerButtons.insert(button);
	}

	void InputManager::SetButton(const String& buttonName, const Vector<GameControllerButton>& buttons)
	{
		if (!m_buttons.contains(buttonName))
		{
			m_buttons[buttonName] = { };
		}

		for (const auto button : buttons)
		{
			m_buttons[buttonName].m_controllerButtons.insert(button);
		}
	}

	[[nodiscard]] bool InputManager::IsButtonDown(const String& buttonName, const Vector<ObserverPtr<const GameController>>& gameControllers) const
	{
		if (m_buttons.contains(buttonName))
		{
			const auto& buttons = m_buttons.at(buttonName);

			for (const auto key : buttons.m_keys)
			{
				if (Input::GetKeyboardState().IsKeyDown(key))
				{
					return true;
				}
			}

			for (const auto mouseButton : buttons.m_mouseButtons)
			{
				if (Input::GetMouseState().IsButtonDown(mouseButton))
				{
					return true;
				}
			}

			for (const auto& gameController : gameControllers)
			{
				for (const auto controllerButton : buttons.m_controllerButtons)
				{
					if (gameController != nullptr && gameController->IsButtonDown(controllerButton))
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	[[nodiscard]] bool InputManager::IsButtonPressed(const String& buttonName, const Vector<ObserverPtr<const GameController>>& gameControllers) const
	{
		if (m_buttons.contains(buttonName))
		{
			const auto& buttons = m_buttons.at(buttonName);

			for (const auto key : buttons.m_keys)
			{
				if (Input::GetKeyboardState().IsKeyPressed(key))
				{
					return true;
				}
			}

			for (const auto mouseButton : buttons.m_mouseButtons)
			{
				if (Input::GetMouseState().IsButtonPressed(mouseButton))
				{
					return true;
				}
			}

			for (const auto& gameController : gameControllers)
			{
				for (const auto controllerButton : buttons.m_controllerButtons)
				{
					if (gameController != nullptr && gameController->IsButtonPressed(controllerButton))
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	[[nodiscard]] bool InputManager::IsButtonUp(const String& buttonName, const Vector<ObserverPtr<const GameController>>& gameControllers) const
	{
		if (m_buttons.contains(buttonName))
		{
			const auto& buttons = m_buttons.at(buttonName);

			for (const auto key : buttons.m_keys)
			{
				if (Input::GetKeyboardState().IsKeyUp(key))
				{
					return true;
				}
			}

			for (const auto mouseButton : buttons.m_mouseButtons)
			{
				if (Input::GetMouseState().IsButtonUp(mouseButton))
				{
					return true;
				}
			}

			for (const auto& gameController : gameControllers)
			{
				for (const auto controllerButton : buttons.m_controllerButtons)
				{
					if (gameController != nullptr && gameController->IsButtonUp(controllerButton))
					{
						return true;
					}
				}
			}
		}

		return false;
	}
}