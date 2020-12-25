#pragma once
#ifndef STARDUST_KEYBOARD_H
#define STARDUST_KEYBOARD_H

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/input/keyboard/KeyCodes.h"

namespace stardust
{
	class Keyboard
	{
	private:
		const u8* m_currentKeys;
		const u8* m_previousKeys;

	public:
		Keyboard(const u8* currentKeys, const u8* previousKeys);
		~Keyboard() noexcept = default;

		[[nodiscard]] bool IsKeyDown(const KeyCode key) const;
		[[nodiscard]] bool IsKeyPressed(const KeyCode key) const;
		[[nodiscard]] bool IsKeyUp(const KeyCode key) const;

		[[nodiscard]] bool IsAnyKeyDown(const Vector<KeyCode>& keys) const;
		[[nodiscard]] bool IsAnyKeyPressed(const Vector<KeyCode>& keys) const;
		[[nodiscard]] bool IsAnyKeyUp(const Vector<KeyCode>& keys) const;

		[[nodiscard]] bool AreAllKeysDown(const Vector<KeyCode>& keys) const;
		[[nodiscard]] bool AreAllKeysPressed(const Vector<KeyCode>& keys) const;
		[[nodiscard]] bool AreAllKeysUp(const Vector<KeyCode>& keys) const;
	};

}

#endif