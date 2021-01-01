#include "stardust/input/InputManager.h"

#include "stardust/input/Input.h"

namespace stardust
{
	void InputManager::SetButton(const String& buttonName, const KeyCode key)
	{
		if (!m_keys.contains(buttonName))
		{
			m_keys[buttonName] = { };
		}

		m_keys[buttonName].insert(key);
	}

	void InputManager::SetButton(const String& buttonName, const Vector<KeyCode>& keys)
	{
		if (!m_keys.contains(buttonName))
		{
			m_keys[buttonName] = { };
		}

		for (const auto key : keys)
		{
			m_keys[buttonName].insert(key);
		}
	}

	void InputManager::SetButton(const String& buttonName, const MouseButton button)
	{
		if (!m_mouseButtons.contains(buttonName))
		{
			m_mouseButtons[buttonName] = { };
		}

		m_mouseButtons[buttonName].insert(button);
	}

	void InputManager::SetButton(const String& buttonName, const Vector<MouseButton>& buttons)
	{
		if (!m_mouseButtons.contains(buttonName))
		{
			m_mouseButtons[buttonName] = { };
		}

		for (const auto button : buttons)
		{
			m_mouseButtons[buttonName].insert(button);
		}
	}

	void InputManager::SetButton(const String& buttonName, const GameControllerButton button)
	{
		if (!m_controllerButtons.contains(buttonName))
		{
			m_controllerButtons[buttonName] = { };
		}

		m_controllerButtons[buttonName].insert(button);
	}

	void InputManager::SetButton(const String& buttonName, const Vector<GameControllerButton>& buttons)
	{
		if (!m_controllerButtons.contains(buttonName))
		{
			m_controllerButtons[buttonName] = { };
		}

		for (const auto button : buttons)
		{
			m_controllerButtons[buttonName].insert(button);
		}
	}

	[[nodiscard]] bool InputManager::IsButtonDown(const String& buttonName, const Vector<ObserverPtr<const GameController>>& gameControllers) const
	{
		if (m_keys.contains(buttonName))
		{
			const auto& keys = m_keys.at(buttonName);

			for (const auto key : keys)
			{
				if (Input::GetKeyboardState().IsKeyDown(key))
				{
					return true;
				}
			}
		}

		if (m_mouseButtons.contains(buttonName))
		{
			const auto& buttons = m_mouseButtons.at(buttonName);

			for (const auto button : buttons)
			{
				if (Input::GetMouseState().IsButtonDown(button))
				{
					return true;
				}
			}
		}

		if (m_controllerButtons.contains(buttonName))
		{
			const auto& buttons = m_controllerButtons.at(buttonName);

			for (const auto& gameController : gameControllers)
			{
				for (const auto button : buttons)
				{
					if (gameController != nullptr && gameController->IsButtonDown(button))
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
		if (m_keys.contains(buttonName))
		{
			const auto& keys = m_keys.at(buttonName);

			for (const auto key : keys)
			{
				if (Input::GetKeyboardState().IsKeyPressed(key))
				{
					return true;
				}
			}
		}

		if (m_mouseButtons.contains(buttonName))
		{
			const auto& buttons = m_mouseButtons.at(buttonName);

			for (const auto button : buttons)
			{
				if (Input::GetMouseState().IsButtonPressed(button))
				{
					return true;
				}
			}
		}

		if (m_controllerButtons.contains(buttonName))
		{
			const auto& buttons = m_controllerButtons.at(buttonName);

			for (const auto& gameController : gameControllers)
			{
				for (const auto button : buttons)
				{
					if (gameController != nullptr && gameController->IsButtonPressed(button))
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
		if (m_keys.contains(buttonName))
		{
			const auto& keys = m_keys.at(buttonName);

			for (const auto key : keys)
			{
				if (Input::GetKeyboardState().IsKeyUp(key))
				{
					return true;
				}
			}
		}

		if (m_mouseButtons.contains(buttonName))
		{
			const auto& buttons = m_mouseButtons.at(buttonName);

			for (const auto button : buttons)
			{
				if (Input::GetMouseState().IsButtonUp(button))
				{
					return true;
				}
			}
		}

		if (m_controllerButtons.contains(buttonName))
		{
			const auto& buttons = m_controllerButtons.at(buttonName);

			for (const auto& gameController : gameControllers)
			{
				for (const auto button : buttons)
				{
					if (gameController != nullptr && gameController->IsButtonUp(button))
					{
						return true;
					}
				}
			}
		}

		return false;
	}
}