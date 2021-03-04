#include "stardust/application/events/Events.h"

namespace stardust
{
	[[nodiscard]] bool IsEventEnqueued(const EventType eventType) noexcept
	{
		return SDL_HasEvent(static_cast<SDL_EventType>(eventType)) == SDL_TRUE;
	}

	[[nodiscard]] bool IsEventTypeEnabled(const EventType eventType) noexcept
	{
		return SDL_EventState(static_cast<SDL_EventType>(eventType), SDL_QUERY) == SDL_ENABLE;
	}

	void SetEventState(const EventType eventType, const bool isEnabled) noexcept
	{
		SDL_EventState(static_cast<SDL_EventType>(eventType), isEnabled ? SDL_ENABLE : SDL_DISABLE);
	}

	Event WaitForEvent()
	{
		Event event{ };
		SDL_WaitEvent(&event);

		return event;
	}

	Optional<Event> WaitForEvent(const u32 millisecondTimeout)
	{
		Event event{ };
		
		if (SDL_WaitEventTimeout(&event, static_cast<i32>(millisecondTimeout)) == 0)
		{
			return NullOpt;
		}
		else
		{
			return event;
		}
	}
}