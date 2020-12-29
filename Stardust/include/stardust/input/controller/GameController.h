#pragma once
#ifndef STARDUST_GAME_CONTROLLER_H
#define STARDUST_GAME_CONTROLLER_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/input/controller/GameControllerCodes.h"

namespace stardust
{
	class GameController
		: private INoncopyable

	{
	public:
		struct ButtonState
		{
			struct DPad
			{
				bool up;
				bool down;
				bool left;
				bool right;
			} dPad;

			bool a;
			bool b;
			bool x;
			bool y;

			bool back;
			bool guide;
			bool start;

			bool leftStick;
			bool rightStick;

			bool leftShoulder;
			bool rightShoulder;

			bool misc;
			bool touchPad;

			Array<bool, 4u> paddles;
		};

		struct Axes
		{
			struct Thumbstick
			{
				i16 x;
				i16 y;
			};

			Thumbstick left;
			Thumbstick right;

			i16 leftTrigger;
			i16 rightTrigger;
		};

	private:
		struct GameControllerDestroyer
		{
			void operator ()(SDL_GameController* const gameController) const noexcept;
		};

		i32 m_id = 0;
		u32 m_playerIndex = 0u;

		ButtonState m_currentButtons;
		ButtonState m_previousButtons;
		Axes m_axes;

		UniquePtr<SDL_GameController, GameControllerDestroyer> m_handle = nullptr;

	public:
		friend class Input;

		explicit GameController(const i32 id);

		GameController(GameController&& other) noexcept;
		GameController& operator =(GameController&& other) noexcept;

		~GameController() noexcept = default;

		[[nodiscard]] bool IsButtonDown(const GameControllerButton button) const;
		[[nodiscard]] bool IsButtonPressed(const GameControllerButton button) const;
		[[nodiscard]] bool IsButtonUp(const GameControllerButton button) const;

		[[nodiscard]] bool IsAnyButtonDown(const Vector<GameControllerButton>& buttons) const;
		[[nodiscard]] bool IsAnyButtonPressed(const Vector<GameControllerButton>& buttons) const;
		[[nodiscard]] bool IsAnyButtonUp(const Vector<GameControllerButton>& buttons) const;

		[[nodiscard]] bool AreAllButtonsDown(const Vector<GameControllerButton>& buttons) const;
		[[nodiscard]] bool AreAllButtonsPressed(const Vector<GameControllerButton>& buttons) const;
		[[nodiscard]] bool AreAllButtonsUp(const Vector<GameControllerButton>& buttons) const;

		inline i32 GetID() const noexcept { return m_id; }
		inline const Axes& GetAxes() const noexcept { return m_axes; }

		inline u32 GetPlayerIndex() const noexcept { return m_playerIndex; }
		inline void SetPlayerIndex(const u32 playerIndex) noexcept { m_playerIndex = playerIndex; }

		[[nodiscard]] inline SDL_GameController* const GetRawHandle() const noexcept { return m_handle.get(); }
		[[nodiscard]] SDL_Joystick* const GetRawJoystickHandle() const;

	private:
		static bool GetButtonState(const GameControllerButton button, const ButtonState& buttonState) noexcept;
	};
}

#endif