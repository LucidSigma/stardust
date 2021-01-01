#include "stardust/input/InputManager.h"

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

	void InputManager::SetButton(const String& buttonName, const Vector<KeyCode> keys)
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

	void InputManager::SetButton(const String& buttonName, const Vector<MouseButton> buttons)
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

	void InputManager::SetButton(const String& buttonName, const Vector<GameControllerButton> buttons)
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
}