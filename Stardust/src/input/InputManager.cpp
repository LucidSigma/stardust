#include "stardust/input/InputManager.h"

#include "stardust/input/Input.h"

namespace stardust
{
	void InputManager::AddToButton(const String& buttonName, const KeyCode key)
	{
		if (!m_buttons.contains(buttonName))
		{
			m_buttons[buttonName].keys= { };
		}

		m_buttons[buttonName].keys.insert(key);
	}

	void InputManager::AddToButton(const String& buttonName, const Vector<KeyCode>& keys)
	{
		if (!m_buttons.contains(buttonName))
		{
			m_buttons[buttonName] = { };
		}

		for (const auto key : keys)
		{
			m_buttons[buttonName].keys.insert(key);
		}
	}

	void InputManager::AddToButton(const String& buttonName, const MouseButton button)
	{
		if (!m_buttons.contains(buttonName))
		{
			m_buttons[buttonName] = { };
		}

		m_buttons[buttonName].mouseButtons.insert(button);
	}

	void InputManager::AddToButton(const String& buttonName, const Vector<MouseButton>& buttons)
	{
		if (!m_buttons.contains(buttonName))
		{
			m_buttons[buttonName] = { };
		}

		for (const auto button : buttons)
		{
			m_buttons[buttonName].mouseButtons.insert(button);
		}
	}

	void InputManager::AddToButton(const String& buttonName, const GameControllerButton button)
	{
		if (!m_buttons.contains(buttonName))
		{
			m_buttons[buttonName] = { };
		}

		m_buttons[buttonName].controllerButtons.insert(button);
	}

	void InputManager::AddToButton(const String& buttonName, const Vector<GameControllerButton>& buttons)
	{
		if (!m_buttons.contains(buttonName))
		{
			m_buttons[buttonName] = { };
		}

		for (const auto button : buttons)
		{
			m_buttons[buttonName].controllerButtons.insert(button);
		}
	}

	void InputManager::RemoveButton(const String& buttonName)
	{
		m_buttons.erase(buttonName);
	}

	void InputManager::RemoveFromButton(const String& buttonName, const KeyCode key)
	{
		if (m_buttons.contains(buttonName))
		{
			m_buttons[buttonName].keys.erase(key);
		}
	}

	void InputManager::RemoveFromButton(const String& buttonName, const MouseButton button)
	{
		if (m_buttons.contains(buttonName))
		{
			m_buttons[buttonName].mouseButtons.erase(button);
		}
	}

	void InputManager::RemoveFromButton(const String& buttonName, const GameControllerButton button)
	{
		if (m_buttons.contains(buttonName))
		{
			m_buttons[buttonName].controllerButtons.erase(button);
		}
	}

