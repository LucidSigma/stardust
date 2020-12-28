#include "stardust/utility/Utility.h"

#include <SDL2/SDL.h>

namespace stardust
{
	namespace utility
	{
		void OpenURL(const StringView& url)
		{
			SDL_OpenURL(url.data());
		}
	}
}