	[[nodiscard]] bool InputManager::IsButtonDown(const String& buttonName, const Vector<ObserverPtr<const GameController>>& gameControllers) const
	{
		if (m_buttons.contains(buttonName))
		{
			const auto& buttons = m_buttons.at(buttonName);

			for (const auto key : buttons.keys)
			{
				if (Input::GetKeyboardState().IsKeyDown(key))
				{
					return true;
				}
			}

			for (const auto mouseButton : buttons.mouseButtons)
			{
				if (Input::GetMouseState().IsButtonDown(mouseButton))
				{
					return true;
				}
			}

			for (const auto& gameController : gameControllers)
			{
				for (const auto controllerButton : buttons.controllerButtons)
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

			for (const auto key : buttons.keys)
			{
				if (Input::GetKeyboardState().IsKeyPressed(key))
				{
					return true;
				}
			}

			for (const auto mouseButton : buttons.mouseButtons)
			{
				if (Input::GetMouseState().IsButtonPressed(mouseButton))
				{
					return true;
				}
			}

			for (const auto& gameController : gameControllers)
			{
				for (const auto controllerButton : buttons.controllerButtons)
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

			for (const auto key : buttons.keys)
			{
				if (Input::GetKeyboardState().IsKeyUp(key))
				{
					return true;
				}
			}

			for (const auto mouseButton : buttons.mouseButtons)
			{
				if (Input::GetMouseState().IsButtonUp(mouseButton))
				{
					return true;
				}
			}

			for (const auto& gameController : gameControllers)
			{
				for (const auto controllerButton : buttons.controllerButtons)
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

	void InputManager::AddToAxis(const String& axisName, const AxisType axisType, const bool inverted)
	{
		if (!m_axes.contains(axisName))
		{
			m_axes[axisName] = { };
		}

		m_axes[axisName].axes.insert({ axisType, inverted });
	}

	void InputManager::AddToPositiveAxis(const String& axisName, const KeyCode key)
	{
		if (!m_axes.contains(axisName))
		{
			m_axes[axisName] = { };
		}

		m_axes[axisName].positiveKeys.insert(key);
	}

	void InputManager::AddToPositiveAxis(const String& axisName, const Vector<KeyCode>& keys)
	{
		if (!m_axes.contains(axisName))
		{
			m_axes[axisName] = { };
		}

		for (const auto key : keys)
		{
			m_axes[axisName].positiveKeys.insert(key);
		}
	}

	void InputManager::AddToPositiveAxis(const String& axisName, const MouseButton button)
	{
		if (!m_axes.contains(axisName))
		{
			m_axes[axisName] = { };
		}

		m_axes[axisName].positiveMouseButtons.insert(button);
	}

	void InputManager::AddToPositiveAxis(const String& axisName, const Vector<MouseButton>& buttons)
	{
		if (!m_axes.contains(axisName))
		{
			m_axes[axisName] = { };
		}

		for (const auto button : buttons)
		{
			m_axes[axisName].positiveMouseButtons.insert(button);
		}
	}

	void InputManager::AddToPositiveAxis(const String& axisName, const GameControllerButton button)
	{
		if (!m_axes.contains(axisName))
		{
			m_axes[axisName] = { };
		}

		m_axes[axisName].positiveControllerButtons.insert(button);
	}

	void InputManager::AddToPositiveAxis(const String& axisName, const Vector<GameControllerButton>& buttons)
	{
		if (!m_axes.contains(axisName))
		{
			m_axes[axisName] = { };
		}

		for (const auto button : buttons)
		{
			m_axes[axisName].positiveControllerButtons.insert(button);
		}
	}

	void InputManager::AddToNegativeAxis(const String& axisName, const KeyCode key)
	{
		if (!m_axes.contains(axisName))
		{
			m_axes[axisName] = { };
		}

		m_axes[axisName].negativeKeys.insert(key);
	}

	void InputManager::AddToNegativeAxis(const String& axisName, const Vector<KeyCode>& keys)
	{
		if (!m_axes.contains(axisName))
		{
			m_axes[axisName] = { };
		}

		for (const auto key : keys)
		{
			m_axes[axisName].negativeKeys.insert(key);
		}
	}

	void InputManager::AddToNegativeAxis(const String& axisName, const MouseButton button)
	{
		if (!m_axes.contains(axisName))
		{
			m_axes[axisName] = { };
		}

		m_axes[axisName].negativeMouseButtons.insert(button);
	}

	void InputManager::AddToNegativeAxis(const String& axisName, const Vector<MouseButton>& buttons)
	{
		if (!m_axes.contains(axisName))
		{
			m_axes[axisName] = { };
		}

		for (const auto button : buttons)
		{
			m_axes[axisName].negativeMouseButtons.insert(button);
		}
	}

	void InputManager::AddToNegativeAxis(const String& axisName, const GameControllerButton button)
	{
		if (!m_axes.contains(axisName))
		{
			m_axes[axisName] = { };
		}

		m_axes[axisName].negativeControllerButtons.insert(button);
	}

	void InputManager::AddToNegativeAxis(const String& axisName, const Vector<GameControllerButton>& buttons)
	{
		if (!m_axes.contains(axisName))
		{
			m_axes[axisName] = { };
		}

		for (const auto button : buttons)
		{
			m_axes[axisName].negativeControllerButtons.insert(button);
		}
	}

	void InputManager::RemoveAxis(const String& axisName)
	{
		m_axes.erase(axisName);
	}

	void InputManager::RemoveFromAxis(const String& axisName, const AxisType axisType)
	{
		if (m_axes.contains(axisName))
		{
			m_axes[axisName].axes.erase(axisType);
		}
	}

	void InputManager::RemoveFromPositiveAxis(const String& axisName, const KeyCode key)
	{
		if (m_axes.contains(axisName))
		{
			m_axes[axisName].positiveKeys.erase(key);
		}
	}

	void InputManager::RemoveFromPositiveAxis(const String& axisName, const MouseButton button)
	{
		if (m_axes.contains(axisName))
		{
			m_axes[axisName].positiveMouseButtons.erase(button);
		}
	}

	void InputManager::RemoveFromPositiveAxis(const String& axisName, const GameControllerButton button)
	{
		if (m_axes.contains(axisName))
		{
			m_axes[axisName].positiveControllerButtons.erase(button);
		}
	}

	void InputManager::RemoveFromNegativeAxis(const String& axisName, const KeyCode key)
	{
		if (m_axes.contains(axisName))
		{
			m_axes[axisName].negativeKeys.erase(key);
		}
	}

	void InputManager::RemoveFromNegativeAxis(const String& axisName, const MouseButton button)
	{
		if (m_axes.contains(axisName))
		{
			m_axes[axisName].negativeMouseButtons.erase(button);
		}
	}

	void InputManager::RemoveFromNegativeAxis(const String& axisName, const GameControllerButton button)
	{
		if (m_axes.contains(axisName))
		{
			m_axes[axisName].negativeControllerButtons.erase(button);
		}
	}
}